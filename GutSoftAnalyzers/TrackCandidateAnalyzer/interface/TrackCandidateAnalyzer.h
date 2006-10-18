#ifndef TrackCandidateAnalyzer_h
#define TrackCandidateAnalyzer_h

//
// Package:         GutSoftAnalyzers/TrackCandidateAnalyzer
// Class:           TrackCandidateAnalyzer
// 
// Description:     EDAnalyzer for TrackCandidates
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Wed Oct 18 01:05:12 UTC 2006
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

class TrackCandidateAnalyzer : public edm::EDAnalyzer {
 public:
  explicit TrackCandidateAnalyzer(const edm::ParameterSet&);
  ~TrackCandidateAnalyzer();

 private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  HistogramFactory *histograms_;

  std::string outputFileName_;
  std::string trackCandidateProducerLabel_;
  std::string baseDirectoryName_;

};

#endif
