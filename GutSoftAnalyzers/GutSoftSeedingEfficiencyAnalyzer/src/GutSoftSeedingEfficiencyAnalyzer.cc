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
// $Date: 2007/01/22 01:35:10 $
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

GutSoftSeedingEfficiencyAnalyzer::GutSoftSeedingEfficiencyAnalyzer(const edm::ParameterSet& conf) : 
  conf_(conf),
  trackingParticleSelector_(conf.getParameter<edm::ParameterSet>("TrackingParticleCuts"))
{

  trajectorySeedInputTag_      = conf_.getUntrackedParameter<edm::InputTag>("TrajectorySeedInputTag");
  trackingTruthInputTag_       = conf_.getUntrackedParameter<edm::InputTag>("TrackingTruthInputTag");
  matchedStripRecHitsInputTag_ = conf_.getUntrackedParameter<edm::InputTag>("matchedStripRecHits");
  rphiStripRecHitsInputTag_    = conf_.getUntrackedParameter<edm::InputTag>("rphiStripRecHits");
  stereoStripRecHitsInputTag_  = conf_.getUntrackedParameter<edm::InputTag>("stereoStripRecHits");
  pixelRecHitsInputTag_        = conf_.getUntrackedParameter<edm::InputTag>("pixelRecHits");

  baseDirectoryName_      = conf_.getUntrackedParameter<std::string>("BaseDirectoryName");

  trackingParticleMinimumPt_            = conf_.getUntrackedParameter<double>("TrackinParticleMinimumPt");
  trackingParticleHitsInInnerSeedRings_ = conf_.getUntrackedParameter<unsigned int>("TrackingParticleHitsInInnerSeedRings");
  trackingParticleHitsInOuterSeedRings_ = conf_.getUntrackedParameter<unsigned int>("TrackingParticleHitsInOuterSeedRings");
  trackingParticleHitsInSeedRings_      = conf_.getUntrackedParameter<unsigned int>("TrackingParticleHitsInSeedRings");

  // hit access
  std::string tmp             = conf.getUntrackedParameter<std::string>("InnerSeedRecHitAccessMode");
  if ( tmp == "STANDARD" ) {
    innerSeedHitAccessMode_ = DetHitAccess::standard;
  } else if ( tmp == "RPHI" ) {
    innerSeedHitAccessMode_ = DetHitAccess::rphi;
  } else {
    innerSeedHitAccessMode_ = DetHitAccess::standard;
  }
  innerSeedHitAccessUseRPhi_  = conf.getUntrackedParameter<bool>("InnerSeedRecHitAccessUseRPhi");
  innerSeedHitAccessUseStereo_  = conf.getUntrackedParameter<bool>("InnerSeedRecHitAccessUseStereo");

  tmp                         = conf.getUntrackedParameter<std::string>("OuterSeedRecHitAccessMode");
  if ( tmp == "STANDARD" ) {
    outerSeedHitAccessMode_ = DetHitAccess::standard;
  } else if ( tmp == "RPHI" ) {
    outerSeedHitAccessMode_ = DetHitAccess::rphi;
  } else {
    outerSeedHitAccessMode_ = DetHitAccess::standard;
  }
  outerSeedHitAccessUseRPhi_  = conf.getUntrackedParameter<bool>("OuterSeedRecHitAccessUseRPhi");
  outerSeedHitAccessUseStereo_  = conf.getUntrackedParameter<bool>("OuterSeedRecHitAccessUseStereo");

  // configure DetHitAccess
  innerSeedHitVector_.setMode(innerSeedHitAccessMode_);
  innerSeedHitVector_.use_rphiRecHits(innerSeedHitAccessUseRPhi_);
  innerSeedHitVector_.use_stereoRecHits(innerSeedHitAccessUseStereo_);
  outerSeedHitVector_.setMode(outerSeedHitAccessMode_);
  outerSeedHitVector_.use_rphiRecHits(outerSeedHitAccessUseRPhi_);
  outerSeedHitVector_.use_stereoRecHits(outerSeedHitAccessUseStereo_);

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

  // rechit collections
  // get Inputs
  edm::Handle<SiStripMatchedRecHit2DCollection> matchedRecHitHandle;
  iEvent.getByLabel(matchedStripRecHitsInputTag_ ,matchedRecHitHandle);
  const SiStripMatchedRecHit2DCollection *matchedRecHitCollection = matchedRecHitHandle.product();
  edm::Handle<SiStripRecHit2DCollection> rphiRecHitHandle;
  iEvent.getByLabel(rphiStripRecHitsInputTag_ ,rphiRecHitHandle);
  const SiStripRecHit2DCollection *rphiRecHitCollection = rphiRecHitHandle.product();
  edm::Handle<SiStripRecHit2DCollection> stereoRecHitHandle;
  iEvent.getByLabel(stereoStripRecHitsInputTag_ ,stereoRecHitHandle);
  const SiStripRecHit2DCollection *stereoRecHitCollection = stereoRecHitHandle.product();
 
  // special treatment for getting pixel collection
  // if collection exists in file, use collection from file
  // if collection does not exist in file, create empty collection
  const SiPixelRecHitCollection *pixelRecHitCollection = 0;
  try {
    edm::Handle<SiPixelRecHitCollection> pixelRecHitHandle;
    iEvent.getByLabel(pixelRecHitsInputTag_, pixelRecHitHandle);
    pixelRecHitCollection = pixelRecHitHandle.product();
  }
  catch (edm::Exception const& x) {
    if ( x.categoryCode() == edm::errors::ProductNotFound ) {
      if ( x.history().size() == 1 ) {
	static const SiPixelRecHitCollection s_empty;
	pixelRecHitCollection = &s_empty;
	edm::LogWarning("GutSoftSeedingEfficiencyAnalyzer") << "Collection SiPixelRecHitCollection with InputTag " << pixelRecHitsInputTag_ << " cannot be found, using empty collection of same type.";
      }
    }
  }
  
  // initialize general hit access for road search
  innerSeedHitVector_.setCollections(rphiRecHitCollection,
				     stereoRecHitCollection,
				     matchedRecHitCollection,
				     pixelRecHitCollection);
  outerSeedHitVector_.setCollections(rphiRecHitCollection,
				     stereoRecHitCollection,
				     matchedRecHitCollection,
				     pixelRecHitCollection);

  // get trackinghit associator
  TrackerHitAssociator associator(iEvent, conf_);

  // create map of all true hits to all rechits in all seed rings
  SimHitMap simHitMap = CreateMapTrueHitsToRecHits(associator);

  // fill histograms
  for ( TrackingParticleCollection::const_iterator trackingParticle = trackingParticleCollection->begin(),
	  trackingParticleEnd = trackingParticleCollection->end();
	trackingParticle != trackingParticleEnd;
	++trackingParticle ) {

    // apply tracking particle cuts
    if ( trackingParticleSelector_( *trackingParticle ) ) {
      // enumerator
      // check which tracking particles could be seeded by RS seeding
      //
      unsigned int hitsInInnerRings = hitsInRings(innerSeedRings_,&(*trackingParticle));
      unsigned int hitsInOuterRings = hitsInRings(outerSeedRings_,&(*trackingParticle));
      if ( std::sqrt(trackingParticle->momentum().perp2()) >= trackingParticleMinimumPt_ ) {
	histograms_->fill("eff_eta_true",trackingParticle->momentum().eta());
	if ( hitsInInnerRings >= trackingParticleHitsInInnerSeedRings_ ) {
	  if ( hitsInOuterRings >= trackingParticleHitsInOuterSeedRings_ ) {
	    if ( (hitsInInnerRings+hitsInOuterRings) >= trackingParticleHitsInSeedRings_ ) {
	      histograms_->fill("eff_eta_rs_true",trackingParticle->momentum().eta());
	      // denominator
	      // loop over tracking particles
	      // create list of reco hits corresponding to simhits
	      // loop over seeds
	      // check whether all seed hits are in list of reco hits of tracking particle
	      // fill trueNrec

	      std::vector<TrackingRecHit*> recHits = AllRecHitsOfTrackingParticle(simHitMap,&(*trackingParticle));

	      // how many seeds are seeding this tracking particle
	      unsigned int seeded = 0;
	      for ( TrajectorySeedCollection::const_iterator seed = seedCollection->begin(),
		      seedEnd = seedCollection->end();
		    seed != seedEnd;
		    ++seed ) {
		bool seeds = true;
		for ( edm::OwnVector<TrackingRecHit>::const_iterator seedHit = seed->recHits().first,
			seedHitEnd = seed->recHits().second;
		      seedHit != seedHitEnd;
		      ++seedHit ) {
		  bool included = false;
		  for ( std::vector<TrackingRecHit*>::iterator hit = recHits.begin(),
			  hitEnd = recHits.end();
			hit != hitEnd;
			++hit ) {
		    // compare two hits
		    if ( seedHit->geographicalId() == (*hit)->geographicalId() ) {
		      if ( std::abs(seedHit->localPosition().x()-(*hit)->localPosition().x()) < 1E9 ) {
			if ( std::abs(seedHit->localPosition().y()-(*hit)->localPosition().y()) < 1E9 ) {
			  included = true;
			}
		      }
		    }
		  }
		  if ( !included ) {
		    seeds = false;
		  }
		}
		if (seeds = true ) {
		  ++seeded;
		}
	      }
	      if ( seeded > 0 ) {
		histograms_->fill("eff_eta_trueNrec",trackingParticle->momentum().eta());
		for ( unsigned int i = 0;
		      i < seeded;
		      ++i ) {
		  histograms_->fill("eta_multiplicity",trackingParticle->momentum().eta());
		}
	      }
	    }
	  }
	}
      }
    }
  }

  for ( TrajectorySeedCollection::const_iterator seed = seedCollection->begin(),
	  seedEnd = seedCollection->end();
	seed != seedEnd;
	++seed ) {
    // fill eta of outermost hit
    const TrackingRecHit *hit = &(*(seed->recHits().second-1));
    GlobalPoint globalPosition = tracker_->idToDet(hit->geographicalId())->surface().toGlobal(hit->localPosition());
    histograms_->fill("eff_eta_rec",globalPosition.eta());
  }

}


void 
GutSoftSeedingEfficiencyAnalyzer::beginJob(const edm::EventSetup &es)
{


  // get tracker geometry
  edm::ESHandle<TrackerGeometry> tracker;
  es.get<TrackerDigiGeometryRecord>().get(tracker);
  tracker_ = tracker.product();

  // get roads
  edm::ESHandle<Roads> roads;
  es.get<RoadMapRecord>().get(roads);
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
  unsigned int eta_nbins = 30;
  double       eta_low   = -3.;
  double       eta_high  =  3.;

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
  histograms_->bookHistogram("eff_eta_rec","TrajectorySeed #eta",
			     directory,eta_nbins,eta_low,eta_high,
			     "#eta","Events");
  histograms_->bookHistogram("eta_multiplicity","Number of Seeds per TrackingParticle in #eta",
			     directory,eta_nbins,eta_low,eta_high,
			     "#eta","Seeds");

}

void 
GutSoftSeedingEfficiencyAnalyzer::endJob() {

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
    
std::vector<TrackingRecHit*>
GutSoftSeedingEfficiencyAnalyzer::AllRecHitsOfTrackingParticle(SimHitMap &simHitMap,
							       const TrackingParticle *trackingParticle)
{
  // 
  // return all TrackingRecHit which are associated to TrackingParticle
  //

  // return value
  std::vector<TrackingRecHit*> result;

  for ( std::vector<PSimHit>::const_iterator simHit = trackingParticle->pSimHit_begin(),
	  simHitEnd = trackingParticle->pSimHit_end();
	simHit != simHitEnd;
	++simHit ) {
    if ( simHitMap.count(*simHit) > 0 ) {
      std::vector<TrackingRecHit*> temp = simHitMap[*simHit];
      result.insert(result.end(),temp.begin(),temp.end());
    }
  }

  return result;
  
}

GutSoftSeedingEfficiencyAnalyzer::SimHitMap 
GutSoftSeedingEfficiencyAnalyzer::CreateMapTrueHitsToRecHits(TrackerHitAssociator &associator) 
{
  //
  // create map between all rechits in seed rings to true simhits
  //

  // return value
  SimHitMap result;

  // loop over inner seed rings
  for ( std::vector<const Ring*>::iterator ring = innerSeedRings_.begin(),
	  ringEnd = innerSeedRings_.end();
	ring != ringEnd;
	++ring ) {
    // loop over detid's in ring
    for ( Ring::const_iterator ringDetIdIterator = (*ring)->begin(),
	    ringDetIdIteratorEnd = (*ring)->end(); 
	  ringDetIdIterator != ringDetIdIteratorEnd; 
	  ++ringDetIdIterator ) {
      
      std::vector<TrackingRecHit*> ringRecHits = innerSeedHitVector_.getHitVector(&(ringDetIdIterator->second));
      
      // loop over inner rechits
      for (std::vector<TrackingRecHit*>::const_iterator ringRecHit = ringRecHits.begin();
	   ringRecHit != ringRecHits.end(); 
	   ++ringRecHit) {
	// associate recHit to simHits
	std::vector<PSimHit> simHits = associator.associateHit(**ringRecHit);

	// loop over simHits and fill map
	for ( std::vector<PSimHit>::iterator simHit = simHits.begin(),
		simHitEnd = simHits.end();
	      simHit != simHitEnd;
	      ++simHit ) {
	  if ( result.count(*simHit) == 0 ) {
	    std::vector<TrackingRecHit*> temp;
	    temp.push_back(*ringRecHit);
	    result[*simHit] = temp;
	  } else {
	    result[*simHit].push_back(*ringRecHit);
	  }
	}
      }
    }
  }

  // loop over outer seed rings
  for ( std::vector<const Ring*>::iterator ring = outerSeedRings_.begin(),
	  ringEnd = outerSeedRings_.end();
	ring != ringEnd;
	++ring ) {
    // loop over detid's in ring
    for ( Ring::const_iterator ringDetIdIterator = (*ring)->begin(),
	    ringDetIdIteratorEnd = (*ring)->end(); 
	  ringDetIdIterator != ringDetIdIteratorEnd; 
	  ++ringDetIdIterator ) {
      
      std::vector<TrackingRecHit*> ringRecHits = outerSeedHitVector_.getHitVector(&(ringDetIdIterator->second));
      
      // loop over outer rechits
      for (std::vector<TrackingRecHit*>::const_iterator ringRecHit = ringRecHits.begin();
	   ringRecHit != ringRecHits.end(); 
	   ++ringRecHit) {
	// associate recHit to simHits
	std::vector<PSimHit> simHits = associator.associateHit(**ringRecHit);

	// loop over simHits and fill map
	for ( std::vector<PSimHit>::iterator simHit = simHits.begin(),
		simHitEnd = simHits.end();
	      simHit != simHitEnd;
	      ++simHit ) {
	  if ( result.count(*simHit) == 0 ) {
	    std::vector<TrackingRecHit*> temp;
	    temp.push_back(*ringRecHit);
	    result[*simHit] = temp;
	  } else {
	    result[*simHit].push_back(*ringRecHit);
	  }
	}
      }
    }
  }

  return result;
}
