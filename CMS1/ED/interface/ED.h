#ifndef ED_h
#define ED_h

//
// Package:         CMS1/ED
// Class:           ED
// 
// Description:     event display for CMS1
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Tue Apr  3 21:47:43 UTC 2007
//
// $Author: gutsche $
// $Date: 2007/04/04 01:23:02 $
// $Revision: 1.1 $
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

//?????????????

#include "CMS1/Base/interface/EventData.h"
#include "CMS1/Base/interface/UserData.h"
#include "CMS1/Base/interface/UserDataBlocks.h"
#include "CMS1/Muons/interface/Muons.h"
#include "CMS1/Tracks/interface/Tracks.h"
#include "CMS1/Electrons/interface/Electrons.h"
#include "CMS1/Jets/interface/Jets.h"
#include "CMS1/MET/interface/MET.h"

//?????????????

#include "TCanvas.h"
#include "TH2D.h"
#include "TGaxis.h"
#include "TPaveLabel.h"
#include "TPad.h"
#include "TLine.h"
#include <TColor.h>
#include <TPolyLine.h>

class ED : public edm::EDAnalyzer {
 public:
  explicit ED(const edm::ParameterSet&);
  ~ED();

 private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

	virtual void displayCaloTowers(const edm::Event& iEvent);
	virtual void displayCaloTower(double eta, double phi, double et, int color, int black);

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
  TPad    *ef_pad_left_;
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

  TH2D    *ef_histo_;
  TGaxis  *ef_histo_axis_right_;
  TGaxis  *ef_histo_axis_top_;

  std::vector<TPaveLabel*> allRings_;
  std::vector<TPaveLabel*> innerSeedRings_;
  std::vector<TPaveLabel*> outerSeedRings_;

  const TrackerGeometry *tracker_;

  edm::InputTag trackingTruthInputTag_;
  edm::InputTag trajectorySeedInputTag_;


};

#endif
