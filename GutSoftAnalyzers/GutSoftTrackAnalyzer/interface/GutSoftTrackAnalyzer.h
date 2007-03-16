#ifndef GutSoftTrackAnalyzer_h
#define GutSoftTrackAnalyzer_h

//
// Package:         GutSoftAnalyzers/GutSoftTrackAnalyzer
// Class:           GutSoftTrackAnalyzer
// 
// Description:     EDAnalyzer for tracks
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Wed Oct 18 01:25:17 UTC 2006
//
// $Author: gutsche $
// $Date: 2006/10/18 01:45:48 $
// $Revision: 1.1 $
//

#include <string>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "GutSoftTools/GutSoftHistogramFactory/interface/GutSoftHistogramFactory.h"

class GutSoftTrackAnalyzer : public edm::EDAnalyzer {
 public:
  explicit GutSoftTrackAnalyzer(const edm::ParameterSet&);
  ~GutSoftTrackAnalyzer();

 private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  GutSoftHistogramFactory *histograms_;

  std::string outputFileName_;
  std::string trackProducerLabel_;
  std::string baseDirectoryName_;

};

#endif
