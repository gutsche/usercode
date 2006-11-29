//
// Package:         GutSoftAnalyzers/GutSoftTrajectorySeedAnalyzer
// Class:           GutSoftTrajectorySeedAnalyzer
// 
// Description:     EDAnalyzer of TrajectorySeeds
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Tue Oct 17 02:07:51 UTC 2006
//
// $Author: gutsche $
// $Date: 2006/11/28 20:36:43 $
// $Revision: 1.2 $
//

#include <string>

#include "GutSoftAnalyzers/GutSoftTrajectorySeedAnalyzer/interface/GutSoftTrajectorySeedAnalyzer.h"

#include "GutSoftTools/GutSoftHistogramFileService/interface/GutSoftHistogramFileService.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/TrajectorySeed/interface/TrajectorySeed.h"
#include "DataFormats/TrajectorySeed/interface/TrajectorySeedCollection.h"

GutSoftTrajectorySeedAnalyzer::GutSoftTrajectorySeedAnalyzer(const edm::ParameterSet& iConfig)
{

  trajectorySeedProducerLabel_ = iConfig.getUntrackedParameter<std::string>("TrajectorySeedProducerLabel");
  baseDirectoryName_           = iConfig.getUntrackedParameter<std::string>("BaseDirectoryName");

  // GutSoftHistogramFactory
  histograms_ = edm::Service<GutSoftHistogramFileService>()->getFactory();

}


GutSoftTrajectorySeedAnalyzer::~GutSoftTrajectorySeedAnalyzer()
{

}


void
GutSoftTrajectorySeedAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  // set baseDirectory in GutSoftHistogramFactory
  histograms_->setBaseDirectory(baseDirectoryName_);

  // get collection
  const TrajectorySeedCollection *seedCollection = 0;
  try {
    edm::Handle<TrajectorySeedCollection> seedCollectionHandle;
    iEvent.getByLabel(trajectorySeedProducerLabel_,seedCollectionHandle);
    seedCollection = seedCollectionHandle.product();
  }
  catch (edm::Exception const& x) {
    if ( x.categoryCode() == edm::errors::ProductNotFound ) {
      if ( x.history().size() == 1 ) {
	static const TrajectorySeedCollection s_empty;
	seedCollection = &s_empty;
	edm::LogWarning("GutSoftTrajectorySeedAnalyzer") << "Collection TrajectorySeedCollection with label " << trajectorySeedProducerLabel_ << " cannot be found, using empty collection of same type";
      }
    }
  }

  // fill histograms
  histograms_->fill("nSeed",seedCollection->size());

}


void 
GutSoftTrajectorySeedAnalyzer::beginJob(const edm::EventSetup&)
{

  // binning for histograms
  unsigned int nSeed_nbins    = 100000;
  unsigned int nSeed_low      = 0;
  unsigned int nSeed_high     = 100000;
  std::string  nSeedDirectory = baseDirectoryName_;

  // book histogram
  histograms_->bookHistogram("nSeed","Number of Seed per event",nSeedDirectory,nSeed_nbins,nSeed_low,nSeed_high,"n_{Seed}","Events");

}

void 
GutSoftTrajectorySeedAnalyzer::endJob() {

}
