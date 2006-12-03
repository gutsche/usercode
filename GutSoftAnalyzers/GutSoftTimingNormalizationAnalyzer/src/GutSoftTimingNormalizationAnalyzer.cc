//
// Package:         GutSoftAnalyzers/GutSoftTimingNormalizationAnalyzer
// Class:           GutSoftTimingNormalizationAnalyzer
// 
// Description:     EDAnalyzer with counter for normalization of timing studies
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Sun Dec  3 15:33:58 UTC 2006
//
// $Author: gutsche $
// $Date: 2006/11/28 20:36:41 $
// $Revision: 1.2 $
//

#include <string>

#include "GutSoftAnalyzers/GutSoftTimingNormalizationAnalyzer/interface/GutSoftTimingNormalizationAnalyzer.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

GutSoftTimingNormalizationAnalyzer::GutSoftTimingNormalizationAnalyzer(const edm::ParameterSet& iConfig)
{

  normalizationCounter_ = iConfig.getParameter<unsigned int>("NormalizationCounter");

}


GutSoftTimingNormalizationAnalyzer::~GutSoftTimingNormalizationAnalyzer()
{
 
}


void
GutSoftTimingNormalizationAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  unsigned int counter = 0;
  for ( unsigned int i = 0; i < normalizationCounter_; ++i ) {
    ++counter;
  }

}


void 
GutSoftTimingNormalizationAnalyzer::beginJob(const edm::EventSetup&)
{

}

void 
GutSoftTimingNormalizationAnalyzer::endJob() {

}
