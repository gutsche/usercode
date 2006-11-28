#ifndef GutSoftRoadSearchCloudAnalyzer_h
#define GutSoftRoadSearchCloudAnalyzer_h

//
// Package:         GutSoftAnalyzers/GutSoftRoadSearchCloudAnalyzer
// Class:           GutSoftRoadSearchCloudAnalyzer
// 
// Description:     EDAnalyzer of RoadSearchClouds
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Tue Oct 17 02:41:12 UTC 2006
//
// $Author: gutsche $
// $Date: 2006/11/27 23:57:13 $
// $Revision: 1.1 $
//

#include <string>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "GutSoftTools/GutSoftHistogramFactory/interface/GutSoftHistogramFactory.h"

class GutSoftRoadSearchCloudAnalyzer : public edm::EDAnalyzer {
 public:
  explicit GutSoftRoadSearchCloudAnalyzer(const edm::ParameterSet&);
  ~GutSoftRoadSearchCloudAnalyzer();

 private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  GutSoftHistogramFactory *histograms_;

  std::string roadSearchCloudProducerLabel_;
  std::string baseDirectoryName_;

};

#endif
