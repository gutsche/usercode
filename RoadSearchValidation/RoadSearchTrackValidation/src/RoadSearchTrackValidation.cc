//
// Package:         RoadSearchValidation/RoadSearchTrackValidation
// Class:           RoadSearchTrackValidation
// 
// Description:     fills track validation histrograms and stores them in ROOT file
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Fri Sep  1 15:39:57 UTC 2006
//
// $Author: gutsche $
// $Date: 2006/09/08 19:36:04 $
// $Revision: 1.4 $
//

#include <string>

#include "RoadSearchValidation/RoadSearchTrackValidation/interface/RoadSearchTrackValidation.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/TrajectorySeed/interface/TrajectorySeed.h"
#include "DataFormats/TrajectorySeed/interface/TrajectorySeedCollection.h"
#include "DataFormats/RoadSearchCloud/interface/RoadSearchCloud.h"
#include "DataFormats/RoadSearchCloud/interface/RoadSearchCloudCollection.h"
#include "DataFormats/TrackCandidate/interface/TrackCandidate.h"
#include "DataFormats/TrackCandidate/interface/TrackCandidateCollection.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/SiStripDetId/interface/StripSubdetector.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"


RoadSearchTrackValidation::RoadSearchTrackValidation(const edm::ParameterSet& iConfig)
{

  file_ = new TFile(iConfig.getUntrackedParameter<std::string>("OutputFileName").c_str(),"RECREATE");
  
  // binning
  unsigned int chi2_nbins = 50;
  double       chi2_low   = 0.;
  double       chi2_high  = 50.;
  
  unsigned int pt_nbins = 160;
  double       pt_low   = 0.;
  double       pt_high  = 20.;

  unsigned int eta_nbins = 30;
  double       eta_low   = -3.;
  double       eta_high  =  3.;

  unsigned int phi_nbins = 36;
  double       phi_low   = -3.1416;
  double       phi_high  = 3.1416;

  unsigned int transCurv_nbins = 100;
  double       transCurv_low   = -0.25;
  double       transCurv_high  =  0.25;

  unsigned int phiZero_nbins = 100;
  double       phiZero_low   = -5.;
  double       phiZero_high  = 5.;

  unsigned int theta_nbins = 40;
  double       theta_low   = 0.;
  double       theta_high  = 4.;

  unsigned int dZero_nbins = 120;
  double       dZero_low   = -0.1;
  double       dZero_high  = 0.1;

  unsigned int dZ_nbins = 400;
  double       dZ_low   = -100.;
  double       dZ_high  = 100.;

  unsigned int numSeeds_nbins = 100000;
  unsigned int numSeeds_low   = 0;
  unsigned int numSeeds_high  = 100000;

  unsigned int numRawClouds_nbins = 100000;
  unsigned int numRawClouds_low   = 0;
  unsigned int numRawClouds_high  = 100000;

  unsigned int numCleanClouds_nbins = 100000;
  unsigned int numCleanClouds_low   = 0;
  unsigned int numCleanClouds_high  = 100000;

  unsigned int numTrackCandidates_nbins = 100000;
  unsigned int numTrackCandidates_low   = 0;
  unsigned int numTrackCandidates_high  = 100000;

  unsigned int numTracks_nbins = 100000;
  unsigned int numTracks_low   = 0;
  unsigned int numTracks_high  = 100000;

  unsigned int nhit_nbins = 31;
  double       nhit_low   = -0.5;
  double       nhit_high  = 30.5;

  // RS plots
  rs_chi2               = new TH1D("rs_chi2","Track reduced chi^{2}",chi2_nbins,chi2_low,chi2_high);
  rs_pt                 = new TH1D("rs_pt","Track p_{T} (GeV)",pt_nbins,pt_low,pt_high);
  rs_eta                = new TH1D("rs_eta","Track #eta",eta_nbins,eta_low,eta_high);
  rs_phi                = new TH1D("rs_phi","Track #phi",phi_nbins,phi_low,phi_high);
  rs_transCurv          = new TH1D("rs_transCurv","Transverse curvature",transCurv_nbins,transCurv_low,transCurv_high);
  rs_phiZero            = new TH1D("rs_phiZero","phiZero",phiZero_nbins,phiZero_low,phiZero_high);
  rs_theta              = new TH1D("rs_theta","Theta",theta_nbins,theta_low,theta_high);
  rs_dZero              = new TH1D("rs_dZero",",dZero",dZero_nbins,dZero_low,dZero_high);
  rs_dZ                 = new TH1D("rs_dZ","dZ",dZ_nbins,dZ_low,dZ_high);
  rs_nhit               = new TH1D("rs_nhit","Number of hits per track",nhit_nbins,nhit_low,nhit_high);

  rs_numSeeds           = new TH1I("rs_numSeeds","Number of Seeds per event",numSeeds_nbins,numSeeds_low,numSeeds_high);
  rs_numRawClouds       = new TH1I("rs_numRawClouds","Number of raw clouds per event",numRawClouds_nbins,numRawClouds_low,numRawClouds_high);
  rs_numCleanClouds     = new TH1I("rs_numCleanClouds","Number of clean clouds per event",numCleanClouds_nbins,numCleanClouds_low,numCleanClouds_high);
  rs_numTrackCandidates = new TH1I("rs_numTrackCandidates","Number of track candidates per event",numTrackCandidates_nbins,numTrackCandidates_low,numTrackCandidates_high);
  rs_numTracks          = new TH1I("rs_numTracks","Number of tracks per event",numTracks_nbins,numTracks_low,numTracks_high);

  rs_nHitPerTrackVsEta = new TH2D("rs_nHitPerTrackVsEta","Number of hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  rs_nHitPerTrackCandidateVsEta = new TH2D("rs_nHitPerTrackCandidateVsEta","Number of hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  rs_nHitPerCleanCloudVsEta = new TH2D("rs_nHitPerCleanCloudVsEta","Number of hits per clean cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  rs_nHitPerRawCloudVsEta = new TH2D("rs_nHitPerRawCloudVsEta","Number of hits per raw cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);

  rs_nHitPerTrackVsPhi = new TH2D("rs_nHitPerTrackVsPhi","Number of hits per track vs. #phi",nhit_nbins,nhit_low,nhit_high,phi_nbins,phi_low,phi_high);

  rs_nStripHitPerTrackVsEta = new TH2D("rs_nStripHitPerTrackVsEta","Number of strip hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  rs_nStripHitPerTrackCandidateVsEta = new TH2D("rs_nStripHitPerTrackCandidateVsEta","Number of strip hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  rs_nStripHitPerCleanCloudVsEta = new TH2D("rs_nStripHitPerCleanCloudVsEta","Number of strip hits per clean cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  rs_nStripHitPerRawCloudVsEta = new TH2D("rs_nStripHitPerRawCloudVsEta","Number of strip hits per raw cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);

  rs_nPixelHitPerTrackVsEta = new TH2D("rs_nPixelHitPerTrackVsEta","Number of pixel hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  rs_nPixelHitPerTrackCandidateVsEta = new TH2D("rs_nPixelHitPerTrackCandidateVsEta","Number of pixel hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  rs_nPixelHitPerCleanCloudVsEta = new TH2D("rs_nPixelHitPerCleanCloudVsEta","Number of pixel hits per clean cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  rs_nPixelHitPerRawCloudVsEta = new TH2D("rs_nPixelHitPerRawCloudVsEta","Number of pixel hits per raw cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);

  rs_nTIBHitPerTrackVsEta = new TH2D("rs_nTIBHitPerTrackVsEta","Number of TIB hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  rs_nTIBHitPerTrackCandidateVsEta = new TH2D("rs_nTIBHitPerTrackCandidateVsEta","Number of TIB hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  rs_nTIBHitPerCleanCloudVsEta = new TH2D("rs_nTIBHitPerCleanCloudVsEta","Number of TIB hits per clean cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  rs_nTIBHitPerRawCloudVsEta = new TH2D("rs_nTIBHitPerRawCloudVsEta","Number of TIB hits per raw cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);

  rs_nTOBHitPerTrackVsEta = new TH2D("rs_nTOBHitPerTrackVsEta","Number of TOB hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  rs_nTOBHitPerTrackCandidateVsEta = new TH2D("rs_nTOBHitPerTrackCandidateVsEta","Number of TOB hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  rs_nTOBHitPerCleanCloudVsEta = new TH2D("rs_nTOBHitPerCleanCloudVsEta","Number of TOB hits per clean cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  rs_nTOBHitPerRawCloudVsEta = new TH2D("rs_nTOBHitPerRawCloudVsEta","Number of TOB hits per raw cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);

  rs_nTIDHitPerTrackVsEta = new TH2D("rs_nTIDHitPerTrackVsEta","Number of TID hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  rs_nTIDHitPerTrackCandidateVsEta = new TH2D("rs_nTIDHitPerTrackCandidateVsEta","Number of TID hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  rs_nTIDHitPerCleanCloudVsEta = new TH2D("rs_nTIDHitPerCleanCloudVsEta","Number of TID hits per clean cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  rs_nTIDHitPerRawCloudVsEta = new TH2D("rs_nTIDHitPerRawCloudVsEta","Number of TID hits per raw cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);

  rs_nTECHitPerTrackVsEta = new TH2D("rs_nTECHitPerTrackVsEta","Number of TEC hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  rs_nTECHitPerTrackCandidateVsEta = new TH2D("rs_nTECHitPerTrackCandidateVsEta","Number of TEC hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  rs_nTECHitPerCleanCloudVsEta = new TH2D("rs_nTECHitPerCleanCloudVsEta","Number of TEC hits per clean cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  rs_nTECHitPerRawCloudVsEta = new TH2D("rs_nTECHitPerRawCloudVsEta","Number of TEC hits per raw cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);

  rs_nPXBHitPerTrackVsEta = new TH2D("rs_nPXBHitPerTrackVsEta","Number of PXB hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  rs_nPXBHitPerTrackCandidateVsEta = new TH2D("rs_nPXBHitPerTrackCandidateVsEta","Number of PXB hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  rs_nPXBHitPerCleanCloudVsEta = new TH2D("rs_nPXBHitPerCleanCloudVsEta","Number of PXB hits per clean cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  rs_nPXBHitPerRawCloudVsEta = new TH2D("rs_nPXBHitPerRawCloudVsEta","Number of PXB hits per raw cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);

  rs_nPXFHitPerTrackVsEta = new TH2D("rs_nPXFHitPerTrackVsEta","Number of PXF hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  rs_nPXFHitPerTrackCandidateVsEta = new TH2D("rs_nPXFHitPerTrackCandidateVsEta","Number of PXF hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  rs_nPXFHitPerCleanCloudVsEta = new TH2D("rs_nPXFHitPerCleanCloudVsEta","Number of PXF hits per clean cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  rs_nPXFHitPerRawCloudVsEta = new TH2D("rs_nPXFHitPerRawCloudVsEta","Number of PXF hits per raw cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);

  // CkF plots
  ckf_chi2               = new TH1D("ckf_chi2","Track reduced chi^{2}",chi2_nbins,chi2_low,chi2_high);
  ckf_pt                 = new TH1D("ckf_pt","Track p_{T} (GeV)",pt_nbins,pt_low,pt_high);
  ckf_eta                = new TH1D("ckf_eta","Track #eta",eta_nbins,eta_low,eta_high);
  ckf_phi                = new TH1D("ckf_phi","Track #phi",phi_nbins,phi_low,phi_high);
  ckf_transCurv          = new TH1D("ckf_transCurv","Transverse curvature",transCurv_nbins,transCurv_low,transCurv_high);
  ckf_phiZero            = new TH1D("ckf_phiZero","phiZero",phiZero_nbins,phiZero_low,phiZero_high);
  ckf_theta              = new TH1D("ckf_theta","Theta",theta_nbins,theta_low,theta_high);
  ckf_dZero              = new TH1D("ckf_dZero","dZero",dZero_nbins,dZero_low,dZero_high);
  ckf_dZ                 = new TH1D("ckf_dZ","dZ",dZ_nbins,dZ_low,dZ_high);
  ckf_nhit               = new TH1D("ckf_nhit","Number of hits per track",nhit_nbins,nhit_low,nhit_high);

  ckf_numSeeds           = new TH1I("ckf_numSeeds","Number of Seeds per event",numSeeds_nbins,numSeeds_low,numSeeds_high);
  ckf_numRawClouds       = new TH1I("ckf_numRawClouds","Number of raw clouds per event",numRawClouds_nbins,numRawClouds_low,numRawClouds_high);
  ckf_numCleanClouds     = new TH1I("ckf_numCleanClouds","Number of clean clouds per event",numCleanClouds_nbins,numCleanClouds_low,numCleanClouds_high);
  ckf_numTrackCandidates = new TH1I("ckf_numTrackCandidates","Number of track candidates per event",numTrackCandidates_nbins,numTrackCandidates_low,numTrackCandidates_high);
  ckf_numTracks          = new TH1I("ckf_numTracks","Number of tracks per event",numTracks_nbins,numTracks_low,numTracks_high);
  
  ckf_nHitPerTrackVsEta = new TH2D("ckf_nHitPerTrackVsEta","Number of hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  ckf_nHitPerTrackCandidateVsEta = new TH2D("ckf_nHitPerTrackCandidateVsEta","Number of hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);

  ckf_nHitPerTrackVsPhi = new TH2D("ckf_nHitPerTrackVsPhi","Number of hits per track vs. #phi",nhit_nbins,nhit_low,nhit_high,phi_nbins,phi_low,phi_high);

  ckf_nStripHitPerTrackVsEta = new TH2D("ckf_nStripHitPerTrackVsEta","Number of strip hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  ckf_nStripHitPerTrackCandidateVsEta = new TH2D("ckf_nStripHitPerTrackCandidateVsEta","Number of strip hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);

  ckf_nPixelHitPerTrackVsEta = new TH2D("ckf_nPixelHitPerTrackVsEta","Number of pixel hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  ckf_nPixelHitPerTrackCandidateVsEta = new TH2D("ckf_nPixelHitPerTrackCandidateVsEta","Number of pixel hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);

  ckf_nTIBHitPerTrackVsEta = new TH2D("ckf_nTIBHitPerTrackVsEta","Number of TIB hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  ckf_nTIBHitPerTrackCandidateVsEta = new TH2D("ckf_nTIBHitPerTrackCandidateVsEta","Number of TIB hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);

  ckf_nTOBHitPerTrackVsEta = new TH2D("ckf_nTOBHitPerTrackVsEta","Number of TOB hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  ckf_nTOBHitPerTrackCandidateVsEta = new TH2D("ckf_nTOBHitPerTrackCandidateVsEta","Number of TOB hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);

  ckf_nTIDHitPerTrackVsEta = new TH2D("ckf_nTIDHitPerTrackVsEta","Number of TID hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  ckf_nTIDHitPerTrackCandidateVsEta = new TH2D("ckf_nTIDHitPerTrackCandidateVsEta","Number of TID hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);

  ckf_nTECHitPerTrackVsEta = new TH2D("ckf_nTECHitPerTrackVsEta","Number of TEC hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  ckf_nTECHitPerTrackCandidateVsEta = new TH2D("ckf_nTECHitPerTrackCandidateVsEta","Number of TEC hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);

  ckf_nPXBHitPerTrackVsEta = new TH2D("ckf_nPXBHitPerTrackVsEta","Number of PXB hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  ckf_nPXBHitPerTrackCandidateVsEta = new TH2D("ckf_nPXBHitPerTrackCandidateVsEta","Number of PXB hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);

  ckf_nPXFHitPerTrackVsEta = new TH2D("ckf_nPXFHitPerTrackVsEta","Number of PXF hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  ckf_nPXFHitPerTrackCandidateVsEta = new TH2D("ckf_nPXFHitPerTrackCandidateVsEta","Number of PXF hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);

  // RS plots
  rs_same_chi2               = new TH1D("rs_same_chi2","Track reduced chi^{2}",chi2_nbins,chi2_low,chi2_high);
  rs_same_pt                 = new TH1D("rs_same_pt","Track p_{T} (GeV)",pt_nbins,pt_low,pt_high);
  rs_same_eta                = new TH1D("rs_same_eta","Track #eta",eta_nbins,eta_low,eta_high);
  rs_same_phi                = new TH1D("rs_same_phi","Track #phi",phi_nbins,phi_low,phi_high);
  rs_same_transCurv          = new TH1D("rs_same_transCurv","Transverse curvature",transCurv_nbins,transCurv_low,transCurv_high);
  rs_same_phiZero            = new TH1D("rs_same_phiZero","phiZero",phiZero_nbins,phiZero_low,phiZero_high);
  rs_same_theta              = new TH1D("rs_same_theta","Theta",theta_nbins,theta_low,theta_high);
  rs_same_dZero              = new TH1D("rs_same_dZero",",dZero",dZero_nbins,dZero_low,dZero_high);
  rs_same_dZ                 = new TH1D("rs_same_dZ","dZ",dZ_nbins,dZ_low,dZ_high);
  rs_same_nhit               = new TH1D("rs_same_nhit","Number of hits per track",nhit_nbins,nhit_low,nhit_high);

  rs_same_numSeeds           = new TH1I("rs_same_numSeeds","Number of Seeds per event",numSeeds_nbins,numSeeds_low,numSeeds_high);
  rs_same_numRawClouds       = new TH1I("rs_same_numRawClouds","Number of raw clouds per event",numRawClouds_nbins,numRawClouds_low,numRawClouds_high);
  rs_same_numCleanClouds     = new TH1I("rs_same_numCleanClouds","Number of clean clouds per event",numCleanClouds_nbins,numCleanClouds_low,numCleanClouds_high);
  rs_same_numTrackCandidates = new TH1I("rs_same_numTrackCandidates","Number of track candidates per event",numTrackCandidates_nbins,numTrackCandidates_low,numTrackCandidates_high);
  rs_same_numTracks          = new TH1I("rs_same_numTracks","Number of tracks per event",numTracks_nbins,numTracks_low,numTracks_high);

  rs_same_nHitPerTrackVsEta = new TH2D("rs_same_nHitPerTrackVsEta","Number of hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  rs_same_nHitPerTrackCandidateVsEta = new TH2D("rs_same_nHitPerTrackCandidateVsEta","Number of hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  rs_same_nHitPerCleanCloudVsEta = new TH2D("rs_same_nHitPerCleanCloudVsEta","Number of hits per clean cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  rs_same_nHitPerRawCloudVsEta = new TH2D("rs_same_nHitPerRawCloudVsEta","Number of hits per raw cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);

  rs_same_nStripHitPerTrackVsEta = new TH2D("rs_same_nStripHitPerTrackVsEta","Number of strip hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  rs_same_nStripHitPerTrackCandidateVsEta = new TH2D("rs_same_nStripHitPerTrackCandidateVsEta","Number of strip hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  rs_same_nStripHitPerCleanCloudVsEta = new TH2D("rs_same_nStripHitPerCleanCloudVsEta","Number of strip hits per clean cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  rs_same_nStripHitPerRawCloudVsEta = new TH2D("rs_same_nStripHitPerRawCloudVsEta","Number of strip hits per raw cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);

  rs_same_nPixelHitPerTrackVsEta = new TH2D("rs_same_nPixelHitPerTrackVsEta","Number of pixel hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  rs_same_nPixelHitPerTrackCandidateVsEta = new TH2D("rs_same_nPixelHitPerTrackCandidateVsEta","Number of pixel hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  rs_same_nPixelHitPerCleanCloudVsEta = new TH2D("rs_same_nPixelHitPerCleanCloudVsEta","Number of pixel hits per clean cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  rs_same_nPixelHitPerRawCloudVsEta = new TH2D("rs_same_nPixelHitPerRawCloudVsEta","Number of pixel hits per raw cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);

  rs_same_nTIBHitPerTrackVsEta = new TH2D("rs_same_nTIBHitPerTrackVsEta","Number of TIB hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  rs_same_nTIBHitPerTrackCandidateVsEta = new TH2D("rs_same_nTIBHitPerTrackCandidateVsEta","Number of TIB hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  rs_same_nTIBHitPerCleanCloudVsEta = new TH2D("rs_same_nTIBHitPerCleanCloudVsEta","Number of TIB hits per clean cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  rs_same_nTIBHitPerRawCloudVsEta = new TH2D("rs_same_nTIBHitPerRawCloudVsEta","Number of TIB hits per raw cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);

  rs_same_nTOBHitPerTrackVsEta = new TH2D("rs_same_nTOBHitPerTrackVsEta","Number of TOB hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  rs_same_nTOBHitPerTrackCandidateVsEta = new TH2D("rs_same_nTOBHitPerTrackCandidateVsEta","Number of TOB hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  rs_same_nTOBHitPerCleanCloudVsEta = new TH2D("rs_same_nTOBHitPerCleanCloudVsEta","Number of TOB hits per clean cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  rs_same_nTOBHitPerRawCloudVsEta = new TH2D("rs_same_nTOBHitPerRawCloudVsEta","Number of TOB hits per raw cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);

  rs_same_nTIDHitPerTrackVsEta = new TH2D("rs_same_nTIDHitPerTrackVsEta","Number of TID hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  rs_same_nTIDHitPerTrackCandidateVsEta = new TH2D("rs_same_nTIDHitPerTrackCandidateVsEta","Number of TID hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  rs_same_nTIDHitPerCleanCloudVsEta = new TH2D("rs_same_nTIDHitPerCleanCloudVsEta","Number of TID hits per clean cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  rs_same_nTIDHitPerRawCloudVsEta = new TH2D("rs_same_nTIDHitPerRawCloudVsEta","Number of TID hits per raw cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);

  rs_same_nTECHitPerTrackVsEta = new TH2D("rs_same_nTECHitPerTrackVsEta","Number of TEC hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  rs_same_nTECHitPerTrackCandidateVsEta = new TH2D("rs_same_nTECHitPerTrackCandidateVsEta","Number of TEC hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  rs_same_nTECHitPerCleanCloudVsEta = new TH2D("rs_same_nTECHitPerCleanCloudVsEta","Number of TEC hits per clean cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  rs_same_nTECHitPerRawCloudVsEta = new TH2D("rs_same_nTECHitPerRawCloudVsEta","Number of TEC hits per raw cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);

  rs_same_nPXBHitPerTrackVsEta = new TH2D("rs_same_nPXBHitPerTrackVsEta","Number of PXB hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  rs_same_nPXBHitPerTrackCandidateVsEta = new TH2D("rs_same_nPXBHitPerTrackCandidateVsEta","Number of PXB hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  rs_same_nPXBHitPerCleanCloudVsEta = new TH2D("rs_same_nPXBHitPerCleanCloudVsEta","Number of PXB hits per clean cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  rs_same_nPXBHitPerRawCloudVsEta = new TH2D("rs_same_nPXBHitPerRawCloudVsEta","Number of PXB hits per raw cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);

  rs_same_nPXFHitPerTrackVsEta = new TH2D("rs_same_nPXFHitPerTrackVsEta","Number of PXF hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  rs_same_nPXFHitPerTrackCandidateVsEta = new TH2D("rs_same_nPXFHitPerTrackCandidateVsEta","Number of PXF hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  rs_same_nPXFHitPerCleanCloudVsEta = new TH2D("rs_same_nPXFHitPerCleanCloudVsEta","Number of PXF hits per clean cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  rs_same_nPXFHitPerRawCloudVsEta = new TH2D("rs_same_nPXFHitPerRawCloudVsEta","Number of PXF hits per raw cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);

  // CkF plots
  ckf_same_chi2               = new TH1D("ckf_same_chi2","Track reduced chi^{2}",chi2_nbins,chi2_low,chi2_high);
  ckf_same_pt                 = new TH1D("ckf_same_pt","Track p_{T} (GeV)",pt_nbins,pt_low,pt_high);
  ckf_same_eta                = new TH1D("ckf_same_eta","Track #eta",eta_nbins,eta_low,eta_high);
  ckf_same_phi                = new TH1D("ckf_same_phi","Track #phi",phi_nbins,phi_low,phi_high);
  ckf_same_transCurv          = new TH1D("ckf_same_transCurv","Transverse curvature",transCurv_nbins,transCurv_low,transCurv_high);
  ckf_same_phiZero            = new TH1D("ckf_same_phiZero","phiZero",phiZero_nbins,phiZero_low,phiZero_high);
  ckf_same_theta              = new TH1D("ckf_same_theta","Theta",theta_nbins,theta_low,theta_high);
  ckf_same_dZero              = new TH1D("ckf_same_dZero","dZero",dZero_nbins,dZero_low,dZero_high);
  ckf_same_dZ                 = new TH1D("ckf_same_dZ","dZ",dZ_nbins,dZ_low,dZ_high);
  ckf_same_nhit               = new TH1D("ckf_same_nhit","Number of hits per track",nhit_nbins,nhit_low,nhit_high);

  ckf_same_numSeeds           = new TH1I("ckf_same_numSeeds","Number of Seeds per event",numSeeds_nbins,numSeeds_low,numSeeds_high);
  ckf_same_numRawClouds       = new TH1I("ckf_same_numRawClouds","Number of raw clouds per event",numRawClouds_nbins,numRawClouds_low,numRawClouds_high);
  ckf_same_numCleanClouds     = new TH1I("ckf_same_numCleanClouds","Number of clean clouds per event",numCleanClouds_nbins,numCleanClouds_low,numCleanClouds_high);
  ckf_same_numTrackCandidates = new TH1I("ckf_same_numTrackCandidates","Number of track candidates per event",numTrackCandidates_nbins,numTrackCandidates_low,numTrackCandidates_high);
  ckf_same_numTracks          = new TH1I("ckf_same_numTracks","Number of tracks per event",numTracks_nbins,numTracks_low,numTracks_high);
  
  ckf_same_nHitPerTrackVsEta = new TH2D("ckf_same_nHitPerTrackVsEta","Number of hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  ckf_same_nHitPerTrackCandidateVsEta = new TH2D("ckf_same_nHitPerTrackCandidateVsEta","Number of hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);

  ckf_same_nStripHitPerTrackVsEta = new TH2D("ckf_same_nStripHitPerTrackVsEta","Number of strip hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  ckf_same_nStripHitPerTrackCandidateVsEta = new TH2D("ckf_same_nStripHitPerTrackCandidateVsEta","Number of strip hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);

  ckf_same_nPixelHitPerTrackVsEta = new TH2D("ckf_same_nPixelHitPerTrackVsEta","Number of pixel hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  ckf_same_nPixelHitPerTrackCandidateVsEta = new TH2D("ckf_same_nPixelHitPerTrackCandidateVsEta","Number of pixel hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);

  ckf_same_nTIBHitPerTrackVsEta = new TH2D("ckf_same_nTIBHitPerTrackVsEta","Number of TIB hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  ckf_same_nTIBHitPerTrackCandidateVsEta = new TH2D("ckf_same_nTIBHitPerTrackCandidateVsEta","Number of TIB hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);

  ckf_same_nTOBHitPerTrackVsEta = new TH2D("ckf_same_nTOBHitPerTrackVsEta","Number of TOB hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  ckf_same_nTOBHitPerTrackCandidateVsEta = new TH2D("ckf_same_nTOBHitPerTrackCandidateVsEta","Number of TOB hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);

  ckf_same_nTIDHitPerTrackVsEta = new TH2D("ckf_same_nTIDHitPerTrackVsEta","Number of TID hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  ckf_same_nTIDHitPerTrackCandidateVsEta = new TH2D("ckf_same_nTIDHitPerTrackCandidateVsEta","Number of TID hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);

  ckf_same_nTECHitPerTrackVsEta = new TH2D("ckf_same_nTECHitPerTrackVsEta","Number of TEC hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  ckf_same_nTECHitPerTrackCandidateVsEta = new TH2D("ckf_same_nTECHitPerTrackCandidateVsEta","Number of TEC hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);

  ckf_same_nPXBHitPerTrackVsEta = new TH2D("ckf_same_nPXBHitPerTrackVsEta","Number of PXB hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  ckf_same_nPXBHitPerTrackCandidateVsEta = new TH2D("ckf_same_nPXBHitPerTrackCandidateVsEta","Number of PXB hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);

  ckf_same_nPXFHitPerTrackVsEta = new TH2D("ckf_same_nPXFHitPerTrackVsEta","Number of PXF hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);
  ckf_same_nPXFHitPerTrackCandidateVsEta = new TH2D("ckf_same_nPXFHitPerTrackCandidateVsEta","Number of PXF hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high);

  // get parameter
  rsSeedProducerLabel_ = iConfig.getUntrackedParameter<std::string>("rsSeedProducerLabel");
  rsRawCloudProducerLabel_ = iConfig.getUntrackedParameter<std::string>("rsRawCloudProducerLabel");
  rsCleanCloudProducerLabel_ = iConfig.getUntrackedParameter<std::string>("rsCleanCloudProducerLabel");
  rsTrackCandidateProducerLabel_ = iConfig.getUntrackedParameter<std::string>("rsTrackCandidateProducerLabel");
  rsTrackProducerLabel_ = iConfig.getUntrackedParameter<std::string>("rsTrackProducerLabel");

  ckfSeedProducerLabel_ = iConfig.getUntrackedParameter<std::string>("ckfSeedProducerLabel");
  ckfTrackCandidateProducerLabel_ = iConfig.getUntrackedParameter<std::string>("ckfTrackCandidateProducerLabel");
  ckfTrackProducerLabel_ = iConfig.getUntrackedParameter<std::string>("ckfTrackProducerLabel");

  sameNumberOfTracks = iConfig.getUntrackedParameter<unsigned int>("sameNumberOfTracks");

}


RoadSearchTrackValidation::~RoadSearchTrackValidation()
{
 
  if ( file_ != 0 ) {
    file_->cd();
    file_->Write();
    delete file_;
  }

}


void
RoadSearchTrackValidation::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  // get collections
  edm::Handle<TrajectorySeedCollection> rsSeedCollectionHandle;
  iEvent.getByLabel(rsSeedProducerLabel_,rsSeedCollectionHandle);
  const TrajectorySeedCollection *rsSeedCollection = rsSeedCollectionHandle.product();

  edm::Handle<RoadSearchCloudCollection> rsRawCloudCollectionHandle;
  iEvent.getByLabel(rsRawCloudProducerLabel_,rsRawCloudCollectionHandle);
  const RoadSearchCloudCollection *rsRawCloudCollection = rsRawCloudCollectionHandle.product();

  edm::Handle<RoadSearchCloudCollection> rsCleanCloudCollectionHandle;
  iEvent.getByLabel(rsCleanCloudProducerLabel_,rsCleanCloudCollectionHandle);
  const RoadSearchCloudCollection *rsCleanCloudCollection = rsCleanCloudCollectionHandle.product();

  edm::Handle<TrackCandidateCollection> rsTrackCandidateCollectionHandle;
  iEvent.getByLabel(rsTrackCandidateProducerLabel_,rsTrackCandidateCollectionHandle);
  const TrackCandidateCollection *rsTrackCandidateCollection = rsTrackCandidateCollectionHandle.product();

  edm::Handle<reco::TrackCollection> rsTrackCollectionHandle;
  iEvent.getByLabel(rsTrackProducerLabel_,rsTrackCollectionHandle);
  const reco::TrackCollection *rsTrackCollection = rsTrackCollectionHandle.product();

  edm::Handle<TrajectorySeedCollection> ckfSeedCollectionHandle;
  iEvent.getByLabel(ckfSeedProducerLabel_,ckfSeedCollectionHandle);
  const TrajectorySeedCollection *ckfSeedCollection = ckfSeedCollectionHandle.product();

  edm::Handle<TrackCandidateCollection> ckfTrackCandidateCollectionHandle;
  iEvent.getByLabel(ckfTrackCandidateProducerLabel_,ckfTrackCandidateCollectionHandle);
  const TrackCandidateCollection *ckfTrackCandidateCollection = ckfTrackCandidateCollectionHandle.product();

  edm::Handle<reco::TrackCollection> ckfTrackCollectionHandle;
  iEvent.getByLabel(ckfTrackProducerLabel_,ckfTrackCollectionHandle);
  const reco::TrackCollection *ckfTrackCollection = ckfTrackCollectionHandle.product();

  // fill histograms
  
  // RS tracks
  rs_numTracks->Fill(rsTrackCollection->size());
  for ( reco::TrackCollection::const_iterator track = rsTrackCollection->begin();
	track != rsTrackCollection->end();
	++track ) {
    rs_chi2->Fill(track->normalizedChi2());
    rs_pt->Fill(track->pt());
    rs_nhit->Fill(track->found());
    rs_eta->Fill(track->momentum().eta());
    rs_phi->Fill(track->momentum().phi());
    rs_transCurv->Fill(track->transverseCurvature());
    rs_phiZero->Fill(track->phi0());
    rs_theta->Fill(track->theta());
    rs_dZero->Fill(track->d0());
    rs_dZ->Fill(track->dz());

    // loop over hits in tracks, count
    unsigned int nHit      = 0;
    unsigned int nStripHit = 0;
    unsigned int nPixelHit = 0;
    unsigned int nTIBHit   = 0;
    unsigned int nTOBHit   = 0;
    unsigned int nTIDHit   = 0;
    unsigned int nTECHit   = 0;
    unsigned int nPXBHit   = 0;
    unsigned int nPXFHit   = 0;

    for ( trackingRecHit_iterator recHit = track->recHitsBegin();
	  recHit != track->recHitsEnd();
	  ++recHit ) {
      ++nHit;
      DetId id((*recHit)->geographicalId());

      if ( (unsigned int)id.subdetId() == StripSubdetector::TIB ) {
	++nStripHit;
	++nTIBHit;
      } else if ( (unsigned int)id.subdetId() == StripSubdetector::TOB ) {
	++nStripHit;
	++nTOBHit;
      } else if ( (unsigned int)id.subdetId() == StripSubdetector::TID ) {
	++nStripHit;
	++nTIDHit;
      } else if ( (unsigned int)id.subdetId() == StripSubdetector::TEC ) {
	++nStripHit;
	++nTECHit;
      } else if ( (unsigned int)id.subdetId() == PixelSubdetector::PixelBarrel ) {
	++nPixelHit;
	++nPXBHit;
      } else if ( (unsigned int)id.subdetId() == PixelSubdetector::PixelEndcap ) {
	++nPixelHit;
	++nPXFHit;
      }
    }
    
    rs_nHitPerTrackVsEta->Fill(nHit,track->momentum().eta());
    rs_nStripHitPerTrackVsEta->Fill(nStripHit,track->momentum().eta());
    rs_nPixelHitPerTrackVsEta->Fill(nPixelHit,track->momentum().eta());
    rs_nTIBHitPerTrackVsEta->Fill(nTIBHit,track->momentum().eta());
    rs_nTOBHitPerTrackVsEta->Fill(nTOBHit,track->momentum().eta());
    rs_nTIDHitPerTrackVsEta->Fill(nTIDHit,track->momentum().eta());
    rs_nTECHitPerTrackVsEta->Fill(nTECHit,track->momentum().eta());
    rs_nPXBHitPerTrackVsEta->Fill(nPXBHit,track->momentum().eta());
    rs_nPXFHitPerTrackVsEta->Fill(nPXFHit,track->momentum().eta());
    rs_nHitPerTrackVsPhi->Fill(nHit,track->momentum().phi());

  }

  // RS seeds
  rs_numSeeds->Fill(rsSeedCollection->size());

  // RS raw clouds
  rs_numRawClouds->Fill(rsRawCloudCollection->size());
  for ( RoadSearchCloudCollection::const_iterator cloud = rsRawCloudCollection->begin();
	cloud != rsRawCloudCollection->end();
	++cloud ) {

    // loop over hits in tracks, count
    unsigned int nHit      = 0;
    unsigned int nStripHit = 0;
    unsigned int nPixelHit = 0;
    unsigned int nTIBHit   = 0;
    unsigned int nTOBHit   = 0;
    unsigned int nTIDHit   = 0;
    unsigned int nTECHit   = 0;
    unsigned int nPXBHit   = 0;
    unsigned int nPXFHit   = 0;

    for ( RoadSearchCloud::RecHitOwnVector::const_iterator recHit = cloud->begin_hits();
	  recHit != cloud->end_hits();
	  ++recHit ) {
      ++nHit;
      DetId id(recHit->geographicalId());
      
      if ( (unsigned int)id.subdetId() == StripSubdetector::TIB ) {
	++nStripHit;
	++nTIBHit;
      } else if ( (unsigned int)id.subdetId() == StripSubdetector::TOB ) {
	++nStripHit;
	++nTOBHit;
      } else if ( (unsigned int)id.subdetId() == StripSubdetector::TID ) {
	++nStripHit;
	++nTIDHit;
      } else if ( (unsigned int)id.subdetId() == StripSubdetector::TEC ) {
	++nStripHit;
	++nTECHit;
      } else if ( (unsigned int)id.subdetId() == PixelSubdetector::PixelBarrel ) {
	++nPixelHit;
	++nPXBHit;
      } else if ( (unsigned int)id.subdetId() == PixelSubdetector::PixelEndcap ) {
	++nPixelHit;
	++nPXFHit;
      }
    }
    
    rs_nHitPerRawCloudVsEta->Fill(nHit,0.);
    rs_nStripHitPerRawCloudVsEta->Fill(nStripHit,0.);
    rs_nPixelHitPerRawCloudVsEta->Fill(nPixelHit,0.);
    rs_nTIBHitPerRawCloudVsEta->Fill(nTIBHit,0.);
    rs_nTOBHitPerRawCloudVsEta->Fill(nTOBHit,0.);
    rs_nTIDHitPerRawCloudVsEta->Fill(nTIDHit,0.);
    rs_nTECHitPerRawCloudVsEta->Fill(nTECHit,0.);
    rs_nPXBHitPerRawCloudVsEta->Fill(nPXBHit,0.);
    rs_nPXFHitPerRawCloudVsEta->Fill(nPXFHit,0.);

  }

  // RS clean clouds
  rs_numCleanClouds->Fill(rsCleanCloudCollection->size());
  for ( RoadSearchCloudCollection::const_iterator cloud = rsCleanCloudCollection->begin();
	cloud != rsCleanCloudCollection->end();
	++cloud ) {

    // loop over hits in tracks, count
    unsigned int nHit      = 0;
    unsigned int nStripHit = 0;
    unsigned int nPixelHit = 0;
    unsigned int nTIBHit   = 0;
    unsigned int nTOBHit   = 0;
    unsigned int nTIDHit   = 0;
    unsigned int nTECHit   = 0;
    unsigned int nPXBHit   = 0;
    unsigned int nPXFHit   = 0;

    for ( RoadSearchCloud::RecHitOwnVector::const_iterator recHit = cloud->begin_hits();
	  recHit != cloud->end_hits();
	  ++recHit ) {
      ++nHit;
      DetId id(recHit->geographicalId());
      
      if ( (unsigned int)id.subdetId() == StripSubdetector::TIB ) {
	++nStripHit;
	++nTIBHit;
      } else if ( (unsigned int)id.subdetId() == StripSubdetector::TOB ) {
	++nStripHit;
	++nTOBHit;
      } else if ( (unsigned int)id.subdetId() == StripSubdetector::TID ) {
	++nStripHit;
	++nTIDHit;
      } else if ( (unsigned int)id.subdetId() == StripSubdetector::TEC ) {
	++nStripHit;
	++nTECHit;
      } else if ( (unsigned int)id.subdetId() == PixelSubdetector::PixelBarrel ) {
	++nPixelHit;
	++nPXBHit;
      } else if ( (unsigned int)id.subdetId() == PixelSubdetector::PixelEndcap ) {
	++nPixelHit;
	++nPXFHit;
      }
    }
    
    rs_nHitPerCleanCloudVsEta->Fill(nHit,0.);
    rs_nStripHitPerCleanCloudVsEta->Fill(nStripHit,0.);
    rs_nPixelHitPerCleanCloudVsEta->Fill(nPixelHit,0.);
    rs_nTIBHitPerCleanCloudVsEta->Fill(nTIBHit,0.);
    rs_nTOBHitPerCleanCloudVsEta->Fill(nTOBHit,0.);
    rs_nTIDHitPerCleanCloudVsEta->Fill(nTIDHit,0.);
    rs_nTECHitPerCleanCloudVsEta->Fill(nTECHit,0.);
    rs_nPXBHitPerCleanCloudVsEta->Fill(nPXBHit,0.);
    rs_nPXFHitPerCleanCloudVsEta->Fill(nPXFHit,0.);

  }

  // RS track candidates
  rs_numTrackCandidates->Fill(rsTrackCandidateCollection->size());
  for ( TrackCandidateCollection::const_iterator candidate = rsTrackCandidateCollection->begin();
	candidate != rsTrackCandidateCollection->end();
	++candidate ) {

    // loop over hits in tracks, count
    unsigned int nHit      = 0;
    unsigned int nStripHit = 0;
    unsigned int nPixelHit = 0;
    unsigned int nTIBHit   = 0;
    unsigned int nTOBHit   = 0;
    unsigned int nTIDHit   = 0;
    unsigned int nTECHit   = 0;
    unsigned int nPXBHit   = 0;
    unsigned int nPXFHit   = 0;

    for ( TrackCandidate::RecHitContainer::const_iterator recHit = candidate->recHits().first;
	  recHit != candidate->recHits().second;
	  ++recHit ) {
      ++nHit;
      DetId id(recHit->geographicalId());
      
      if ( (unsigned int)id.subdetId() == StripSubdetector::TIB ) {
	++nStripHit;
	++nTIBHit;
      } else if ( (unsigned int)id.subdetId() == StripSubdetector::TOB ) {
	++nStripHit;
	++nTOBHit;
      } else if ( (unsigned int)id.subdetId() == StripSubdetector::TID ) {
	++nStripHit;
	++nTIDHit;
      } else if ( (unsigned int)id.subdetId() == StripSubdetector::TEC ) {
	++nStripHit;
	++nTECHit;
      } else if ( (unsigned int)id.subdetId() == PixelSubdetector::PixelBarrel ) {
	++nPixelHit;
	++nPXBHit;
      } else if ( (unsigned int)id.subdetId() == PixelSubdetector::PixelEndcap ) {
	++nPixelHit;
	++nPXFHit;
      }
    }
    
    rs_nHitPerTrackCandidateVsEta->Fill(nHit,0.);
    rs_nStripHitPerTrackCandidateVsEta->Fill(nStripHit,0.);
    rs_nPixelHitPerTrackCandidateVsEta->Fill(nPixelHit,0.);
    rs_nTIBHitPerTrackCandidateVsEta->Fill(nTIBHit,0.);
    rs_nTOBHitPerTrackCandidateVsEta->Fill(nTOBHit,0.);
    rs_nTIDHitPerTrackCandidateVsEta->Fill(nTIDHit,0.);
    rs_nTECHitPerTrackCandidateVsEta->Fill(nTECHit,0.);
    rs_nPXBHitPerTrackCandidateVsEta->Fill(nPXBHit,0.);
    rs_nPXFHitPerTrackCandidateVsEta->Fill(nPXFHit,0.);

  }

  // Ckf tracks
  ckf_numTracks->Fill(ckfTrackCollection->size());
  for ( reco::TrackCollection::const_iterator track = ckfTrackCollection->begin();
	track != ckfTrackCollection->end();
	++track ) {
    ckf_chi2->Fill(track->normalizedChi2());
    ckf_pt->Fill(track->pt());
    ckf_nhit->Fill(track->found());
    ckf_eta->Fill(track->momentum().eta());
    ckf_phi->Fill(track->momentum().phi());
    ckf_transCurv->Fill(track->transverseCurvature());
    ckf_phiZero->Fill(track->phi0());
    ckf_theta->Fill(track->theta());
    ckf_dZero->Fill(track->d0());
    ckf_dZ->Fill(track->dz());

    // loop over hits in tracks, count
    unsigned int nHit      = 0;
    unsigned int nStripHit = 0;
    unsigned int nPixelHit = 0;
    unsigned int nTIBHit   = 0;
    unsigned int nTOBHit   = 0;
    unsigned int nTIDHit   = 0;
    unsigned int nTECHit   = 0;
    unsigned int nPXBHit   = 0;
    unsigned int nPXFHit   = 0;

    for ( trackingRecHit_iterator recHit = track->recHitsBegin();
	  recHit != track->recHitsEnd();
	  ++ recHit ) {
      ++nHit;
      DetId id((*recHit)->geographicalId());

      if ( (unsigned int)id.subdetId() == StripSubdetector::TIB ) {
	++nStripHit;
	++nTIBHit;
      } else if ( (unsigned int)id.subdetId() == StripSubdetector::TOB ) {
	++nStripHit;
	++nTOBHit;
      } else if ( (unsigned int)id.subdetId() == StripSubdetector::TID ) {
	++nStripHit;
	++nTIDHit;
      } else if ( (unsigned int)id.subdetId() == StripSubdetector::TEC ) {
	++nStripHit;
	++nTECHit;
      } else if ( (unsigned int)id.subdetId() == PixelSubdetector::PixelBarrel ) {
	++nPixelHit;
	++nPXBHit;
      } else if ( (unsigned int)id.subdetId() == PixelSubdetector::PixelEndcap ) {
	++nPixelHit;
	++nPXFHit;
      }
    }
    
    ckf_nHitPerTrackVsEta->Fill(nHit,track->momentum().eta());
    ckf_nStripHitPerTrackVsEta->Fill(nStripHit,track->momentum().eta());
    ckf_nPixelHitPerTrackVsEta->Fill(nPixelHit,track->momentum().eta());
    ckf_nTIBHitPerTrackVsEta->Fill(nTIBHit,track->momentum().eta());
    ckf_nTOBHitPerTrackVsEta->Fill(nTOBHit,track->momentum().eta());
    ckf_nTIDHitPerTrackVsEta->Fill(nTIDHit,track->momentum().eta());
    ckf_nTECHitPerTrackVsEta->Fill(nTECHit,track->momentum().eta());
    ckf_nPXBHitPerTrackVsEta->Fill(nPXBHit,track->momentum().eta());
    ckf_nPXFHitPerTrackVsEta->Fill(nPXFHit,track->momentum().eta());
    ckf_nHitPerTrackVsPhi->Fill(nHit,track->momentum().phi());

  }

  ckf_numSeeds->Fill(ckfSeedCollection->size());

  // CKF track candidates
  ckf_numTrackCandidates->Fill(ckfTrackCandidateCollection->size());
  for ( TrackCandidateCollection::const_iterator candidate = ckfTrackCandidateCollection->begin();
	candidate != ckfTrackCandidateCollection->end();
	++candidate ) {

    // loop over hits in tracks, count
    unsigned int nHit      = 0;
    unsigned int nStripHit = 0;
    unsigned int nPixelHit = 0;
    unsigned int nTIBHit   = 0;
    unsigned int nTOBHit   = 0;
    unsigned int nTIDHit   = 0;
    unsigned int nTECHit   = 0;
    unsigned int nPXBHit   = 0;
    unsigned int nPXFHit   = 0;

    for ( TrackCandidate::RecHitContainer::const_iterator recHit = candidate->recHits().first;
	  recHit != candidate->recHits().second;
	  ++recHit ) {
      ++nHit;
      DetId id(recHit->geographicalId());
      
      if ( (unsigned int)id.subdetId() == StripSubdetector::TIB ) {
	++nStripHit;
	++nTIBHit;
      } else if ( (unsigned int)id.subdetId() == StripSubdetector::TOB ) {
	++nStripHit;
	++nTOBHit;
      } else if ( (unsigned int)id.subdetId() == StripSubdetector::TID ) {
	++nStripHit;
	++nTIDHit;
      } else if ( (unsigned int)id.subdetId() == StripSubdetector::TEC ) {
	++nStripHit;
	++nTECHit;
      } else if ( (unsigned int)id.subdetId() == PixelSubdetector::PixelBarrel ) {
	++nPixelHit;
	++nPXBHit;
      } else if ( (unsigned int)id.subdetId() == PixelSubdetector::PixelEndcap ) {
	++nPixelHit;
	++nPXFHit;
      }
    }
    
    ckf_nHitPerTrackCandidateVsEta->Fill(nHit,0.);
    ckf_nStripHitPerTrackCandidateVsEta->Fill(nStripHit,0.);
    ckf_nPixelHitPerTrackCandidateVsEta->Fill(nPixelHit,0.);
    ckf_nTIBHitPerTrackCandidateVsEta->Fill(nTIBHit,0.);
    ckf_nTOBHitPerTrackCandidateVsEta->Fill(nTOBHit,0.);
    ckf_nTIDHitPerTrackCandidateVsEta->Fill(nTIDHit,0.);
    ckf_nTECHitPerTrackCandidateVsEta->Fill(nTECHit,0.);
    ckf_nPXBHitPerTrackCandidateVsEta->Fill(nPXBHit,0.);
    ckf_nPXFHitPerTrackCandidateVsEta->Fill(nPXFHit,0.);

  }

  if ( (ckfTrackCollection->size() == sameNumberOfTracks) &&
       (rsTrackCollection->size() == sameNumberOfTracks ) ) {
  // RS tracks
  rs_same_numTracks->Fill(rsTrackCollection->size());
  for ( reco::TrackCollection::const_iterator track = rsTrackCollection->begin();
	track != rsTrackCollection->end();
	++track ) {
    rs_same_chi2->Fill(track->normalizedChi2());
    rs_same_pt->Fill(track->pt());
    rs_same_nhit->Fill(track->found());
    rs_same_eta->Fill(track->momentum().eta());
    rs_same_phi->Fill(track->momentum().phi());
    rs_same_transCurv->Fill(track->transverseCurvature());
    rs_same_phiZero->Fill(track->phi0());
    rs_same_theta->Fill(track->theta());
    rs_same_dZero->Fill(track->d0());
    rs_same_dZ->Fill(track->dz());

    // loop over hits in tracks, count
    unsigned int nHit      = 0;
    unsigned int nStripHit = 0;
    unsigned int nPixelHit = 0;
    unsigned int nTIBHit   = 0;
    unsigned int nTOBHit   = 0;
    unsigned int nTIDHit   = 0;
    unsigned int nTECHit   = 0;
    unsigned int nPXBHit   = 0;
    unsigned int nPXFHit   = 0;

    for ( trackingRecHit_iterator recHit = track->recHitsBegin();
	  recHit != track->recHitsEnd();
	  ++ recHit ) {
      ++nHit;
      DetId id((*recHit)->geographicalId());

      if ( (unsigned int)id.subdetId() == StripSubdetector::TIB ) {
	++nStripHit;
	++nTIBHit;
      } else if ( (unsigned int)id.subdetId() == StripSubdetector::TOB ) {
	++nStripHit;
	++nTOBHit;
      } else if ( (unsigned int)id.subdetId() == StripSubdetector::TID ) {
	++nStripHit;
	++nTIDHit;
      } else if ( (unsigned int)id.subdetId() == StripSubdetector::TEC ) {
	++nStripHit;
	++nTECHit;
      } else if ( (unsigned int)id.subdetId() == PixelSubdetector::PixelBarrel ) {
	++nPixelHit;
	++nPXBHit;
      } else if ( (unsigned int)id.subdetId() == PixelSubdetector::PixelEndcap ) {
	++nPixelHit;
	++nPXFHit;
      }
    }
    
    rs_same_nHitPerTrackVsEta->Fill(nHit,track->momentum().eta());
    rs_same_nStripHitPerTrackVsEta->Fill(nStripHit,track->momentum().eta());
    rs_same_nPixelHitPerTrackVsEta->Fill(nPixelHit,track->momentum().eta());
    rs_same_nTIBHitPerTrackVsEta->Fill(nTIBHit,track->momentum().eta());
    rs_same_nTOBHitPerTrackVsEta->Fill(nTOBHit,track->momentum().eta());
    rs_same_nTIDHitPerTrackVsEta->Fill(nTIDHit,track->momentum().eta());
    rs_same_nTECHitPerTrackVsEta->Fill(nTECHit,track->momentum().eta());
    rs_same_nPXBHitPerTrackVsEta->Fill(nPXBHit,track->momentum().eta());
    rs_same_nPXFHitPerTrackVsEta->Fill(nPXFHit,track->momentum().eta());

  }

  // RS seeds, check for doubled seeds, deactivated
  //   rs_same_numSeeds->Fill(rsSeedCollection->size());
  //   for ( TrajectorySeedCollection::const_iterator seed1 = rsSeedCollection->begin();
  // 	seed1 != rsSeedCollection->end();
  // 	++seed1 ) {

  //     for ( TrajectorySeedCollection::const_iterator seed2 = seed1+1;
  // 	  seed2 != rsSeedCollection->end();
  // 	  ++seed2 ) {

  //       unsigned int counter = 0;
  //       for (TrajectorySeed::const_iterator hit1 = seed1->recHits().first;
  // 	   hit1 != seed1->recHits().second;
  // 	   ++hit1 ) {
  // 	for (TrajectorySeed::const_iterator hit2 = seed2->recHits().first;
  // 	     hit2 != seed2->recHits().second;
  // 	     ++hit2 ) {
  // 	  if (hit1->geographicalId() == hit2->geographicalId())
  // 	    if (hit1->localPosition().x() == hit2->localPosition().x())
  // 	      if (hit1->localPosition().y() == hit2->localPosition().y())
  // 		++counter;
  // 	}
  //       }
  //       if ( counter > 1 ) {
  // 	edm::LogVerbatim("RoadSearch") << "two seeds with the same hits";
  // 	edm::LogVerbatim("RoadSearch") << " Seed 1 hit 1 id: " << seed1->recHits().first->geographicalId().rawId()
  // 				       << " x: " << seed1->recHits().first->localPosition().x()
  // 				       << " y: " << seed1->recHits().first->localPosition().y()
  // 				       << " hit 2 id: " << (--(seed1->recHits().second))->geographicalId().rawId()
  // 				       << " x: " << (--(seed1->recHits().second))->localPosition().x()
  // 				       << " y: " << (--(seed1->recHits().second))->localPosition().y();
  // 	edm::LogVerbatim("RoadSearch") << " Seed 2 hit 1 id: " << seed2->recHits().first->geographicalId().rawId()
  // 				       << " x: " << seed2->recHits().first->localPosition().x()
  // 				       << " y: " << seed2->recHits().first->localPosition().y()
  // 				       << " hit 2 id: " << (--(seed2->recHits().second))->geographicalId().rawId()
  // 				       << " x: " << (--(seed2->recHits().second))->localPosition().x()
  // 				       << " y: " << (--(seed2->recHits().second))->localPosition().y();
  //       }
  //     }
  //   }


  // RS raw clouds
  rs_same_numRawClouds->Fill(rsRawCloudCollection->size());
  for ( RoadSearchCloudCollection::const_iterator cloud = rsRawCloudCollection->begin();
	cloud != rsRawCloudCollection->end();
	++cloud ) {

    // loop over hits in tracks, count
    unsigned int nHit      = 0;
    unsigned int nStripHit = 0;
    unsigned int nPixelHit = 0;
    unsigned int nTIBHit   = 0;
    unsigned int nTOBHit   = 0;
    unsigned int nTIDHit   = 0;
    unsigned int nTECHit   = 0;
    unsigned int nPXBHit   = 0;
    unsigned int nPXFHit   = 0;

    for ( RoadSearchCloud::RecHitOwnVector::const_iterator recHit = cloud->begin_hits();
	  recHit != cloud->end_hits();
	  ++recHit ) {
      ++nHit;
      DetId id(recHit->geographicalId());
      
      if ( (unsigned int)id.subdetId() == StripSubdetector::TIB ) {
	++nStripHit;
	++nTIBHit;
      } else if ( (unsigned int)id.subdetId() == StripSubdetector::TOB ) {
	++nStripHit;
	++nTOBHit;
      } else if ( (unsigned int)id.subdetId() == StripSubdetector::TID ) {
	++nStripHit;
	++nTIDHit;
      } else if ( (unsigned int)id.subdetId() == StripSubdetector::TEC ) {
	++nStripHit;
	++nTECHit;
      } else if ( (unsigned int)id.subdetId() == PixelSubdetector::PixelBarrel ) {
	++nPixelHit;
	++nPXBHit;
      } else if ( (unsigned int)id.subdetId() == PixelSubdetector::PixelEndcap ) {
	++nPixelHit;
	++nPXFHit;
      }
    }
    
    rs_same_nHitPerRawCloudVsEta->Fill(nHit,0.);
    rs_same_nStripHitPerRawCloudVsEta->Fill(nStripHit,0.);
    rs_same_nPixelHitPerRawCloudVsEta->Fill(nPixelHit,0.);
    rs_same_nTIBHitPerRawCloudVsEta->Fill(nTIBHit,0.);
    rs_same_nTOBHitPerRawCloudVsEta->Fill(nTOBHit,0.);
    rs_same_nTIDHitPerRawCloudVsEta->Fill(nTIDHit,0.);
    rs_same_nTECHitPerRawCloudVsEta->Fill(nTECHit,0.);
    rs_same_nPXBHitPerRawCloudVsEta->Fill(nPXBHit,0.);
    rs_same_nPXFHitPerRawCloudVsEta->Fill(nPXFHit,0.);

  }

  // RS clean clouds
  rs_same_numCleanClouds->Fill(rsCleanCloudCollection->size());
  for ( RoadSearchCloudCollection::const_iterator cloud = rsCleanCloudCollection->begin();
	cloud != rsCleanCloudCollection->end();
	++cloud ) {

    // loop over hits in tracks, count
    unsigned int nHit      = 0;
    unsigned int nStripHit = 0;
    unsigned int nPixelHit = 0;
    unsigned int nTIBHit   = 0;
    unsigned int nTOBHit   = 0;
    unsigned int nTIDHit   = 0;
    unsigned int nTECHit   = 0;
    unsigned int nPXBHit   = 0;
    unsigned int nPXFHit   = 0;

    for ( RoadSearchCloud::RecHitOwnVector::const_iterator recHit = cloud->begin_hits();
	  recHit != cloud->end_hits();
	  ++recHit ) {
      ++nHit;
      DetId id(recHit->geographicalId());
      
      if ( (unsigned int)id.subdetId() == StripSubdetector::TIB ) {
	++nStripHit;
	++nTIBHit;
      } else if ( (unsigned int)id.subdetId() == StripSubdetector::TOB ) {
	++nStripHit;
	++nTOBHit;
      } else if ( (unsigned int)id.subdetId() == StripSubdetector::TID ) {
	++nStripHit;
	++nTIDHit;
      } else if ( (unsigned int)id.subdetId() == StripSubdetector::TEC ) {
	++nStripHit;
	++nTECHit;
      } else if ( (unsigned int)id.subdetId() == PixelSubdetector::PixelBarrel ) {
	++nPixelHit;
	++nPXBHit;
      } else if ( (unsigned int)id.subdetId() == PixelSubdetector::PixelEndcap ) {
	++nPixelHit;
	++nPXFHit;
      }
    }
    
    rs_same_nHitPerCleanCloudVsEta->Fill(nHit,0.);
    rs_same_nStripHitPerCleanCloudVsEta->Fill(nStripHit,0.);
    rs_same_nPixelHitPerCleanCloudVsEta->Fill(nPixelHit,0.);
    rs_same_nTIBHitPerCleanCloudVsEta->Fill(nTIBHit,0.);
    rs_same_nTOBHitPerCleanCloudVsEta->Fill(nTOBHit,0.);
    rs_same_nTIDHitPerCleanCloudVsEta->Fill(nTIDHit,0.);
    rs_same_nTECHitPerCleanCloudVsEta->Fill(nTECHit,0.);
    rs_same_nPXBHitPerCleanCloudVsEta->Fill(nPXBHit,0.);
    rs_same_nPXFHitPerCleanCloudVsEta->Fill(nPXFHit,0.);

  }

  // RS track candidates
  rs_same_numTrackCandidates->Fill(rsTrackCandidateCollection->size());
  for ( TrackCandidateCollection::const_iterator candidate = rsTrackCandidateCollection->begin();
	candidate != rsTrackCandidateCollection->end();
	++candidate ) {

    // loop over hits in tracks, count
    unsigned int nHit      = 0;
    unsigned int nStripHit = 0;
    unsigned int nPixelHit = 0;
    unsigned int nTIBHit   = 0;
    unsigned int nTOBHit   = 0;
    unsigned int nTIDHit   = 0;
    unsigned int nTECHit   = 0;
    unsigned int nPXBHit   = 0;
    unsigned int nPXFHit   = 0;

    for ( TrackCandidate::RecHitContainer::const_iterator recHit = candidate->recHits().first;
	  recHit != candidate->recHits().second;
	  ++recHit ) {
      ++nHit;
      DetId id(recHit->geographicalId());
      
      if ( (unsigned int)id.subdetId() == StripSubdetector::TIB ) {
	++nStripHit;
	++nTIBHit;
      } else if ( (unsigned int)id.subdetId() == StripSubdetector::TOB ) {
	++nStripHit;
	++nTOBHit;
      } else if ( (unsigned int)id.subdetId() == StripSubdetector::TID ) {
	++nStripHit;
	++nTIDHit;
      } else if ( (unsigned int)id.subdetId() == StripSubdetector::TEC ) {
	++nStripHit;
	++nTECHit;
      } else if ( (unsigned int)id.subdetId() == PixelSubdetector::PixelBarrel ) {
	++nPixelHit;
	++nPXBHit;
      } else if ( (unsigned int)id.subdetId() == PixelSubdetector::PixelEndcap ) {
	++nPixelHit;
	++nPXFHit;
      }
    }
    
    rs_same_nHitPerTrackCandidateVsEta->Fill(nHit,0.);
    rs_same_nStripHitPerTrackCandidateVsEta->Fill(nStripHit,0.);
    rs_same_nPixelHitPerTrackCandidateVsEta->Fill(nPixelHit,0.);
    rs_same_nTIBHitPerTrackCandidateVsEta->Fill(nTIBHit,0.);
    rs_same_nTOBHitPerTrackCandidateVsEta->Fill(nTOBHit,0.);
    rs_same_nTIDHitPerTrackCandidateVsEta->Fill(nTIDHit,0.);
    rs_same_nTECHitPerTrackCandidateVsEta->Fill(nTECHit,0.);
    rs_same_nPXBHitPerTrackCandidateVsEta->Fill(nPXBHit,0.);
    rs_same_nPXFHitPerTrackCandidateVsEta->Fill(nPXFHit,0.);

  }

  // Ckf tracks
  ckf_same_numTracks->Fill(ckfTrackCollection->size());
  for ( reco::TrackCollection::const_iterator track = ckfTrackCollection->begin();
	track != ckfTrackCollection->end();
	++track ) {
    ckf_same_chi2->Fill(track->normalizedChi2());
    ckf_same_pt->Fill(track->pt());
    ckf_same_nhit->Fill(track->found());
    ckf_same_eta->Fill(track->momentum().eta());
    ckf_same_phi->Fill(track->momentum().phi());
    ckf_same_transCurv->Fill(track->transverseCurvature());
    ckf_same_phiZero->Fill(track->phi0());
    ckf_same_theta->Fill(track->theta());
    ckf_same_dZero->Fill(track->d0());
    ckf_same_dZ->Fill(track->dz());

    // loop over hits in tracks, count
    unsigned int nHit      = 0;
    unsigned int nStripHit = 0;
    unsigned int nPixelHit = 0;
    unsigned int nTIBHit   = 0;
    unsigned int nTOBHit   = 0;
    unsigned int nTIDHit   = 0;
    unsigned int nTECHit   = 0;
    unsigned int nPXBHit   = 0;
    unsigned int nPXFHit   = 0;

    for ( trackingRecHit_iterator recHit = track->recHitsBegin();
	  recHit != track->recHitsEnd();
	  ++ recHit ) {
      ++nHit;
      DetId id((*recHit)->geographicalId());

      if ( (unsigned int)id.subdetId() == StripSubdetector::TIB ) {
	++nStripHit;
	++nTIBHit;
      } else if ( (unsigned int)id.subdetId() == StripSubdetector::TOB ) {
	++nStripHit;
	++nTOBHit;
      } else if ( (unsigned int)id.subdetId() == StripSubdetector::TID ) {
	++nStripHit;
	++nTIDHit;
      } else if ( (unsigned int)id.subdetId() == StripSubdetector::TEC ) {
	++nStripHit;
	++nTECHit;
      } else if ( (unsigned int)id.subdetId() == PixelSubdetector::PixelBarrel ) {
	++nPixelHit;
	++nPXBHit;
      } else if ( (unsigned int)id.subdetId() == PixelSubdetector::PixelEndcap ) {
	++nPixelHit;
	++nPXFHit;
      }
    }
    
    ckf_same_nHitPerTrackVsEta->Fill(nHit,track->momentum().eta());
    ckf_same_nStripHitPerTrackVsEta->Fill(nStripHit,track->momentum().eta());
    ckf_same_nPixelHitPerTrackVsEta->Fill(nPixelHit,track->momentum().eta());
    ckf_same_nTIBHitPerTrackVsEta->Fill(nTIBHit,track->momentum().eta());
    ckf_same_nTOBHitPerTrackVsEta->Fill(nTOBHit,track->momentum().eta());
    ckf_same_nTIDHitPerTrackVsEta->Fill(nTIDHit,track->momentum().eta());
    ckf_same_nTECHitPerTrackVsEta->Fill(nTECHit,track->momentum().eta());
    ckf_same_nPXBHitPerTrackVsEta->Fill(nPXBHit,track->momentum().eta());
    ckf_same_nPXFHitPerTrackVsEta->Fill(nPXFHit,track->momentum().eta());

  }

  ckf_same_numSeeds->Fill(ckfSeedCollection->size());

  // CKF track candidates
  ckf_same_numTrackCandidates->Fill(ckfTrackCandidateCollection->size());
  for ( TrackCandidateCollection::const_iterator candidate = ckfTrackCandidateCollection->begin();
	candidate != ckfTrackCandidateCollection->end();
	++candidate ) {

    // loop over hits in tracks, count
    unsigned int nHit      = 0;
    unsigned int nStripHit = 0;
    unsigned int nPixelHit = 0;
    unsigned int nTIBHit   = 0;
    unsigned int nTOBHit   = 0;
    unsigned int nTIDHit   = 0;
    unsigned int nTECHit   = 0;
    unsigned int nPXBHit   = 0;
    unsigned int nPXFHit   = 0;

    for ( TrackCandidate::RecHitContainer::const_iterator recHit = candidate->recHits().first;
	  recHit != candidate->recHits().second;
	  ++recHit ) {
      ++nHit;
      DetId id(recHit->geographicalId());
      
      if ( (unsigned int)id.subdetId() == StripSubdetector::TIB ) {
	++nStripHit;
	++nTIBHit;
      } else if ( (unsigned int)id.subdetId() == StripSubdetector::TOB ) {
	++nStripHit;
	++nTOBHit;
      } else if ( (unsigned int)id.subdetId() == StripSubdetector::TID ) {
	++nStripHit;
	++nTIDHit;
      } else if ( (unsigned int)id.subdetId() == StripSubdetector::TEC ) {
	++nStripHit;
	++nTECHit;
      } else if ( (unsigned int)id.subdetId() == PixelSubdetector::PixelBarrel ) {
	++nPixelHit;
	++nPXBHit;
      } else if ( (unsigned int)id.subdetId() == PixelSubdetector::PixelEndcap ) {
	++nPixelHit;
	++nPXFHit;
      }
    }
    
    ckf_same_nHitPerTrackCandidateVsEta->Fill(nHit,0.);
    ckf_same_nStripHitPerTrackCandidateVsEta->Fill(nStripHit,0.);
    ckf_same_nPixelHitPerTrackCandidateVsEta->Fill(nPixelHit,0.);
    ckf_same_nTIBHitPerTrackCandidateVsEta->Fill(nTIBHit,0.);
    ckf_same_nTOBHitPerTrackCandidateVsEta->Fill(nTOBHit,0.);
    ckf_same_nTIDHitPerTrackCandidateVsEta->Fill(nTIDHit,0.);
    ckf_same_nTECHitPerTrackCandidateVsEta->Fill(nTECHit,0.);
    ckf_same_nPXBHitPerTrackCandidateVsEta->Fill(nPXBHit,0.);
    ckf_same_nPXFHitPerTrackCandidateVsEta->Fill(nPXFHit,0.);

  }

  }

}


void 
RoadSearchTrackValidation::beginJob(const edm::EventSetup&)
{
}

void 
RoadSearchTrackValidation::endJob() {
}
