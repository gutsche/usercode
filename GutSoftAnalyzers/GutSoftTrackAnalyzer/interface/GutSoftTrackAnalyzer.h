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
// $Date: 2007/02/27 08:35:44 $
// $Revision: 1.4 $
//

#include <string>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include "GutSoftTools/GutSoftHistogramFactory/interface/GutSoftHistogramFactory.h"

#include "Validation/RecoTrack/interface/TrackEfficiencySelector.h"

class GutSoftTrackAnalyzer : public edm::EDAnalyzer {
 public:
  explicit GutSoftTrackAnalyzer(const edm::ParameterSet&);
  ~GutSoftTrackAnalyzer();

 private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  GutSoftHistogramFactory *histograms_;

  edm::InputTag trackInputTag_;
  std::string   baseDirectoryName_;

  double minimalAbsoluteEta_;
  double maximalAbsoluteEta_;

  TrackEfficiencySelector trackSelector_;

};

#endif
