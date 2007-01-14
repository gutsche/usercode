#ifndef GutSoftTimingAnalyzer_h
#define GutSoftTimingAnalyzer_h

//
// Package:         GutSoftAnalyzers/GutSoftTimingAnalyzer
// Class:           GutSoftTimingAnalyzer
// 
// Description:     EDAnalyzer for timing informattion stored in collection
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Thu Jan 11 17:31:01 UTC 2007
//
// $Author: gutsche $
// $Date: 2007/01/11 01:51:04 $
// $Revision: 1.1 $
//

#include <string>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "GutSoftTools/GutSoftHistogramFactory/interface/GutSoftHistogramFactory.h"

#include "FWCore/ParameterSet/interface/InputTag.h"

class GutSoftTimingAnalyzer : public edm::EDAnalyzer {
 public:
  explicit GutSoftTimingAnalyzer(const edm::ParameterSet&);
  ~GutSoftTimingAnalyzer();

 private:
  virtual void beginJob(const edm::EventSetup& setup) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  GutSoftHistogramFactory *histograms_;

  edm::InputTag timingInputTag_;

  edm::InputTag normalizationAnalyzerInputTag_;

  edm::InputTag seedInputTag_;
  edm::InputTag cloudInputTag_;
  edm::InputTag trackCandidateInputTag_;
  edm::InputTag trackInputTag_;

  double normalization_;

  unsigned int time_nbins_;
  double       time_low_;
  double       time_high_;

  unsigned int numSeeds_nbins_;
  double       numSeeds_low_;
  double       numSeeds_high_;

  unsigned int numClouds_nbins_;
  double       numClouds_low_;
  double       numClouds_high_;

  unsigned int numTrackCandidates_nbins_;
  double       numTrackCandidates_low_;
  double       numTrackCandidates_high_;

  unsigned int numTracks_nbins_;
  double       numTracks_low_;
  double       numTracks_high_;

  bool first_;

  std::string baseDirectoryName_;

};

#endif
