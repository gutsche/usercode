//
// Package:         GutSoftAnalyzers/TrajectorySeedAnalyzer
// Class:           TrajectorySeedAnalyzer
// 
// Description:     EDAnalyzer of TrajectorySeeds
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Tue Oct 17 02:07:51 UTC 2006
//
// $Author: gutsche $
// $Date: 2006/10/17 03:12:34 $
// $Revision: 1.1 $
//

#include <string>

#include "GutSoftAnalyzers/TrajectorySeedAnalyzer/interface/TrajectorySeedAnalyzer.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/TrajectorySeed/interface/TrajectorySeed.h"
#include "DataFormats/TrajectorySeed/interface/TrajectorySeedCollection.h"

TrajectorySeedAnalyzer::TrajectorySeedAnalyzer(const edm::ParameterSet& iConfig)
{

  outputFileName_              = iConfig.getUntrackedParameter<std::string>("OutputFileName");
  trajectorySeedProducerLabel_ = iConfig.getUntrackedParameter<std::string>("TrajectorySeedProducerLabel");
  baseDirectoryName_           = iConfig.getUntrackedParameter<std::string>("BaseDirectoryName");

}


TrajectorySeedAnalyzer::~TrajectorySeedAnalyzer()
{

  if ( histograms_ ) {
    delete histograms_;
    histograms_ = 0;
  }
 
}


void
TrajectorySeedAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
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
	edm::LogWarning("TrajectorySeedAnalyzer") << "Collection TrajectorySeedCollection with label " << trajectorySeedProducerLabel_ << " cannot be found, using empty collection of same type";
      }
    }
  }

  // fill histograms
  histograms_->fill("nSeed",seedCollection->size());

}


void 
TrajectorySeedAnalyzer::beginJob(const edm::EventSetup&)
{

  // HistogramFactory
  histograms_ = new HistogramFactory(outputFileName_);

  // binning for histograms
  unsigned int nSeed_nbins    = 100000;
  unsigned int nSeed_low      = 0;
  unsigned int nSeed_high     = 100000;
  std::string  nSeedDirectory = baseDirectoryName_;

  // book histogram
  histograms_->bookHistogram("nSeed","Number of Seed per event",nSeedDirectory,nSeed_nbins,nSeed_low,nSeed_high,"n_{Seed}","Events");

}

void 
TrajectorySeedAnalyzer::endJob() {

  // delete HistogramFactory, histogram file is written out and can be handled in module endJob functions of the following modules
  if (histograms_) {
    delete histograms_;
    histograms_ = 0;
  }

}
