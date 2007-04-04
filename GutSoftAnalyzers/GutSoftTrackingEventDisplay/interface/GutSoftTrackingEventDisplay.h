#ifndef GutSoftTrackingEventDisplay_h
#define GutSoftTrackingEventDisplay_h

//
// Package:         GutSoftAnalyzers/GutSoftTrackingEventDisplay
// Class:           GutSoftTrackingEventDisplay
// 
// Description:     event display for tracking
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Tue Apr  3 21:47:43 UTC 2007
//
// $Author: gutsche $
// $Date: 2007/03/28 13:34:17 $
// $Revision: 1.2 $
//

#include <string>
#include <vector>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include "RecoTracker/RingRecord/interface/Rings.h"
#include "RecoTracker/RoadMapRecord/interface/Roads.h"

#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"

#include "TCanvas.h"
#include "TH2D.h"
#include "TGaxis.h"
#include "TPaveLabel.h"
#include "TPad.h"
#include "TLine.h"

class GutSoftTrackingEventDisplay : public edm::EDAnalyzer {
 public:
  explicit GutSoftTrackingEventDisplay(const edm::ParameterSet&);
  ~GutSoftTrackingEventDisplay();

 private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  std::string  outputFileNameBegin_;
  std::string  outputFileName_;
  std::string  outputFileNameEnd_;

  bool         labelRings_;

  const Rings* rings_;
  std::string  ringLabel_;
  const Roads* roads_;
  std::string  roadLabel_;

  TCanvas *canvas_;
  TPad    *xy_pad_;
  TPad    *xy_pad_left_;
  TPad    *xy_pad_right_;
  TPad    *rz_pad_;
  TH2D    *rz_histo_;
  TGaxis  *rz_histo_axis_right_;
  TGaxis  *rz_histo_axis_top_;
  TH2D    *xy_histo_;
  TGaxis  *xy_histo_axis_right_;
  TGaxis  *xy_histo_axis_top_;
  TLine   *xy_histo_x_line_;
  TLine   *xy_histo_y_line_;

  std::vector<TPaveLabel*> allRings_;
  std::vector<TPaveLabel*> innerSeedRings_;
  std::vector<TPaveLabel*> outerSeedRings_;

  const TrackerGeometry *tracker_;

  edm::InputTag trackingTruthInputTag_;
  edm::InputTag trajectorySeedInputTag_;


};

#endif
