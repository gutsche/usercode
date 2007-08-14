//
// Package:         GutSoftAnalyzers/GutSoftTrackingParticleAnalyzer
// Class:           GutSoftTrackingParticleAnalyzer
// 
// Description:     EDAnalyzer for TrackingParticle information
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Thu Feb 15 21:09:04 UTC 2007
//
// $Author: gutsche $
// $Date: 2007/07/17 21:36:52 $
// $Revision: 1.1 $
//

#include <string>
#include <cmath>
#include <sstream>
#include <algorithm>

#include "GutSoftAnalyzers/GutSoftTrackingParticleAnalyzer/interface/GutSoftTrackingParticleAnalyzer.h"

#include "GutSoftTools/GutSoftHistogramFileService/interface/GutSoftHistogramFileService.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"

#include "SimDataFormats/TrackingHit/interface/PSimHit.h"

#include "TrackingTools/RoadSearchHitAccess/interface/RoadSearchDetIdHelper.h"

#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"

#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit2D.h"

#include "RecoTracker/RingRecord/interface/RingRecord.h"
#include "RecoTracker/RoadMapRecord/interface/RoadMapRecord.h"

#include "DataFormats/DetId/interface/DetId.h"

GutSoftTrackingParticleAnalyzer::GutSoftTrackingParticleAnalyzer(const edm::ParameterSet& iConfig)
{

  // input tags
  trackingTruthInputTag_        = iConfig.getUntrackedParameter<edm::InputTag>("TrackingTruthInputTag");

  // base directory for histograms
  baseDirectoryName_            = iConfig.getUntrackedParameter<std::string>("BaseDirectoryName");

  // GutSoftHistogramFactory
  histograms_                   = edm::Service<GutSoftHistogramFileService>()->getFactory();

  minpt_     = iConfig.getUntrackedParameter<double>("MinPt", 0.);
  maxpt_     = iConfig.getUntrackedParameter<double>("MaxPt", 10000.);
  mineta_    = iConfig.getUntrackedParameter<double>("MinEta", -10.);
  maxeta_    = iConfig.getUntrackedParameter<double>("MaxEta", 10.);
  minphi_    = iConfig.getUntrackedParameter<double>("MinPhi", -3.5);
  maxphi_    = iConfig.getUntrackedParameter<double>("MaxPhi", 3.5);
  absCharge_ = iConfig.getUntrackedParameter<unsigned int>("AbsCharge", 1);
  dumpParticles_ = iConfig.getUntrackedParameter<bool>("DumpParticles", 1);

  // ring label
  ringsLabel_ = iConfig.getParameter<std::string>("RingsLabel");

  // road label
  roadsLabel_ = iConfig.getParameter<std::string>("RoadsLabel");

}


GutSoftTrackingParticleAnalyzer::~GutSoftTrackingParticleAnalyzer()
{
 
}


void
GutSoftTrackingParticleAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  // set baseDirectory in GutSoftHistogramFactory
  histograms_->setBaseDirectory(baseDirectoryName_);

  // get tracking particle collection for true information
  edm::Handle<TrackingParticleCollection>  trackingParticleCollectionHandle;
  iEvent.getByLabel(trackingTruthInputTag_,trackingParticleCollectionHandle);
  const TrackingParticleCollection *trackingParticleCollection = trackingParticleCollectionHandle.product();
    
  unsigned int counter = 0;

  for ( TrackingParticleCollection::const_iterator trackingParticle = trackingParticleCollection->begin(),
	  trackingParticleEnd = trackingParticleCollection->end();
	trackingParticle != trackingParticleEnd;
	++trackingParticle ) {
    
    if ( std::sqrt(trackingParticle->momentum().perp2()) >= minpt_ 
	 && std::sqrt(trackingParticle->momentum().perp2()) <= maxpt_
	 && trackingParticle->momentum().eta() >= mineta_
	 && trackingParticle->momentum().eta() <= maxeta_ 
	 && trackingParticle->momentum().phi() >= minphi_
	 && trackingParticle->momentum().phi() <= maxphi_ 
	 && std::abs(trackingParticle->charge()) == absCharge_ ) {
      ++counter;
      histograms_->fill("eta",trackingParticle->momentum().eta());
      histograms_->fill("tip",std::sqrt(trackingParticle->vertex().perp2()));
    }
  }
  
  if ( dumpParticles_ ) {
    std::string particleDump = dumpTrackingParticles(trackingParticleCollection);
    edm::LogInfo("GutSoftTrackingParticleAnalyzer") << particleDump;
  }
  
  edm::LogInfo("GutSoftTrackingParticleAnalyzer") << "Number of selected TrackingParticles: " << counter;

}

void 
GutSoftTrackingParticleAnalyzer::beginJob(const edm::EventSetup& es)
{

  // get tracker geometry
  edm::ESHandle<TrackerGeometry> trackerHandle;
  es.get<TrackerDigiGeometryRecord>().get(trackerHandle);
  tracker_ = trackerHandle.product();

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
  
  // binning for histograms
  std::string  directory = baseDirectoryName_;

  unsigned int eta_nbins = 60;
  double       eta_low   = -3.;
  double       eta_high  =  3.;

  histograms_->bookHistogram("eta","TrackingParticle #eta",
			     directory,eta_nbins,eta_low,eta_high,
			     "#eta","Events");
  unsigned int tip_nbins = 3000;
  double       tip_low   = 0.;
  double       tip_high  = 3.;

  histograms_->bookHistogram("tip","Transverse impact parameter",
			     directory,tip_nbins,tip_low,tip_high,
			     "ip_{T} [cm]","Events");
}

void 
GutSoftTrackingParticleAnalyzer::endJob() {

}

std::string
GutSoftTrackingParticleAnalyzer::dumpTrackingParticles(const TrackingParticleCollection *trackingParticleCollection) {
  //
  // dump trackingParticles
  //

  // return value
  std::ostringstream result;

  for ( TrackingParticleCollection::const_iterator trackingParticle = trackingParticleCollection->begin(),
	  trackingParticleEnd = trackingParticleCollection->end();
        trackingParticle != trackingParticleEnd;
        ++trackingParticle ) {
    if ( trackingParticle->charge() != 0 ) {
      result << "----------" << std::endl
	     << "TrackingParticle PDG ID: " << trackingParticle->pdgId() << std::endl
	     << "pt: " << std::sqrt(trackingParticle->momentum().perp2()) << " eta: " << trackingParticle->momentum().eta() << " number of hits: " << trackingParticle->trackPSimHit().size() << std::endl;
      TrackingParticle::GenParticleRefVector genParticles = trackingParticle->genParticle();
      int barCode = 0;
      for ( TrackingParticle::GenParticleRefVector::const_iterator hepMCParticle = genParticles.begin(),
	      hepMCParticleEnd = genParticles.end();
	    hepMCParticle != hepMCParticleEnd;
	    ++hepMCParticle ) {
	result << "barcode: " << (*hepMCParticle)->barcode() << std::endl;
	if ( barCode == 0 ) {
	  barCode = (*hepMCParticle)->barcode();
	}
      }

      for ( std::vector<PSimHit>::const_iterator simHit = trackingParticle->pSimHit_begin(),
	      simHitEnd = trackingParticle->pSimHit_end();
	    simHit != simHitEnd;
	    ++simHit ) {
	const Ring *ring = rings_->getRing(RoadSearchDetIdHelper::ReturnRPhiId(DetId(simHit->detUnitId())));
	std::vector<const Ring*>::iterator innerSeedRingIterator = std::find(innerSeedRings_.begin(),innerSeedRings_.end(),ring);
	std::vector<const Ring*>::iterator outerSeedRingIterator = std::find(outerSeedRings_.begin(),outerSeedRings_.end(),ring);

	if ( innerSeedRingIterator != innerSeedRings_.end() ||
	     outerSeedRingIterator != outerSeedRings_.end() ) {
	  
	  GlobalPoint globalPosition = tracker_->idToDet(DetId(simHit->detUnitId()))->surface().toGlobal(simHit->localPosition());
	  
	  if ( ring != 0 ) {
	    result << "Hit DetId: " << simHit->detUnitId() << " ring: " << ring->getindex() << " " << globalPosition.perp() << " " << globalPosition.phi() 
		   << " " << globalPosition.x() << " " << globalPosition.y() << " " << globalPosition.z() << std::endl;
	  } else {
	    result << "Hit DetId: " << simHit->detUnitId() << " ring: NAN " << globalPosition.perp() << " " << globalPosition.phi() 
		   << " " << globalPosition.x() << " " << globalPosition.y() << " " << globalPosition.z() << std::endl;
	  }
	}
      } // loop over sim hits
    }
  }
      
  return result.str();
}
