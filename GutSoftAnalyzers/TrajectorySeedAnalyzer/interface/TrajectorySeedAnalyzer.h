#ifndef TrajectorySeedAnalyzer_h
#define TrajectorySeedAnalyzer_h

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
// $Date: 2006/10/11 03:21:18 $
// $Revision: 1.3 $
//

#include <string>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "GutSoftTools/HistogramFactory/interface/HistogramFactory.h"

class TrajectorySeedAnalyzer : public edm::EDAnalyzer {
 public:
  explicit TrajectorySeedAnalyzer(const edm::ParameterSet&);
  ~TrajectorySeedAnalyzer();

 private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  HistogramFactory *histograms_;

  std::string outputFileName_;
  std::string trajectorySeedProducerLabel_;
  std::string baseDirectoryName_;

};

#endif
