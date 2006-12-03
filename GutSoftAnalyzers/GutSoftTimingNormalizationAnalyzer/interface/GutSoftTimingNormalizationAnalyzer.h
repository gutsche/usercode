#ifndef GutSoftTimingNormalizationAnalyzer_h
#define GutSoftTimingNormalizationAnalyzer_h

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

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class GutSoftTimingNormalizationAnalyzer : public edm::EDAnalyzer {
 public:
  explicit GutSoftTimingNormalizationAnalyzer(const edm::ParameterSet&);
  ~GutSoftTimingNormalizationAnalyzer();

 private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  unsigned int normalizationCounter_;

};

#endif
