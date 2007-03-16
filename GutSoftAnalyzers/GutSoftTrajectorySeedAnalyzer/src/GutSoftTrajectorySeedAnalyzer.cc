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
// $Date: 2006/10/25 02:07:31 $
// $Revision: 1.2 $
//

#include <string>

#include "GutSoftAnalyzers/GutSoftTrajectorySeedAnalyzer/interface/GutSoftTrajectorySeedAnalyzer.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/TrajectorySeed/interface/TrajectorySeed.h"
#include "DataFormats/TrajectorySeed/interface/TrajectorySeedCollection.h"

GutSoftTrajectorySeedAnalyzer::GutSoftTrajectorySeedAnalyzer(const edm::ParameterSet& iConfig)
{

  outputFileName_              = iConfig.getUntrackedParameter<std::string>("OutputFileName");
  trajectorySeedProducerLabel_ = iConfig.getUntrackedParameter<std::string>("TrajectorySeedProducerLabel");
  baseDirectoryName_           = iConfig.getUntrackedParameter<std::string>("BaseDirectoryName");

}


GutSoftTrajectorySeedAnalyzer::~GutSoftTrajectorySeedAnalyzer()
{

}


void
GutSoftTrajectorySeedAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

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
	seedCollection = new TrajectorySeedCollection();
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

  // GutSoftHistogramFactory
  histograms_ = new GutSoftHistogramFactory(outputFileName_);

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

  // delete GutSoftHistogramFactory, histogram file is written out and can be handled in module endJob functions of the following modules
  if (histograms_) {
    delete histograms_;
    histograms_ = 0;
  }

}
