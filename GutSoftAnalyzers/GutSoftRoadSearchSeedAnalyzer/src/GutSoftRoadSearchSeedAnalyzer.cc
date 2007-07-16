//
// Package:         GutSoftAnalyzers/GutSoftRoadSearchSeedAnalyzer
// Class:           GutSoftRoadSearchSeedAnalyzer
// 
// Description:     EDAnalyzer of RoadSearchSeeds
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Tue Oct 17 02:07:51 UTC 2006
//
// $Author: gutsche $
// $Date: 2007/06/16 07:37:42 $
// $Revision: 1.5 $
//

#include <string>

#include "GutSoftAnalyzers/GutSoftRoadSearchSeedAnalyzer/interface/GutSoftRoadSearchSeedAnalyzer.h"

#include "GutSoftTools/GutSoftHistogramFileService/interface/GutSoftHistogramFileService.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/RoadSearchSeed/interface/RoadSearchSeed.h"
#include "DataFormats/RoadSearchSeed/interface/RoadSearchSeedCollection.h"

#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"

GutSoftRoadSearchSeedAnalyzer::GutSoftRoadSearchSeedAnalyzer(const edm::ParameterSet& iConfig)
{

  trajectorySeedInputTag_ = iConfig.getUntrackedParameter<edm::InputTag>("RoadSearchSeedInputTag");
  baseDirectoryName_      = iConfig.getUntrackedParameter<std::string>("BaseDirectoryName");

  // GutSoftHistogramFactory
  histograms_ = edm::Service<GutSoftHistogramFileService>()->getFactory();

}


GutSoftRoadSearchSeedAnalyzer::~GutSoftRoadSearchSeedAnalyzer()
{

}


void
GutSoftRoadSearchSeedAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  // set baseDirectory in GutSoftHistogramFactory
  histograms_->setBaseDirectory(baseDirectoryName_);

  // get collection
  const RoadSearchSeedCollection *seedCollection = 0;
  try {
    edm::Handle<RoadSearchSeedCollection> seedCollectionHandle;
    iEvent.getByLabel(trajectorySeedInputTag_,seedCollectionHandle);
    seedCollection = seedCollectionHandle.product();
  }
  catch (edm::Exception const& x) {
    if ( x.categoryCode() == edm::errors::ProductNotFound ) {
      if ( x.history().size() == 1 ) {
	static const RoadSearchSeedCollection s_empty;
	seedCollection = &s_empty;
	edm::LogWarning("GutSoftRoadSearchSeedAnalyzer") << "Collection RoadSearchSeedCollection with label " << trajectorySeedInputTag_ << " cannot be found, using empty collection of same type";
      }
    }
  }

  // get tracker geometry
  edm::ESHandle<TrackerGeometry> trackerHandle;
  iSetup.get<TrackerDigiGeometryRecord>().get(trackerHandle);
  const TrackerGeometry *tracker = trackerHandle.product();
  
  // fill histograms
  histograms_->fill("nSeed",seedCollection->size());
  unsigned int counter_A = 0;
  unsigned int counter_B = 0;
  unsigned int counter_C = 0;

  for ( RoadSearchSeedCollection::const_iterator seed = seedCollection->begin(), 
	  seedsEnd = seedCollection->end();
	seed != seedsEnd; 
	++seed ) {
    double eta = std::abs((tracker->idToDet((*seed->begin())->geographicalId())->surface().toGlobal((*seed->begin())->localPosition())).eta());
    if ( eta <= 1.1 ) {
      ++counter_A;
    } else if ( eta > 1.1 && eta <= 1.6 ) {
      ++counter_B;
    } else if ( eta > 1.6 ) {
      ++counter_C;
    }
  }
  histograms_->fill("nSeed_A",counter_A);
  histograms_->fill("nSeed_B",counter_B);
  histograms_->fill("nSeed_C",counter_C);

}


void 
GutSoftRoadSearchSeedAnalyzer::beginJob(const edm::EventSetup&)
{

  // binning for histograms
  unsigned int nSeed_nbins    = 100000;
  unsigned int nSeed_low      = 0;
  unsigned int nSeed_high     = 100000;
  std::string  nSeedDirectory = baseDirectoryName_;

  // book histogram
  histograms_->bookHistogram("nSeed","Number of Seed per event",nSeedDirectory,nSeed_nbins,nSeed_low,nSeed_high,"n_{Seed}","Events");
  histograms_->bookHistogram("nSeed_A","Number of Seed per event for |#eta| #leq 1.1",nSeedDirectory,nSeed_nbins,nSeed_low,nSeed_high,"n_{Seed}","Events");
  histograms_->bookHistogram("nSeed_B","Number of Seed per event for 1.1 < |#eta| #leq 1.6",nSeedDirectory,nSeed_nbins,nSeed_low,nSeed_high,"n_{Seed}","Events");
  histograms_->bookHistogram("nSeed_C","Number of Seed per event for |#eta| > 1.6",nSeedDirectory,nSeed_nbins,nSeed_low,nSeed_high,"n_{Seed}","Events");

}

void 
GutSoftRoadSearchSeedAnalyzer::endJob() {

}
