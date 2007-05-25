//
// Package:         GutSoftAnalyzers/GutSoftSeedingEfficiencyAnalyzer
// Class:           GutSoftSeedingEfficiencyAnalyzer
// 
// Description:     EDAnalyzer for seeding efficiency studies
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Thu Feb  8 19:03:24 UTC 2007
//
// $Author: gutsche $
// $Date: 2007/03/28 20:16:15 $
// $Revision: 1.4 $
//

#include <string>

#include "GutSoftAnalyzers/GutSoftSeedingEfficiencyAnalyzer/interface/GutSoftSeedingEfficiencyAnalyzer.h"

#include "GutSoftTools/GutSoftHistogramFileService/interface/GutSoftHistogramFileService.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/TrajectorySeed/interface/TrajectorySeed.h"
#include "DataFormats/TrajectorySeed/interface/TrajectorySeedCollection.h"

#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticleFwd.h"

#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"

#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"

#include "RecoTracker/RoadMapRecord/interface/RoadMapRecord.h"

#include "TrackingTools/RoadSearchHitAccess/interface/RoadSearchDetIdHelper.h"

#include "DataFormats/SiStripDetId/interface/StripSubdetector.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "DataFormats/SiStripDetId/interface/TIBDetId.h"
#include "DataFormats/SiStripDetId/interface/TOBDetId.h"
#include "DataFormats/SiStripDetId/interface/TIDDetId.h"
#include "DataFormats/SiStripDetId/interface/TECDetId.h"
#include "DataFormats/SiPixelDetId/interface/PXBDetId.h"
#include "DataFormats/SiPixelDetId/interface/PXFDetId.h"

#include "RecoTracker/RoadSearchSeedFinder/interface/RoadSearchCircleSeed.h"

GutSoftSeedingEfficiencyAnalyzer::GutSoftSeedingEfficiencyAnalyzer(const edm::ParameterSet& conf) : 
conf_(conf),
trackingParticleSelector_(conf.getParameter<edm::ParameterSet>("TrackingParticleCuts"))
{
  
  trajectorySeedInputTag_      = conf_.getUntrackedParameter<edm::InputTag>("TrajectorySeedInputTag");
  trackingTruthInputTag_       = conf_.getUntrackedParameter<edm::InputTag>("TrackingTruthInputTag");
  simTracksInputTag_           = conf_.getUntrackedParameter<edm::InputTag>("SimTracksInputTag");
  
  baseDirectoryName_           = conf_.getUntrackedParameter<std::string>("BaseDirectoryName");
  
  trackingParticleHitsInInnerSeedRings_ = conf_.getUntrackedParameter<unsigned int>("TrackingParticleHitsInInnerSeedRings");
  trackingParticleHitsInOuterSeedRings_ = conf_.getUntrackedParameter<unsigned int>("TrackingParticleHitsInOuterSeedRings");
  trackingParticleHitsInSeedRings_      = conf_.getUntrackedParameter<unsigned int>("TrackingParticleHitsInSeedRings");
  
  roadsLabel_ = conf_.getUntrackedParameter<std::string>("RoadsLabel");
  ringsLabel_ = conf_.getUntrackedParameter<std::string>("RingsLabel");
  
  // GutSoftHistogramFactory
  histograms_ = edm::Service<GutSoftHistogramFileService>()->getFactory();
  
}


GutSoftSeedingEfficiencyAnalyzer::~GutSoftSeedingEfficiencyAnalyzer()
{
  
}


void
GutSoftSeedingEfficiencyAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  
  // set baseDirectory in GutSoftHistogramFactory
  histograms_->setBaseDirectory(baseDirectoryName_);
  
  // get collections
  
  // get tracking particle collection for true information
  edm::Handle<TrackingParticleCollection>  trackingParticleCollectionHandle;
  iEvent.getByLabel(trackingTruthInputTag_,trackingParticleCollectionHandle);
  const TrackingParticleCollection *trackingParticleCollection = trackingParticleCollectionHandle.product();
  
  // get seed collection
  const TrajectorySeedCollection *seedCollection = 0;
  try {
    edm::Handle<TrajectorySeedCollection> seedCollectionHandle;
    iEvent.getByLabel(trajectorySeedInputTag_,seedCollectionHandle);
    seedCollection = seedCollectionHandle.product();
  }
  catch (edm::Exception const& x) {
    if ( x.categoryCode() == edm::errors::ProductNotFound ) {
      if ( x.history().size() == 1 ) {
        static const TrajectorySeedCollection s_empty;
        seedCollection = &s_empty;
        edm::LogWarning("GutSoftSeedingEfficiencyAnalyzer") << "Collection TrajectorySeedCollection with label " << trajectorySeedInputTag_ << " cannot be found, using empty collection of same type";
      }
    }
  }
  
  // get simTracks
  edm::Handle<edm::SimTrackContainer> simTrackHandle;
  iEvent.getByLabel(simTracksInputTag_, simTrackHandle);
  const edm::SimTrackContainer *simTracks = simTrackHandle.product();

  // get trackinghit associator
  TrackerHitAssociator associator(iEvent, conf_);
  
  // barCode vectors
  std::vector<int> trueBarCodes;
  std::vector<int> trueRSBarCodes;

  // clear barCode map
  trueParticles_.clear();

  for ( TrackingParticleCollection::const_iterator trackingParticle = trackingParticleCollection->begin(),
	  trackingParticleEnd = trackingParticleCollection->end();
        trackingParticle != trackingParticleEnd;
        ++trackingParticle ) {
    
    // apply tracking particle cuts
    if ( trackingParticleSelector_( *trackingParticle ) ) {
      if ( trackingParticle->charge() != 0 ) {

	// check if trackingParticle is seebable by RS
	bool seedable = false;
	unsigned int hitsInInnerRings = hitsInRings(innerSeedRings_,&(*trackingParticle));
	unsigned int hitsInOuterRings = hitsInRings(outerSeedRings_,&(*trackingParticle));
	if ( hitsInInnerRings >= trackingParticleHitsInInnerSeedRings_ ) {
	  if ( hitsInOuterRings >= trackingParticleHitsInOuterSeedRings_ ) {
	    if ( (hitsInInnerRings+hitsInOuterRings) >= trackingParticleHitsInSeedRings_ ) {
	      seedable = true;
	    }
	  }
	}

	// fill barcode vectors
	TrackingParticle::GenParticleRefVector genParticles = trackingParticle->genParticle();
	for ( TrackingParticle::GenParticleRefVector::const_iterator hepMCParticle = genParticles.begin(),
		hepMCParticleEnd = genParticles.end();
	      hepMCParticle != hepMCParticleEnd;
	      ++hepMCParticle ) {
	  int barCode = (*hepMCParticle)->barcode();
	  if ( barCode != 0 ) {
	    trueBarCodes.push_back(barCode);
	    trueParticles_[barCode] = *(*hepMCParticle);
	    if ( seedable ) {
	      trueRSBarCodes.push_back(barCode);
	    }
	  }
	}
      }
    }
  }

  // sort and unique
  std::sort(trueBarCodes.begin(),trueBarCodes.end());
  trueBarCodes.erase(std::unique(trueBarCodes.begin(),trueBarCodes.end()),trueBarCodes.end());
  std::sort(trueRSBarCodes.begin(),trueRSBarCodes.end());
  trueRSBarCodes.erase(std::unique(trueRSBarCodes.begin(),trueRSBarCodes.end()),trueRSBarCodes.end());

  // determine barCodes corresponding to a seed (a seed has three hits belonging to the same barCode)
  std::vector<int> foundBarCodes;
  for ( TrajectorySeedCollection::const_iterator seed = seedCollection->begin(), 
	  seedsEnd = seedCollection->end();
	seed != seedsEnd; 
	++seed ) {

    std::vector<const TrackingRecHit*> recHits;
    std::vector<GlobalPoint>           recHitGlobalPoints;
    std::vector<const Ring*>           recHitRings;
    std::vector<std::vector<int> >     recHitBarCodes;
    for ( TrajectorySeed::const_iterator hit = seed->recHits().first , hitEnd = seed->recHits().second;
	  hit != hitEnd;
	  ++hit ) {
      recHits.push_back(&*hit);
      DetId id = hit->geographicalId();
      recHitGlobalPoints.push_back(tracker_->idToDet(id)->surface().toGlobal(hit->localPosition()));
      recHitRings.push_back(rings_->getRing(RoadSearchDetIdHelper::ReturnRPhiId(id)));
      recHitBarCodes.push_back(GenParticleBarCodeFromRecHit(associator,simTracks,&*hit));
    }

    bool firstInLoop = true;
    int savedBarCode = 0;
    if ( recHits.size() == 3 ) {
      RoadSearchCircleSeed circle(recHits[0],
				  recHits[1],
				  recHits[2],
				  recHitGlobalPoints[0],
				  recHitGlobalPoints[1],
				  recHitGlobalPoints[2]);
      for (unsigned int i = 0;
	   i < recHits.size();
	   ++i ) {

	int barCode = recHitBarCodes[i].front();
	if ( firstInLoop ) {
	  savedBarCode = barCode;
	  firstInLoop = false;
	} else {
	  if ( barCode != savedBarCode ) {
	    savedBarCode = 0;
	  }
	}
      }
    }
    if ( savedBarCode != 0 &&
	 savedBarCode != -1 ) {
      foundBarCodes.push_back(savedBarCode);
    }
  } // end of loop over seeds
  
  // sort and unique
  std::sort(foundBarCodes.begin(),foundBarCodes.end());
  foundBarCodes.erase(std::unique(foundBarCodes.begin(),foundBarCodes.end()),foundBarCodes.end());

  std::vector<int> missedBarCodes;
  std::vector<int> trueFoundBarCodes;
  for ( std::vector<int>::iterator trueBarCode = trueBarCodes.begin(),
	  trueBarCodeEnd = trueBarCodes.end();
	trueBarCode != trueBarCodeEnd;
	++ trueBarCode ) {
    bool missed = true;
    for ( std::vector<int>::iterator foundBarCode = foundBarCodes.begin(),
	    foundBarCodeEnd = foundBarCodes.end();
	  foundBarCode != foundBarCodeEnd;
	  ++ foundBarCode ) {
      if ( *trueBarCode == *foundBarCode ) {
	trueFoundBarCodes.push_back(*trueBarCode);
	missed = false;
	break;
      }
    }
    if ( missed ) {
      missedBarCodes.push_back(*trueBarCode);
    }
  }
  if ( missedBarCodes.size() > 0 ) {
    missingBarCodes_.push_back(missedBarCodes);
    percentages_.push_back((double)missedBarCodes.size()/(double)trueBarCodes.size());
    runs_.push_back(iEvent.id().run());
    events_.push_back(iEvent.id().event());
  }

  std::vector<int> missedRSBarCodes;
  for ( std::vector<int>::iterator trueRSBarCode = trueRSBarCodes.begin(),
	  trueRSBarCodeEnd = trueRSBarCodes.end();
	trueRSBarCode != trueRSBarCodeEnd;
	++ trueRSBarCode ) {
    bool missed = true;
    for ( std::vector<int>::iterator foundBarCode = foundBarCodes.begin(),
	    foundBarCodeEnd = foundBarCodes.end();
	  foundBarCode != foundBarCodeEnd;
	  ++ foundBarCode ) {
      if ( *trueRSBarCode == *foundBarCode ) {
	missed = false;
	break;
      }
    }
    if ( missed ) {
      missedRSBarCodes.push_back(*trueRSBarCode);
    }
  }
  if ( missedRSBarCodes.size() > 0 ) {
    missingRSBarCodes_.push_back(missedRSBarCodes);
    percentagesRS_.push_back((double)missedRSBarCodes.size()/(double)trueRSBarCodes.size());
    runsRS_.push_back(iEvent.id().run());
    eventsRS_.push_back(iEvent.id().event());
  }

  // fill histograms
  for ( std::vector<int>::iterator trueBarCode = trueBarCodes.begin(),
	  trueBarCodeEnd = trueBarCodes.end();
	trueBarCode != trueBarCodeEnd;
	++ trueBarCode ) {
    histograms_->fill("eff_eta_true",trueParticles_[*trueBarCode].momentum().eta());
    histograms_->fill("eff_pt_true",trueParticles_[*trueBarCode].momentum().perp());
  }

  for ( std::vector<int>::iterator trueRSBarCode = trueRSBarCodes.begin(),
	  trueRSBarCodeEnd = trueRSBarCodes.end();
	trueRSBarCode != trueRSBarCodeEnd;
	++ trueRSBarCode ) {
    histograms_->fill("eff_eta_rs_true",trueParticles_[*trueRSBarCode].momentum().eta());
    histograms_->fill("eff_pt_rs_true",trueParticles_[*trueRSBarCode].momentum().perp());
  }

  for ( std::vector<int>::iterator trueFoundBarCode = trueFoundBarCodes.begin(),
	  trueFoundBarCodeEnd = trueFoundBarCodes.end();
	trueFoundBarCode != trueFoundBarCodeEnd;
	++ trueFoundBarCode ) {
    histograms_->fill("eff_eta_trueNrec",trueParticles_[*trueFoundBarCode].momentum().eta());
    histograms_->fill("eff_pt_trueNrec",trueParticles_[*trueFoundBarCode].momentum().perp());
  }

}


void 
GutSoftSeedingEfficiencyAnalyzer::beginJob(const edm::EventSetup &es)
{
  
  // get tracker geometry
  edm::ESHandle<TrackerGeometry> tracker;
  es.get<TrackerDigiGeometryRecord>().get(tracker);
  tracker_ = tracker.product();
  
  // get rings
  edm::ESHandle<Rings> ringsHandle;
  es.get<RingRecord>().get(ringsLabel_, ringsHandle);
  rings_ = ringsHandle.product();

  // get roads
  edm::ESHandle<Roads> roads;
  es.get<RoadMapRecord>().get(roadsLabel_, roads);
  roads_ = roads.product();
  
  // fill vectors of inner and outer seed rings from roads
  for ( Roads::const_iterator road = roads_->begin(); road != roads_->end(); ++road ) {
    Roads::RoadSeed seed = (*road).first;
    for ( std::vector<const Ring*>::const_iterator innerSeedRing = seed.first.begin();
          innerSeedRing != seed.first.end();
          ++innerSeedRing) {
      innerSeedRings_.push_back(*innerSeedRing);
    }
    for ( std::vector<const Ring*>::const_iterator outerSeedRing = seed.second.begin();
          outerSeedRing != seed.second.end();
          ++outerSeedRing) {
      outerSeedRings_.push_back(*outerSeedRing);
    }
  }
  
  std::string  directory = baseDirectoryName_;
  
  // binning for histograms
  unsigned int eta_nbins = 60;
  double       eta_low   = -3.;
  double       eta_high  =  3.;
  
  unsigned int pt_nbins = 60;
  double       pt_low   =  0.;
  double       pt_high  = 30.;
  
  unsigned int nhit_nbins = 31;
  double       nhit_low   = -0.5;
  double       nhit_high  = 30.5;
  
  // layers: PXB: 1-3, PXF: 4-5, TIB: 6-9, TID: 10-12, TOB: 13-18, TEC: 19-27
  unsigned int layer_nbins = 28;
  double       layer_low   = -0.5;
  double       layer_high  = 27.5;
  
  // transversal impact parameter
  unsigned int tip_nbins   = 2000;
  double tip_low           = 0.0;
  double tip_high          = 100.0;
  
  // longitudinal impact parameter
  unsigned int lip_nbins   = 2000;
  double lip_low           = 0.0;
  double lip_high          = 100.0;
  
  // book histogram
  histograms_->bookHistogram("eff_eta_true","TrackingParticle #eta",
                             directory,eta_nbins,eta_low,eta_high,
                             "#eta","Events");
  histograms_->bookHistogram("eff_eta_rs_true","TrackingParticle #eta for RS seeding algorithm",
                             directory,eta_nbins,eta_low,eta_high,
                             "#eta","Events");
  histograms_->bookHistogram("eff_eta_trueNrec","seeded TrackingParticle #eta",
                             directory,eta_nbins,eta_low,eta_high,
                             "#eta","Events");

  histograms_->bookHistogram("eff_pt_true","TrackingParticle p_{T} [GeV]",
                             directory,pt_nbins,pt_low,pt_high,
                             "p_{T} [GeV]","Events");
  histograms_->bookHistogram("eff_pt_rs_true","TrackingParticle p_{T} [GeV] for RS seeding algorithm",
                             directory,pt_nbins,pt_low,pt_high,
                             "p_{T} [GeV]","Events");
  histograms_->bookHistogram("eff_pt_trueNrec","seeded TrackingParticle p_{T} [GeV]",
                             directory,pt_nbins,pt_low,pt_high,
                             "p_{T} [GeV]","Events");

  histograms_->bookHistogram("eff_eta_rec","TrajectorySeed #eta",
                             directory,eta_nbins,eta_low,eta_high,
                             "#eta","Events");
  histograms_->bookHistogram("eta_multiplicity","Number of Seeds per TrackingParticle in #eta",
                             directory,eta_nbins,eta_low,eta_high,
                             "#eta","Seeds");
  
  histograms_->bookHistogram("nHitPerTrackingParticleVsEta","Number of hits per tracking particle vs. #eta",
                             directory,eta_nbins,eta_low,eta_high,nhit_nbins,nhit_low,nhit_high,
                             "#eta","n_{Hits}","Events");
  histograms_->bookHistogram("nHitPerLayerVsEta","Number of hits per layer vs. #eta",
                             directory,eta_nbins,eta_low,eta_high,layer_nbins,layer_low,layer_high,
                             "Layer","n_{Hits}","Events");
  histograms_->bookHistogram("tip","transverse impact parameter",
                             directory,tip_nbins,tip_low,tip_high,
                             "tip [cm]","Events");
  histograms_->bookHistogram("lip","longitudinal impact parameter",
                             directory,lip_nbins,lip_low,lip_high,
                             "lip [cm]","Events");
  histograms_->bookHistogram("firstLayerVsEta","First layer vs. #eta",
                             directory,eta_nbins,eta_low,eta_high,layer_nbins,layer_low,layer_high,
                             "Layer","n_{Hits}","Events");
  histograms_->bookHistogram("lastLayerVsEta","Last layer vs. #eta",
                             directory,eta_nbins,eta_low,eta_high,layer_nbins,layer_low,layer_high,
                             "Layer","n_{Hits}","Events");
  
  histograms_->bookHistogram("nHitPerTrackingParticleVsEta_rs","Number of hits per tracking particle vs. #eta",
                             directory,eta_nbins,eta_low,eta_high,nhit_nbins,nhit_low,nhit_high,
                             "#eta","n_{Hits}","Events");
  histograms_->bookHistogram("nHitPerLayerVsEta_rs","Number of hits per layer vs. #eta",
                             directory,eta_nbins,eta_low,eta_high,layer_nbins,layer_low,layer_high,
                             "Layer","n_{Hits}","Events");
  histograms_->bookHistogram("tip_rs","transverse impact parameter",
                             directory,tip_nbins,tip_low,tip_high,
                             "tip [cm]","Events");
  histograms_->bookHistogram("lip_rs","longitudinal impact parameter",
                             directory,lip_nbins,lip_low,lip_high,
                             "lip [cm]","Events");
  histograms_->bookHistogram("firstLayerVsEta_rs","First layer vs. #eta",
                             directory,eta_nbins,eta_low,eta_high,layer_nbins,layer_low,layer_high,
                             "Layer","n_{Hits}","Events");
  histograms_->bookHistogram("lastLayerVsEta_rs","Last layer vs. #eta",
                             directory,eta_nbins,eta_low,eta_high,layer_nbins,layer_low,layer_high,
                             "Layer","n_{Hits}","Events");
  
  
  
}

void 
GutSoftSeedingEfficiencyAnalyzer::endJob() {
  
  std::ostringstream output;
  
  output << std::endl
	 << "Missed BarCodes and percentages per run:event:"
	 << std::endl << std::endl;
  
  bool first = false;
  for ( unsigned int counter = 0;
	counter < missingBarCodes_.size();
	++counter ) {
    first = true;
    output << runs_[counter] 
	   << ":"
	   << events_[counter]
	   << " : ";

    for ( std::vector<int>::iterator missingBarCode = missingBarCodes_[counter].begin(),
	    missingBarCodeEnd = missingBarCodes_[counter].end();
	  missingBarCode != missingBarCodeEnd;
	  ++missingBarCode) {
      if ( first ) {
	output << *missingBarCode;
	first = false;
      } else {
	output << "," << *missingBarCode;
      }
    }
    output << " " << percentages_[counter] 
	   << std::endl;
  }
  
  output << std::endl
	 << "Missed RSBarCodes and percentages per run:event:"
	 << std::endl << std::endl;
  
  first = false;
  for ( unsigned int counter = 0;
	counter < missingRSBarCodes_.size();
	++counter ) {
    first = true;
    output << runsRS_[counter] 
	   << ":"
	   << eventsRS_[counter]
	   << " : ";

    for ( std::vector<int>::iterator missingRSBarCode = missingRSBarCodes_[counter].begin(),
	    missingRSBarCodeEnd = missingRSBarCodes_[counter].end();
	  missingRSBarCode != missingRSBarCodeEnd;
	  ++missingRSBarCode) {
      if ( first ) {
	output << *missingRSBarCode;
	first = false;
      } else {
	output << "," << *missingRSBarCode;
      }
    }
    output << " " << percentagesRS_[counter] 
	   << std::endl;
  }
  
  edm::LogInfo("GutSOftSeedingEfficiencyAnalyzer") << output.str();
}

bool 
GutSoftSeedingEfficiencyAnalyzer::isIncluded(std::vector<const Ring*> &rings, 
                                             DetId id) {
  //
  // check if detid is included in one of the input rings
  //
  
  // return value
  bool result = false;
  
  for ( std::vector<const Ring*>::iterator ring = rings.begin(),
        ringEnd = rings.end();
        ring != ringEnd;
        ++ring ) {
    if ( (*ring)->containsDetId(RoadSearchDetIdHelper::ReturnRPhiId(id)) ) {
      result = true;
      break;
    }
  }
  
  return result;
}

unsigned int
GutSoftSeedingEfficiencyAnalyzer::hitsInRings(std::vector<const Ring*> &rings, 
                                              const TrackingParticle *particle) {
  //
  // counts hits of TrackingParticle contained in rings
  // check that counted particles are on different layers
  //
  
  std::vector<DetId> countedIds;
  
  for ( std::vector<PSimHit>::const_iterator simHit = particle->pSimHit_begin(),
        simHitEnd = particle->pSimHit_end();
        simHit != simHitEnd;
        ++simHit ) {
    
    DetId id(simHit->detUnitId());
    if ( isIncluded(rings,id) ) {
      if ( countedIds.size() == 0 ) {
        countedIds.push_back(id);
      } else {
        bool push = true;
        for ( std::vector<DetId>::iterator countedId = countedIds.begin(),
              countedIdEnd = countedIds.end();
              countedId != countedIdEnd;
              ++countedId ) {
          if ( RoadSearchDetIdHelper::detIdsOnSameLayer(*countedId,id) ) {
            push = false;
          }
        }
        if ( push ) {
          countedIds.push_back(id);
        }
      }
    }
  }
  
  return countedIds.size();
}

std::vector<int>
GutSoftSeedingEfficiencyAnalyzer::GenParticleBarCodeFromRecHit(TrackerHitAssociator &associator,
							       const edm::SimTrackContainer *simTracks,
							       const TrackingRecHit* hit) {
  //
  // return BarCode of GenParticle for RecHit
  //

  std::vector<int> barcodes;
  std::vector< SimHitIdpr> simHitIds = associator.associateHitId(*hit);
  
  for ( std::vector<SimHitIdpr>::iterator simHitId = simHitIds.begin(),
	  simHitIdEnd = simHitIds.end();
	simHitId != simHitIdEnd;
	++simHitId ) {
    for ( edm::SimTrackContainer::const_iterator simTrack = simTracks->begin(),
	    simTrackEnd = simTracks->end();
	  simTrack != simTrackEnd;
	  ++simTrack ) {
      if ( simHitId->first == simTrack->trackId() ) {
	barcodes.push_back(simTrack->genpartIndex());
      }
    }
  }

  return barcodes;
}
