//
// Package:         GutSoftAnalyzers/GutSoftHepMCAnalyzer
// Class:           GutSoftHepMCAnalyzer
// 
// Description:     EDAnalyzer for HepMC information
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Thu Feb 15 21:09:04 UTC 2007
//
// $Author: gutsche $
// $Date: 2007/04/08 16:47:50 $
// $Revision: 1.4 $
//

#include <string>
#include <cmath>
#include <sstream>
#include <algorithm>

#include "GutSoftAnalyzers/GutSoftHepMCAnalyzer/interface/GutSoftHepMCAnalyzer.h"

#include "GutSoftTools/GutSoftHistogramFileService/interface/GutSoftHistogramFileService.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"

#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"

#include "SimDataFormats/TrackingHit/interface/PSimHit.h"

#include "TrackingTools/RoadSearchHitAccess/interface/RoadSearchDetIdHelper.h"

#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"

#include "RecoTracker/RingRecord/interface/RingRecord.h"
#include "RecoTracker/RoadMapRecord/interface/RoadMapRecord.h"

#include "DataFormats/DetId/interface/DetId.h"

GutSoftHepMCAnalyzer::GutSoftHepMCAnalyzer(const edm::ParameterSet& iConfig)
{

  // input tags
  hepMCInputTag_                = iConfig.getUntrackedParameter<edm::InputTag>("HepMCInputTag");
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
  status_    = iConfig.getUntrackedParameter<int>("Status", 0);
  processID_ = iConfig.getUntrackedParameter<int>("ProcessID", 0);

  // ring label
  ringsLabel_ = iConfig.getParameter<std::string>("RingsLabel");

  // road label
  roadsLabel_ = iConfig.getParameter<std::string>("RoadsLabel");

}


GutSoftHepMCAnalyzer::~GutSoftHepMCAnalyzer()
{
 
}


void
GutSoftHepMCAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  // set baseDirectory in GutSoftHistogramFactory
  histograms_->setBaseDirectory(baseDirectoryName_);

  // get HepMC product
  edm::Handle<edm::HepMCProduct> hepMCProductHandle;
  iEvent.getByLabel(hepMCInputTag_, hepMCProductHandle);
  const HepMC::GenEvent *genEvent = hepMCProductHandle->GetEvent();

  // get tracking particle collection for true information
  edm::Handle<TrackingParticleCollection>  trackingParticleCollectionHandle;
  iEvent.getByLabel(trackingTruthInputTag_,trackingParticleCollectionHandle);
  const TrackingParticleCollection *trackingParticleCollection = trackingParticleCollectionHandle.product();
    
  if(processID_ == 0 || processID_ == genEvent->signal_process_id()) {
    
    for ( HepMC::GenEvent::particle_const_iterator p = genEvent->particles_begin(),
	    pEnd = genEvent->particles_end();
	  p != pEnd; 
	  ++p ) {

      if ( (*p)->momentum().perp() >= minpt_ 
	   && (*p)->momentum().perp() <= maxpt_
	   && (*p)->momentum().eta() >= mineta_
	   && (*p)->momentum().eta() <= maxeta_ 
	   && (*p)->momentum().phi() >= minphi_
	   && (*p)->momentum().phi() <= maxphi_ 
	   && (*p)->status() == status_ ) {
	histograms_->fill("eta",(*p)->momentum().eta());
      }
    }
  }

  std::string particleDump = dumpTrackingParticles(trackingParticleCollection);

  edm::LogInfo("GutSoftHepMCAnalyzer") << particleDump;

}

void 
GutSoftHepMCAnalyzer::beginJob(const edm::EventSetup& es)
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

  histograms_->bookHistogram("eta","HepMC #eta",
			     directory,eta_nbins,eta_low,eta_high,
			     "#eta","Events");
}

void 
GutSoftHepMCAnalyzer::endJob() {

}

std::string
GutSoftHepMCAnalyzer::dumpTrackingParticles(const TrackingParticleCollection *trackingParticleCollection) {
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
      for ( TrackingParticle::GenParticleRefVector::const_iterator hepMCParticle = genParticles.begin(),
	      hepMCParticleEnd = genParticles.end();
	    hepMCParticle != hepMCParticleEnd;
	    ++hepMCParticle ) {
	result << "barcode: " << (*hepMCParticle)->barcode() << std::endl;
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

  } // loop over tracking particles
  
  return result.str();
}
