#ifndef CMS1EventDisplay_h
#define CMS1EventDisplay_h

//
// Package:         CMS1/EventDisplay
// Class:           EventDisplay
// 
// Description:     event display for CMS1
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Tue Apr  3 21:47:43 UTC 2007
//
// $Author: latb $
// $Date: 2007/06/20 15:45:04 $
// $Revision: 1.2 $
//

#include <string>
#include <vector>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
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

class EventDisplay : public edm::EDAnalyzer {
 public:
  explicit EventDisplay(const edm::ParameterSet&);
  ~EventDisplay();

 private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  enum View_t { XY = 0, RZ = 1, EPE = 2, EPH = 3, NViews = 4 };
  enum Layer_t { BARREL = 0, ENDCAP = 1 };
	virtual void displayCaloTowers(View_t viewType, const edm::Event& iEvent);
	virtual void displayCaloTower(View_t viewType, CaloTower* t);


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
   
  double minTrackPt_;

	int hadColor_;
	int emColor_;

	double EBradius_;
	double EEz_;
	
	int RZprojective_;
	int XYshowEndcaps_;

	int debug_;
};

#endif
