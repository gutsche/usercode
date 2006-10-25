//
// Package:         GutSoftTools/HistogramFileMerger
// Class:           HistogramFileMergerModule
// 
// Description:     EDAnalyzer to merge histogram root files
//                  in endjob routine
//                  to be scheduled after histogram root file producers
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Wed Oct 25 20:48:20 UTC 2006
//
// $Author: gutsche $
// $Date: 2006/10/25 21:09:36 $
// $Revision: 1.1 $
//

#include "GutSoftTools/HistogramFileMerger/interface/HistogramFileMergerModule.h"

#include "GutSoftTools/HistogramFileMerger/interface/HistogramFileMerger.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

HistogramFileMergerModule::HistogramFileMergerModule(const edm::ParameterSet& iConfig)
{

  outputFileName_              = iConfig.getUntrackedParameter<std::string>("OutputFileName");
  inputFileNames_              = iConfig.getUntrackedParameter<std::vector<std::string> >("InputFileNames");
  deleteInputFiles_            = iConfig.getUntrackedParameter<bool>("DeleteInputFiles");

}


HistogramFileMergerModule::~HistogramFileMergerModule()
{

}


void
HistogramFileMergerModule::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

}


void 
HistogramFileMergerModule::beginJob(const edm::EventSetup&)
{

}

void 
HistogramFileMergerModule::endJob() {

  // merge inputFileNames
  HistogramFileMerger merger(outputFileName_,inputFileNames_);
  if ( deleteInputFiles_ ) {
    merger.deleteInputFiles();
  }

}
