//
// Package:         GutSoftTools/GutSoftHistogramFileMerger
// Class:           GutSoftHistogramFileMergerModule
// 
// Description:     EDAnalyzer to merge histogram root files
//                  in endjob routine
//                  to be scheduled after histogram root file producers
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Wed Oct 25 20:48:20 UTC 2006
//
// $Author: gutsche $
// $Date: 2006/10/25 22:01:37 $
// $Revision: 1.2 $
//

#include "GutSoftTools/GutSoftHistogramFileMerger/interface/GutSoftHistogramFileMergerModule.h"

#include "GutSoftTools/GutSoftHistogramFileMerger/interface/GutSoftHistogramFileMerger.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

GutSoftHistogramFileMergerModule::GutSoftHistogramFileMergerModule(const edm::ParameterSet& iConfig)
{

  outputFileName_              = iConfig.getUntrackedParameter<std::string>("OutputFileName");
  inputFileNames_              = iConfig.getUntrackedParameter<std::vector<std::string> >("InputFileNames");
  deleteInputFiles_            = iConfig.getUntrackedParameter<bool>("DeleteInputFiles");

}


GutSoftHistogramFileMergerModule::~GutSoftHistogramFileMergerModule()
{

}


void
GutSoftHistogramFileMergerModule::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

}


void 
GutSoftHistogramFileMergerModule::beginJob(const edm::EventSetup&)
{

}

void 
GutSoftHistogramFileMergerModule::endJob() {

  // merge inputFileNames
  GutSoftHistogramFileMerger merger(outputFileName_,inputFileNames_);
  if ( deleteInputFiles_ ) {
    merger.deleteInputFiles();
  }

}
