//
// Package:         GutSoftAnalyzers/GutSoftFileCheckAnalyzer
// Class:           GutSoftFileCheckAnalyzer
// 
// Description:     EDAnalyzer for tracks
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Wed Oct 18 01:25:17 UTC 2006
//
// $Author: gutsche $
// $Date: 2007/01/10 21:46:07 $
// $Revision: 1.1 $
//

#include <string>

#include "GutSoftAnalyzers/GutSoftFileCheckAnalyzer/interface/GutSoftFileCheckAnalyzer.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

GutSoftFileCheckAnalyzer::GutSoftFileCheckAnalyzer(const edm::ParameterSet& iConfig)
{

  eventCount_ = 0;

}


GutSoftFileCheckAnalyzer::~GutSoftFileCheckAnalyzer()
{
 
}


void
GutSoftFileCheckAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  ++eventCount_;

}


void 
GutSoftFileCheckAnalyzer::beginJob(const edm::EventSetup&)
{

}

void 
GutSoftFileCheckAnalyzer::endJob() {

  edm::LogInfo("GutSoftFileCheckAnalyzer") << "Number of Events: " << eventCount_;

}
