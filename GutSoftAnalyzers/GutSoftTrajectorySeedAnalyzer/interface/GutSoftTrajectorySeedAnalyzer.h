#ifndef GutSoftTrajectorySeedAnalyzer_h
#define GutSoftTrajectorySeedAnalyzer_h

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
// $Date: 2006/11/28 20:36:42 $
// $Revision: 1.2 $
//

#include <string>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include "GutSoftTools/GutSoftHistogramFactory/interface/GutSoftHistogramFactory.h"

class GutSoftTrajectorySeedAnalyzer : public edm::EDAnalyzer {
 public:
  explicit GutSoftTrajectorySeedAnalyzer(const edm::ParameterSet&);
  ~GutSoftTrajectorySeedAnalyzer();

 private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  GutSoftHistogramFactory *histograms_;

  edm::InputTag trajectorySeedInputTag_;
  std::string   baseDirectoryName_;

};

#endif
