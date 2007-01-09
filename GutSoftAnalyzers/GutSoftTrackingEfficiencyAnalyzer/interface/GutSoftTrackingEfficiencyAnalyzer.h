#ifndef GutSoftTrackingEfficiencyAnalyzer_h
#define GutSoftTrackingEfficiencyAnalyzer_h

//
// Package:         GutSoftAnalyzers/GutSoftTrackingEfficiencyAnalyzer
// Class:           GutSoftTrackingEfficiencyAnalyzer
// 
// Description:     EDAnalyzer for tracks
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Wed Oct 18 01:25:17 UTC 2006
//
// $Author: gutsche $
// $Date: 2006/11/28 20:36:41 $
// $Revision: 1.2 $
//

#include <string>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "GutSoftTools/GutSoftHistogramFactory/interface/GutSoftHistogramFactory.h"

#include "FWCore/ParameterSet/interface/InputTag.h"

#include "Validation/RecoTrack/interface/TrackEfficiencySelector.h"
#include "Validation/RecoTrack/interface/TPEfficiencySelector.h"
#include "Validation/RecoTrack/interface/TrackFakeRateSelector.h"

#include "SimTracker/TrackAssociation/interface/TrackAssociatorBase.h"

class GutSoftTrackingEfficiencyAnalyzer : public edm::EDAnalyzer {
 public:
  explicit GutSoftTrackingEfficiencyAnalyzer(const edm::ParameterSet&);
  ~GutSoftTrackingEfficiencyAnalyzer();

 private:
  virtual void beginJob(const edm::EventSetup& setup) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  GutSoftHistogramFactory *histograms_;

  edm::InputTag trackingTruthInputTag_;
  edm::InputTag trackInputTag_;

  TrackEfficiencySelector trackSelectorForEfficiencies_;
  TrackFakeRateSelector   trackSelectorForPurity_;

  TPEfficiencySelector    trackingParticleSelectorForEfficiencies_;
  TPEfficiencySelector    trackingParticleSelectorForPurity_;

  std::string baseDirectoryName_;

  std::string associatorName_;

  TrackAssociatorBase *associator_;

};

#endif
