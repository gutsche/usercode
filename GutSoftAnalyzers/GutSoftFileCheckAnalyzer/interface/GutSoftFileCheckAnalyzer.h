#ifndef GutSoftFileCheckAnalyzer_h
#define GutSoftFileCheckAnalyzer_h

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

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class GutSoftFileCheckAnalyzer : public edm::EDAnalyzer {
 public:
  explicit GutSoftFileCheckAnalyzer(const edm::ParameterSet&);
  ~GutSoftFileCheckAnalyzer();

 private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  unsigned int eventCount_;

};

#endif
