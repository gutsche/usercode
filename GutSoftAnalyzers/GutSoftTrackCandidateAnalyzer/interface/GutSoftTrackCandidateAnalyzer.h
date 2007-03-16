#ifndef GutSoftTrackCandidateAnalyzer_h
#define GutSoftTrackCandidateAnalyzer_h

//
// Package:         GutSoftAnalyzers/GutSoftTrackCandidateAnalyzer
// Class:           GutSoftTrackCandidateAnalyzer
// 
// Description:     EDAnalyzer for TrackCandidates
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Wed Oct 18 01:05:12 UTC 2006
//
// $Author: gutsche $
// $Date: 2006/10/18 01:45:49 $
// $Revision: 1.1 $
//

#include <string>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "GutSoftTools/GutSoftHistogramFactory/interface/GutSoftHistogramFactory.h"

class GutSoftTrackCandidateAnalyzer : public edm::EDAnalyzer {
 public:
  explicit GutSoftTrackCandidateAnalyzer(const edm::ParameterSet&);
  ~GutSoftTrackCandidateAnalyzer();

 private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  GutSoftHistogramFactory *histograms_;

  std::string outputFileName_;
  std::string trackCandidateProducerLabel_;
  std::string baseDirectoryName_;

};

#endif
