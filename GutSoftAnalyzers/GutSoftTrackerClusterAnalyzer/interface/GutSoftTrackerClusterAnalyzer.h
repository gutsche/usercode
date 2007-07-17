#ifndef GutSoftTrackerClusterAnalyzer_h
#define GutSoftTrackerClusterAnalyzer_h

//
// Package:         GutSoftAnalyzers/GutSoftTrackerClusterAnalyzer
// Class:           GutSoftTrackerClusterAnalyzer
// 
// Description:     EDAnalyzer for TrackerCluster information
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Thu Feb 15 21:09:04 UTC 2007
//
// $Author: gutsche $
// $Date: 2007/04/08 16:47:49 $
// $Revision: 1.3 $
//

#include <string>
#include <vector>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include "GutSoftTools/GutSoftHistogramFactory/interface/GutSoftHistogramFactory.h"

class GutSoftTrackerClusterAnalyzer : public edm::EDAnalyzer {
 public:
  explicit GutSoftTrackerClusterAnalyzer(const edm::ParameterSet&);
  ~GutSoftTrackerClusterAnalyzer();

 private:
  virtual void beginJob(const edm::EventSetup& setup) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  GutSoftHistogramFactory *histograms_;

  edm::InputTag pixelClusterInputTag_;
  edm::InputTag stripClusterInputTag_;

  std::string baseDirectoryName_;

};

#endif
