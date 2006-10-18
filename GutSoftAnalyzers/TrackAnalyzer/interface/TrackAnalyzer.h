#ifndef TrackAnalyzer_h
#define TrackAnalyzer_h

//
// Package:         GutSoftAnalyzers/TrackAnalyzer
// Class:           TrackAnalyzer
// 
// Description:     EDAnalyzer for tracks
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Wed Oct 18 01:25:17 UTC 2006
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

class TrackAnalyzer : public edm::EDAnalyzer {
 public:
  explicit TrackAnalyzer(const edm::ParameterSet&);
  ~TrackAnalyzer();

 private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  HistogramFactory *histograms_;

  std::string outputFileName_;
  std::string trackProducerLabel_;
  std::string baseDirectoryName_;

};

#endif
