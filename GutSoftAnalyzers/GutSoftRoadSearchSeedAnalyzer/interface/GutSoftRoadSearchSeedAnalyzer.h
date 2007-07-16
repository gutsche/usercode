#ifndef GutSoftRoadSearchSeedAnalyzer_h
#define GutSoftRoadSearchSeedAnalyzer_h

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
// $Date: 2007/01/22 01:35:09 $
// $Revision: 1.3 $
//

#include <string>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include "GutSoftTools/GutSoftHistogramFactory/interface/GutSoftHistogramFactory.h"

class GutSoftRoadSearchSeedAnalyzer : public edm::EDAnalyzer {
 public:
  explicit GutSoftRoadSearchSeedAnalyzer(const edm::ParameterSet&);
  ~GutSoftRoadSearchSeedAnalyzer();

 private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  GutSoftHistogramFactory *histograms_;

  edm::InputTag trajectorySeedInputTag_;
  std::string   baseDirectoryName_;

};

#endif
