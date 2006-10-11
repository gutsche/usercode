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
// $Date: 2006/10/06 14:42:31 $
// $Revision: 1.6 $
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

RoadSearchTrackValidation::RoadSearchTrackValidation(const edm::ParameterSet& iConfig) :
  histograms_(iConfig.getUntrackedParameter<std::string>("OutputFileName"))
{

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
  histograms_.bookHistogram("rs_chi2_1d","Track reduced chi^{2}","RS",chi2_nbins,chi2_low,chi2_high,"#chi^2","Events");
  histograms_.bookHistogram("rs_pt_1d","Track p_{T} (GeV)","RS",pt_nbins,pt_low,pt_high,"p_{T} [GeV]","Events");
  histograms_.bookHistogram("rs_eta_1d","Track #eta","RS",eta_nbins,eta_low,eta_high,"#eta","Events");
  histograms_.bookHistogram("rs_phi_1d","Track #phi","RS",phi_nbins,phi_low,phi_high,"#phi","Events");
  histograms_.bookHistogram("rs_transCurv_1d","Transverse curvature","RS",transCurv_nbins,transCurv_low,transCurv_high,"transCurv","");
  histograms_.bookHistogram("rs_phiZero_1d","phiZero","RS",phiZero_nbins,phiZero_low,phiZero_high,"#phi_{0}","Events");
  histograms_.bookHistogram("rs_theta_1d","Theta","RS",theta_nbins,theta_low,theta_high,"#theta","Events");
  histograms_.bookHistogram("rs_dZero_1d",",dZero","RS",dZero_nbins,dZero_low,dZero_high,"d_{0}","Events");
  histograms_.bookHistogram("rs_dZ_1d","dZ","RS",dZ_nbins,dZ_low,dZ_high,"d_{Z}","Events");
  histograms_.bookHistogram("rs_nhit_1d","Number of hits per track","RS",nhit_nbins,nhit_low,nhit_high,"n_{hit}","Events");

  histograms_.bookHistogram("rs_numSeeds_1d","Number of Seeds per event","RS",numSeeds_nbins,numSeeds_low,numSeeds_high,"n_{seeds}","Events");
  histograms_.bookHistogram("rs_numRawClouds_1d","Number of raw clouds per event","RS",numRawClouds_nbins,numRawClouds_low,numRawClouds_high,"n_{rawClouds}","Events");
  histograms_.bookHistogram("rs_numCleanClouds_1d","Number of clean clouds per event","RS",numCleanClouds_nbins,numCleanClouds_low,numCleanClouds_high,"n_{cleanClouds}","Events");
  histograms_.bookHistogram("rs_numTrackCandidates_1d","Number of track candidates per event","RS",numTrackCandidates_nbins,numTrackCandidates_low,numTrackCandidates_high,"n_{TrackCandidates}","Events");
  histograms_.bookHistogram("rs_numTracks_1d","Number of tracks per event","RS",numTracks_nbins,numTracks_low,numTracks_high,"n_{Tracks}","Events");

  histograms_.bookHistogram("rs_nHitPerTrackVsEta_2d","Number of hits per track vs. #eta","RS",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("rs_nHitPerTrackCandidateVsEta_2d","Number of hits per track candidate vs. #eta","RS",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("rs_nHitPerCleanCloudVsEta_2d","Number of hits per clean cloud vs. #eta","RS",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("rs_nHitPerRawCloudVsEta_2d","Number of hits per raw cloud vs. #eta","RS",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

  histograms_.bookHistogram("rs_nHitPerTrackVsPhi_2d","Number of hits per track vs. #phi","RS",nhit_nbins,nhit_low,nhit_high,phi_nbins,phi_low,phi_high,"n_{Hit}","#phi","Events");

  histograms_.bookHistogram("rs_nStripHitPerTrackVsEta_2d","Number of strip hits per track vs. #eta","RS",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("rs_nStripHitPerTrackCandidateVsEta_2d","Number of strip hits per track candidate vs. #eta","RS",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("rs_nStripHitPerCleanCloudVsEta_2d","Number of strip hits per clean cloud vs. #eta","RS",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("rs_nStripHitPerRawCloudVsEta_2d","Number of strip hits per raw cloud vs. #eta","RS",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

  histograms_.bookHistogram("rs_nPixelHitPerTrackVsEta_2d","Number of pixel hits per track vs. #eta","RS",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("rs_nPixelHitPerTrackCandidateVsEta_2d","Number of pixel hits per track candidate vs. #eta","RS",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("rs_nPixelHitPerCleanCloudVsEta_2d","Number of pixel hits per clean cloud vs. #eta","RS",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("rs_nPixelHitPerRawCloudVsEta_2d","Number of pixel hits per raw cloud vs. #eta","RS",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

  histograms_.bookHistogram("rs_nTIBHitPerTrackVsEta_2d","Number of TIB hits per track vs. #eta","RS",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("rs_nTIBHitPerTrackCandidateVsEta_2d","Number of TIB hits per track candidate vs. #eta","RS",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("rs_nTIBHitPerCleanCloudVsEta_2d","Number of TIB hits per clean cloud vs. #eta","RS",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("rs_nTIBHitPerRawCloudVsEta_2d","Number of TIB hits per raw cloud vs. #eta","RS",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

  histograms_.bookHistogram("rs_nTOBHitPerTrackVsEta_2d","Number of TOB hits per track vs. #eta","RS",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("rs_nTOBHitPerTrackCandidateVsEta_2d","Number of TOB hits per track candidate vs. #eta","RS",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("rs_nTOBHitPerCleanCloudVsEta_2d","Number of TOB hits per clean cloud vs. #eta","RS",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("rs_nTOBHitPerRawCloudVsEta_2d","Number of TOB hits per raw cloud vs. #eta","RS",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

  histograms_.bookHistogram("rs_nTIDHitPerTrackVsEta_2d","Number of TID hits per track vs. #eta","RS",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("rs_nTIDHitPerTrackCandidateVsEta_2d","Number of TID hits per track candidate vs. #eta","RS",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("rs_nTIDHitPerCleanCloudVsEta_2d","Number of TID hits per clean cloud vs. #eta","RS",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("rs_nTIDHitPerRawCloudVsEta_2d","Number of TID hits per raw cloud vs. #eta","RS",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

  histograms_.bookHistogram("rs_nTECHitPerTrackVsEta_2d","Number of TEC hits per track vs. #eta","RS",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("rs_nTECHitPerTrackCandidateVsEta_2d","Number of TEC hits per track candidate vs. #eta","RS",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("rs_nTECHitPerCleanCloudVsEta_2d","Number of TEC hits per clean cloud vs. #eta","RS",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("rs_nTECHitPerRawCloudVsEta_2d","Number of TEC hits per raw cloud vs. #eta","RS",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

  histograms_.bookHistogram("rs_nPXBHitPerTrackVsEta_2d","Number of PXB hits per track vs. #eta","RS",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("rs_nPXBHitPerTrackCandidateVsEta_2d","Number of PXB hits per track candidate vs. #eta","RS",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("rs_nPXBHitPerCleanCloudVsEta_2d","Number of PXB hits per clean cloud vs. #eta","RS",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("rs_nPXBHitPerRawCloudVsEta_2d","Number of PXB hits per raw cloud vs. #eta","RS",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

  histograms_.bookHistogram("rs_nPXFHitPerTrackVsEta_2d","Number of PXF hits per track vs. #eta","RS",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("rs_nPXFHitPerTrackCandidateVsEta_2d","Number of PXF hits per track candidate vs. #eta","RS",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("rs_nPXFHitPerCleanCloudVsEta_2d","Number of PXF hits per clean cloud vs. #eta","RS",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("rs_nPXFHitPerRawCloudVsEta_2d","Number of PXF hits per raw cloud vs. #eta","RS",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

  // CKF plots
  histograms_.bookHistogram("ckf_chi2_1d","Track reduced chi^{2}","CKF",chi2_nbins,chi2_low,chi2_high,"#chi^2","Events");
  histograms_.bookHistogram("ckf_pt_1d","Track p_{T} (GeV)","CKF",pt_nbins,pt_low,pt_high,"p_{T} [GeV]","Events");
  histograms_.bookHistogram("ckf_eta_1d","Track #eta","CKF",eta_nbins,eta_low,eta_high,"#eta","Events");
  histograms_.bookHistogram("ckf_phi_1d","Track #phi","CKF",phi_nbins,phi_low,phi_high,"#phi","Events");
  histograms_.bookHistogram("ckf_transCurv_1d","Transveckfe curvature","CKF",transCurv_nbins,transCurv_low,transCurv_high,"transCurv","");
  histograms_.bookHistogram("ckf_phiZero_1d","phiZero","CKF",phiZero_nbins,phiZero_low,phiZero_high,"#phi_{0}","Events");
  histograms_.bookHistogram("ckf_theta_1d","Theta","CKF",theta_nbins,theta_low,theta_high,"#theta","Events");
  histograms_.bookHistogram("ckf_dZero_1d",",dZero","CKF",dZero_nbins,dZero_low,dZero_high,"d_{0}","Events");
  histograms_.bookHistogram("ckf_dZ_1d","dZ","CKF",dZ_nbins,dZ_low,dZ_high,"d_{Z}","Events");
  histograms_.bookHistogram("ckf_nhit_1d","Number of hits per track","CKF",nhit_nbins,nhit_low,nhit_high,"n_{hit}","Events");

  histograms_.bookHistogram("ckf_numSeeds_1d","Number of Seeds per event","CKF",numSeeds_nbins,numSeeds_low,numSeeds_high,"n_{seeds}","Events");
  histograms_.bookHistogram("ckf_numTrackCandidates_1d","Number of track candidates per event","CKF",numTrackCandidates_nbins,numTrackCandidates_low,numTrackCandidates_high,"n_{TrackCandidates}","Events");
  histograms_.bookHistogram("ckf_numTracks_1d","Number of tracks per event","CKF",numTracks_nbins,numTracks_low,numTracks_high,"n_{Tracks}","Events");

  histograms_.bookHistogram("ckf_nHitPerTrackVsEta_2d","Number of hits per track vs. #eta","CKF",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("ckf_nHitPerTrackCandidateVsEta_2d","Number of hits per track candidate vs. #eta","CKF",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

  histograms_.bookHistogram("ckf_nHitPerTrackVsPhi_2d","Number of hits per track vs. #phi","CKF",nhit_nbins,nhit_low,nhit_high,phi_nbins,phi_low,phi_high,"n_{Hit}","#phi","Events");

  histograms_.bookHistogram("ckf_nStripHitPerTrackVsEta_2d","Number of strip hits per track vs. #eta","CKF",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("ckf_nStripHitPerTrackCandidateVsEta_2d","Number of strip hits per track candidate vs. #eta","CKF",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

  histograms_.bookHistogram("ckf_nPixelHitPerTrackVsEta_2d","Number of pixel hits per track vs. #eta","CKF",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("ckf_nPixelHitPerTrackCandidateVsEta_2d","Number of pixel hits per track candidate vs. #eta","CKF",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

  histograms_.bookHistogram("ckf_nTIBHitPerTrackVsEta_2d","Number of TIB hits per track vs. #eta","CKF",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("ckf_nTIBHitPerTrackCandidateVsEta_2d","Number of TIB hits per track candidate vs. #eta","CKF",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

  histograms_.bookHistogram("ckf_nTOBHitPerTrackVsEta_2d","Number of TOB hits per track vs. #eta","CKF",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("ckf_nTOBHitPerTrackCandidateVsEta_2d","Number of TOB hits per track candidate vs. #eta","CKF",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

  histograms_.bookHistogram("ckf_nTIDHitPerTrackVsEta_2d","Number of TID hits per track vs. #eta","CKF",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("ckf_nTIDHitPerTrackCandidateVsEta_2d","Number of TID hits per track candidate vs. #eta","CKF",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

  histograms_.bookHistogram("ckf_nTECHitPerTrackVsEta_2d","Number of TEC hits per track vs. #eta","CKF",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("ckf_nTECHitPerTrackCandidateVsEta_2d","Number of TEC hits per track candidate vs. #eta","CKF",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

  histograms_.bookHistogram("ckf_nPXBHitPerTrackVsEta_2d","Number of PXB hits per track vs. #eta","CKF",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("ckf_nPXBHitPerTrackCandidateVsEta_2d","Number of PXB hits per track candidate vs. #eta","CKF",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

  histograms_.bookHistogram("ckf_nPXFHitPerTrackVsEta_2d","Number of PXF hits per track vs. #eta","CKF",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("ckf_nPXFHitPerTrackCandidateVsEta_2d","Number of PXF hits per track candidate vs. #eta","CKF",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

  // RS_SAME plots, both tracking algorithms have the same number of tracks steered by cards
  histograms_.bookHistogram("rs_same_chi2_1d","Track reduced chi^{2}","RS_SAME",chi2_nbins,chi2_low,chi2_high,"#chi^2","Events");
  histograms_.bookHistogram("rs_same_pt_1d","Track p_{T} (GeV)","RS_SAME",pt_nbins,pt_low,pt_high,"p_{T} [GeV]","Events");
  histograms_.bookHistogram("rs_same_eta_1d","Track #eta","RS_SAME",eta_nbins,eta_low,eta_high,"#eta","Events");
  histograms_.bookHistogram("rs_same_phi_1d","Track #phi","RS_SAME",phi_nbins,phi_low,phi_high,"#phi","Events");
  histograms_.bookHistogram("rs_same_transCurv_1d","Transvers_samee curvature","RS_SAME",transCurv_nbins,transCurv_low,transCurv_high,"transCurv","");
  histograms_.bookHistogram("rs_same_phiZero_1d","phiZero","RS_SAME",phiZero_nbins,phiZero_low,phiZero_high,"#phi_{0}","Events");
  histograms_.bookHistogram("rs_same_theta_1d","Theta","RS_SAME",theta_nbins,theta_low,theta_high,"#theta","Events");
  histograms_.bookHistogram("rs_same_dZero_1d",",dZero","RS_SAME",dZero_nbins,dZero_low,dZero_high,"d_{0}","Events");
  histograms_.bookHistogram("rs_same_dZ_1d","dZ","RS_SAME",dZ_nbins,dZ_low,dZ_high,"d_{Z}","Events");
  histograms_.bookHistogram("rs_same_nhit_1d","Number of hits per track","RS_SAME",nhit_nbins,nhit_low,nhit_high,"n_{hit}","Events");

  histograms_.bookHistogram("rs_same_numSeeds_1d","Number of Seeds per event","RS_SAME",numSeeds_nbins,numSeeds_low,numSeeds_high,"n_{seeds}","Events");
  histograms_.bookHistogram("rs_same_numRawClouds_1d","Number of raw clouds per event","RS_SAME",numRawClouds_nbins,numRawClouds_low,numRawClouds_high,"n_{rawClouds}","Events");
  histograms_.bookHistogram("rs_same_numCleanClouds_1d","Number of clean clouds per event","RS_SAME",numCleanClouds_nbins,numCleanClouds_low,numCleanClouds_high,"n_{cleanClouds}","Events");
  histograms_.bookHistogram("rs_same_numTrackCandidates_1d","Number of track candidates per event","RS_SAME",numTrackCandidates_nbins,numTrackCandidates_low,numTrackCandidates_high,"n_{TrackCandidates}","Events");
  histograms_.bookHistogram("rs_same_numTracks_1d","Number of tracks per event","RS_SAME",numTracks_nbins,numTracks_low,numTracks_high,"n_{Tracks}","Events");

  histograms_.bookHistogram("rs_same_nHitPerTrackVsEta_2d","Number of hits per track vs. #eta","RS_SAME",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("rs_same_nHitPerTrackCandidateVsEta_2d","Number of hits per track candidate vs. #eta","RS_SAME",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("rs_same_nHitPerCleanCloudVsEta_2d","Number of hits per clean cloud vs. #eta","RS_SAME",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("rs_same_nHitPerRawCloudVsEta_2d","Number of hits per raw cloud vs. #eta","RS_SAME",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

  histograms_.bookHistogram("rs_same_nHitPerTrackVsPhi_2d","Number of hits per track vs. #phi","RS_SAME",nhit_nbins,nhit_low,nhit_high,phi_nbins,phi_low,phi_high,"n_{Hit}","#phi","Events");

  histograms_.bookHistogram("rs_same_nStripHitPerTrackVsEta_2d","Number of strip hits per track vs. #eta","RS_SAME",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("rs_same_nStripHitPerTrackCandidateVsEta_2d","Number of strip hits per track candidate vs. #eta","RS_SAME",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("rs_same_nStripHitPerCleanCloudVsEta_2d","Number of strip hits per clean cloud vs. #eta","RS_SAME",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("rs_same_nStripHitPerRawCloudVsEta_2d","Number of strip hits per raw cloud vs. #eta","RS_SAME",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

  histograms_.bookHistogram("rs_same_nPixelHitPerTrackVsEta_2d","Number of pixel hits per track vs. #eta","RS_SAME",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("rs_same_nPixelHitPerTrackCandidateVsEta_2d","Number of pixel hits per track candidate vs. #eta","RS_SAME",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("rs_same_nPixelHitPerCleanCloudVsEta_2d","Number of pixel hits per clean cloud vs. #eta","RS_SAME",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("rs_same_nPixelHitPerRawCloudVsEta_2d","Number of pixel hits per raw cloud vs. #eta","RS_SAME",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

  histograms_.bookHistogram("rs_same_nTIBHitPerTrackVsEta_2d","Number of TIB hits per track vs. #eta","RS_SAME",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("rs_same_nTIBHitPerTrackCandidateVsEta_2d","Number of TIB hits per track candidate vs. #eta","RS_SAME",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("rs_same_nTIBHitPerCleanCloudVsEta_2d","Number of TIB hits per clean cloud vs. #eta","RS_SAME",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("rs_same_nTIBHitPerRawCloudVsEta_2d","Number of TIB hits per raw cloud vs. #eta","RS_SAME",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

  histograms_.bookHistogram("rs_same_nTOBHitPerTrackVsEta_2d","Number of TOB hits per track vs. #eta","RS_SAME",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("rs_same_nTOBHitPerTrackCandidateVsEta_2d","Number of TOB hits per track candidate vs. #eta","RS_SAME",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("rs_same_nTOBHitPerCleanCloudVsEta_2d","Number of TOB hits per clean cloud vs. #eta","RS_SAME",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("rs_same_nTOBHitPerRawCloudVsEta_2d","Number of TOB hits per raw cloud vs. #eta","RS_SAME",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

  histograms_.bookHistogram("rs_same_nTIDHitPerTrackVsEta_2d","Number of TID hits per track vs. #eta","RS_SAME",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("rs_same_nTIDHitPerTrackCandidateVsEta_2d","Number of TID hits per track candidate vs. #eta","RS_SAME",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("rs_same_nTIDHitPerCleanCloudVsEta_2d","Number of TID hits per clean cloud vs. #eta","RS_SAME",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("rs_same_nTIDHitPerRawCloudVsEta_2d","Number of TID hits per raw cloud vs. #eta","RS_SAME",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

  histograms_.bookHistogram("rs_same_nTECHitPerTrackVsEta_2d","Number of TEC hits per track vs. #eta","RS_SAME",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("rs_same_nTECHitPerTrackCandidateVsEta_2d","Number of TEC hits per track candidate vs. #eta","RS_SAME",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("rs_same_nTECHitPerCleanCloudVsEta_2d","Number of TEC hits per clean cloud vs. #eta","RS_SAME",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("rs_same_nTECHitPerRawCloudVsEta_2d","Number of TEC hits per raw cloud vs. #eta","RS_SAME",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

  histograms_.bookHistogram("rs_same_nPXBHitPerTrackVsEta_2d","Number of PXB hits per track vs. #eta","RS_SAME",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("rs_same_nPXBHitPerTrackCandidateVsEta_2d","Number of PXB hits per track candidate vs. #eta","RS_SAME",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("rs_same_nPXBHitPerCleanCloudVsEta_2d","Number of PXB hits per clean cloud vs. #eta","RS_SAME",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("rs_same_nPXBHitPerRawCloudVsEta_2d","Number of PXB hits per raw cloud vs. #eta","RS_SAME",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

  histograms_.bookHistogram("rs_same_nPXFHitPerTrackVsEta_2d","Number of PXF hits per track vs. #eta","RS_SAME",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("rs_same_nPXFHitPerTrackCandidateVsEta_2d","Number of PXF hits per track candidate vs. #eta","RS_SAME",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("rs_same_nPXFHitPerCleanCloudVsEta_2d","Number of PXF hits per clean cloud vs. #eta","RS_SAME",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("rs_same_nPXFHitPerRawCloudVsEta_2d","Number of PXF hits per raw cloud vs. #eta","RS_SAME",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

  // CKF_SAME plots, both tracking algorithms have the same number of tracks steered by cards
  histograms_.bookHistogram("ckf_same_chi2_1d","Track reduced chi^{2}","CKF_SAME",chi2_nbins,chi2_low,chi2_high,"#chi^2","Events");
  histograms_.bookHistogram("ckf_same_pt_1d","Track p_{T} (GeV)","CKF_SAME",pt_nbins,pt_low,pt_high,"p_{T} [GeV]","Events");
  histograms_.bookHistogram("ckf_same_eta_1d","Track #eta","CKF_SAME",eta_nbins,eta_low,eta_high,"#eta","Events");
  histograms_.bookHistogram("ckf_same_phi_1d","Track #phi","CKF_SAME",phi_nbins,phi_low,phi_high,"#phi","Events");
  histograms_.bookHistogram("ckf_same_transCurv_1d","Transveckf_samee curvature","CKF_SAME",transCurv_nbins,transCurv_low,transCurv_high,"transCurv","");
  histograms_.bookHistogram("ckf_same_phiZero_1d","phiZero","CKF_SAME",phiZero_nbins,phiZero_low,phiZero_high,"#phi_{0}","Events");
  histograms_.bookHistogram("ckf_same_theta_1d","Theta","CKF_SAME",theta_nbins,theta_low,theta_high,"#theta","Events");
  histograms_.bookHistogram("ckf_same_dZero_1d",",dZero","CKF_SAME",dZero_nbins,dZero_low,dZero_high,"d_{0}","Events");
  histograms_.bookHistogram("ckf_same_dZ_1d","dZ","CKF_SAME",dZ_nbins,dZ_low,dZ_high,"d_{Z}","Events");
  histograms_.bookHistogram("ckf_same_nhit_1d","Number of hits per track","CKF_SAME",nhit_nbins,nhit_low,nhit_high,"n_{hit}","Events");

  histograms_.bookHistogram("ckf_same_numSeeds_1d","Number of Seeds per event","CKF_SAME",numSeeds_nbins,numSeeds_low,numSeeds_high,"n_{seeds}","Events");
  histograms_.bookHistogram("ckf_same_numTrackCandidates_1d","Number of track candidates per event","CKF_SAME",numTrackCandidates_nbins,numTrackCandidates_low,numTrackCandidates_high,"n_{TrackCandidates}","Events");
  histograms_.bookHistogram("ckf_same_numTracks_1d","Number of tracks per event","CKF_SAME",numTracks_nbins,numTracks_low,numTracks_high,"n_{Tracks}","Events");

  histograms_.bookHistogram("ckf_same_nHitPerTrackVsEta_2d","Number of hits per track vs. #eta","CKF_SAME",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("ckf_same_nHitPerTrackCandidateVsEta_2d","Number of hits per track candidate vs. #eta","CKF_SAME",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

  histograms_.bookHistogram("ckf_same_nHitPerTrackVsPhi_2d","Number of hits per track vs. #phi","CKF_SAME",nhit_nbins,nhit_low,nhit_high,phi_nbins,phi_low,phi_high,"n_{Hit}","#phi","Events");

  histograms_.bookHistogram("ckf_same_nStripHitPerTrackVsEta_2d","Number of strip hits per track vs. #eta","CKF_SAME",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("ckf_same_nStripHitPerTrackCandidateVsEta_2d","Number of strip hits per track candidate vs. #eta","CKF_SAME",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

  histograms_.bookHistogram("ckf_same_nPixelHitPerTrackVsEta_2d","Number of pixel hits per track vs. #eta","CKF_SAME",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("ckf_same_nPixelHitPerTrackCandidateVsEta_2d","Number of pixel hits per track candidate vs. #eta","CKF_SAME",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

  histograms_.bookHistogram("ckf_same_nTIBHitPerTrackVsEta_2d","Number of TIB hits per track vs. #eta","CKF_SAME",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("ckf_same_nTIBHitPerTrackCandidateVsEta_2d","Number of TIB hits per track candidate vs. #eta","CKF_SAME",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

  histograms_.bookHistogram("ckf_same_nTOBHitPerTrackVsEta_2d","Number of TOB hits per track vs. #eta","CKF_SAME",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("ckf_same_nTOBHitPerTrackCandidateVsEta_2d","Number of TOB hits per track candidate vs. #eta","CKF_SAME",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

  histograms_.bookHistogram("ckf_same_nTIDHitPerTrackVsEta_2d","Number of TID hits per track vs. #eta","CKF_SAME",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("ckf_same_nTIDHitPerTrackCandidateVsEta_2d","Number of TID hits per track candidate vs. #eta","CKF_SAME",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

  histograms_.bookHistogram("ckf_same_nTECHitPerTrackVsEta_2d","Number of TEC hits per track vs. #eta","CKF_SAME",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("ckf_same_nTECHitPerTrackCandidateVsEta_2d","Number of TEC hits per track candidate vs. #eta","CKF_SAME",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

  histograms_.bookHistogram("ckf_same_nPXBHitPerTrackVsEta_2d","Number of PXB hits per track vs. #eta","CKF_SAME",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("ckf_same_nPXBHitPerTrackCandidateVsEta_2d","Number of PXB hits per track candidate vs. #eta","CKF_SAME",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

  histograms_.bookHistogram("ckf_same_nPXFHitPerTrackVsEta_2d","Number of PXF hits per track vs. #eta","CKF_SAME",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  histograms_.bookHistogram("ckf_same_nPXFHitPerTrackCandidateVsEta_2d","Number of PXF hits per track candidate vs. #eta","CKF_SAME",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

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
 
}


void
RoadSearchTrackValidation::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  // get collections
  const TrajectorySeedCollection *rsSeedCollection = 0;
  try {
    edm::Handle<TrajectorySeedCollection> rsSeedCollectionHandle;
    iEvent.getByLabel(rsSeedProducerLabel_,rsSeedCollectionHandle);
    rsSeedCollection = rsSeedCollectionHandle.product();
  }
  catch (edm::Exception const& x) {
    if ( x.categoryCode() == edm::errors::ProductNotFound ) {
      if ( x.history().size() == 1 ) {
	rsSeedCollection = new TrajectorySeedCollection();
	edm::LogWarning("RoadSearchTrackValidation") << "Collection TrajectorySeedCollection with label " << rsSeedProducerLabel_ << " cannot be found, using empty collection of same type";
      }
    }
  }

  const RoadSearchCloudCollection *rsRawCloudCollection = 0;
  try {
    edm::Handle<RoadSearchCloudCollection> rsRawCloudCollectionHandle;
    iEvent.getByLabel(rsRawCloudProducerLabel_,rsRawCloudCollectionHandle);
    rsRawCloudCollection = rsRawCloudCollectionHandle.product();
  }
  catch (edm::Exception const& x) {
    if ( x.categoryCode() == edm::errors::ProductNotFound ) {
      if ( x.history().size() == 1 ) {
	rsRawCloudCollection = new RoadSearchCloudCollection();
	edm::LogWarning("RoadSearchTrackValidation") << "Collection RoadSearchCloudCollection with label " << rsRawCloudProducerLabel_ << " cannot be found, using empty collection of same type";
      }
    }
  }

  const RoadSearchCloudCollection *rsCleanCloudCollection = 0;
  try {
    edm::Handle<RoadSearchCloudCollection> rsCleanCloudCollectionHandle;
    iEvent.getByLabel(rsCleanCloudProducerLabel_,rsCleanCloudCollectionHandle);
    rsCleanCloudCollection = rsCleanCloudCollectionHandle.product();
  }
  catch (edm::Exception const& x) {
    if ( x.categoryCode() == edm::errors::ProductNotFound ) {
      if ( x.history().size() == 1 ) {
	rsCleanCloudCollection = new RoadSearchCloudCollection();
	edm::LogWarning("RoadSearchTrackValidation") << "Collection RoadSearchCloudCollection with label " << rsCleanCloudProducerLabel_ << " cannot be found, using empty collection of same type";
      }
    }
  }

  const TrackCandidateCollection *rsTrackCandidateCollection = 0;
  try {
    edm::Handle<TrackCandidateCollection> rsTrackCandidateCollectionHandle;
    iEvent.getByLabel(rsTrackCandidateProducerLabel_,rsTrackCandidateCollectionHandle);
    rsTrackCandidateCollection = rsTrackCandidateCollectionHandle.product();
  }
  catch (edm::Exception const& x) {
    if ( x.categoryCode() == edm::errors::ProductNotFound ) {
      if ( x.history().size() == 1 ) {
	rsTrackCandidateCollection = new TrackCandidateCollection();
	edm::LogWarning("RoadSearchTrackValidation") << "Collection TrackCandidateCollection with label " << rsTrackCandidateProducerLabel_ << " cannot be found, using empty collection of same type";
      }
    }
  }

  const reco::TrackCollection *rsTrackCollection = 0;
  try {
    edm::Handle<reco::TrackCollection> rsTrackCollectionHandle;
    iEvent.getByLabel(rsTrackProducerLabel_,rsTrackCollectionHandle);
    rsTrackCollection = rsTrackCollectionHandle.product();
  }
  catch (edm::Exception const& x) {
    if ( x.categoryCode() == edm::errors::ProductNotFound ) {
      if ( x.history().size() == 1 ) {
	rsTrackCollection = new reco::TrackCollection();
	edm::LogWarning("RoadSearchTrackValidation") << "Collection reco::TrackCollection with label " << rsTrackProducerLabel_ << " cannot be found, using empty collection of same type";
      }
    }
  }

  const TrajectorySeedCollection *ckfSeedCollection = 0;
  try {
    edm::Handle<TrajectorySeedCollection> ckfSeedCollectionHandle;
    iEvent.getByLabel(ckfSeedProducerLabel_,ckfSeedCollectionHandle);
    ckfSeedCollection = ckfSeedCollectionHandle.product();
  }
  catch (edm::Exception const& x) {
    if ( x.categoryCode() == edm::errors::ProductNotFound ) {
      if ( x.history().size() == 1 ) {
	ckfSeedCollection = new TrajectorySeedCollection();
	edm::LogWarning("RoadSearchTrackValidation") << "Collection TrajectorySeedCollection with label " << ckfSeedProducerLabel_ << " cannot be found, using empty collection of same type";
      }
    }
  }

  const TrackCandidateCollection *ckfTrackCandidateCollection = 0;
  try {
    edm::Handle<TrackCandidateCollection> ckfTrackCandidateCollectionHandle;
    iEvent.getByLabel(ckfTrackCandidateProducerLabel_,ckfTrackCandidateCollectionHandle);
    ckfTrackCandidateCollection = ckfTrackCandidateCollectionHandle.product();
  }
  catch (edm::Exception const& x) {
    if ( x.categoryCode() == edm::errors::ProductNotFound ) {
      if ( x.history().size() == 1 ) {
	ckfTrackCandidateCollection = new TrackCandidateCollection();
	edm::LogWarning("RoadSearchTrackValidation") << "Collection TrackCandidateCollection with label " << ckfTrackCandidateProducerLabel_ << " cannot be found, using empty collection of same type";
      }
    }
  }

  const reco::TrackCollection *ckfTrackCollection = 0;
  try {
    edm::Handle<reco::TrackCollection> ckfTrackCollectionHandle;
    iEvent.getByLabel(ckfTrackProducerLabel_,ckfTrackCollectionHandle);
    ckfTrackCollection = ckfTrackCollectionHandle.product();
  }
  catch (edm::Exception const& x) {
    if ( x.categoryCode() == edm::errors::ProductNotFound ) {
      if ( x.history().size() == 1 ) {
	ckfTrackCollection = new reco::TrackCollection();
	edm::LogWarning("RoadSearchTrackValidation") << "Collection reco::TrackCollection with label " << ckfTrackProducerLabel_ << " cannot be found, using empty collection of same type";
      }
    }
  }

  // fill histograms
  
  // RS tracks
  histograms_.fill("rs_numTracks_1d",rsTrackCollection->size());
  for ( reco::TrackCollection::const_iterator track = rsTrackCollection->begin();
	track != rsTrackCollection->end();
	++track ) {
    histograms_.fill("rs_chi2_1d",track->normalizedChi2());
    histograms_.fill("rs_pt_1d",track->pt());
    histograms_.fill("rs_nhit_1d",track->found());
    histograms_.fill("rs_eta_1d",track->momentum().eta());
    histograms_.fill("rs_phi_1d",track->momentum().phi());
    histograms_.fill("rs_transCurv_1d",track->transverseCurvature());
    histograms_.fill("rs_phiZero_1d",track->phi0());
    histograms_.fill("rs_theta_1d",track->theta());
    histograms_.fill("rs_dZero_1d",track->d0());
    histograms_.fill("rs_dZ_1d",track->dz());

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
      if ( (*recHit)->isValid() ) {
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
    }
    
    histograms_.fill("rs_nHitPerTrackVsEta_2d",nHit,track->momentum().eta());
    histograms_.fill("rs_nStripHitPerTrackVsEta_2d",nStripHit,track->momentum().eta());
    histograms_.fill("rs_nPixelHitPerTrackVsEta_2d",nPixelHit,track->momentum().eta());
    histograms_.fill("rs_nTIBHitPerTrackVsEta_2d",nTIBHit,track->momentum().eta());
    histograms_.fill("rs_nTOBHitPerTrackVsEta_2d",nTOBHit,track->momentum().eta());
    histograms_.fill("rs_nTIDHitPerTrackVsEta_2d",nTIDHit,track->momentum().eta());
    histograms_.fill("rs_nTECHitPerTrackVsEta_2d",nTECHit,track->momentum().eta());
    histograms_.fill("rs_nPXBHitPerTrackVsEta_2d",nPXBHit,track->momentum().eta());
    histograms_.fill("rs_nPXFHitPerTrackVsEta_2d",nPXFHit,track->momentum().eta());
    histograms_.fill("rs_nHitPerTrackVsPhi_2d",nHit,track->momentum().phi());

  }

  // RS seeds
  histograms_.fill("rs_numSeeds_1d",rsSeedCollection->size());

  // RS raw clouds
  histograms_.fill("rs_numRawClouds_1d",rsRawCloudCollection->size());
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
    
    histograms_.fill("rs_nHitPerRawCloudVsEta_2d",nHit,0.);
    histograms_.fill("rs_nStripHitPerRawCloudVsEta_2d",nStripHit,0.);
    histograms_.fill("rs_nPixelHitPerRawCloudVsEta_2d",nPixelHit,0.);
    histograms_.fill("rs_nTIBHitPerRawCloudVsEta_2d",nTIBHit,0.);
    histograms_.fill("rs_nTOBHitPerRawCloudVsEta_2d",nTOBHit,0.);
    histograms_.fill("rs_nTIDHitPerRawCloudVsEta_2d",nTIDHit,0.);
    histograms_.fill("rs_nTECHitPerRawCloudVsEta_2d",nTECHit,0.);
    histograms_.fill("rs_nPXBHitPerRawCloudVsEta_2d",nPXBHit,0.);
    histograms_.fill("rs_nPXFHitPerRawCloudVsEta_2d",nPXFHit,0.);

  }

  // RS clean clouds
  histograms_.fill("rs_numCleanClouds_1d",rsCleanCloudCollection->size());
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
    
    histograms_.fill("rs_nHitPerCleanCloudVsEta_2d",nHit,0.);
    histograms_.fill("rs_nStripHitPerCleanCloudVsEta_2d",nStripHit,0.);
    histograms_.fill("rs_nPixelHitPerCleanCloudVsEta_2d",nPixelHit,0.);
    histograms_.fill("rs_nTIBHitPerCleanCloudVsEta_2d",nTIBHit,0.);
    histograms_.fill("rs_nTOBHitPerCleanCloudVsEta_2d",nTOBHit,0.);
    histograms_.fill("rs_nTIDHitPerCleanCloudVsEta_2d",nTIDHit,0.);
    histograms_.fill("rs_nTECHitPerCleanCloudVsEta_2d",nTECHit,0.);
    histograms_.fill("rs_nPXBHitPerCleanCloudVsEta_2d",nPXBHit,0.);
    histograms_.fill("rs_nPXFHitPerCleanCloudVsEta_2d",nPXFHit,0.);

  }

  // RS track candidates
  histograms_.fill("rs_numTrackCandidates_1d",rsTrackCandidateCollection->size());
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
    
    histograms_.fill("rs_nHitPerTrackCandidateVsEta_2d",nHit,0.);
    histograms_.fill("rs_nStripHitPerTrackCandidateVsEta_2d",nStripHit,0.);
    histograms_.fill("rs_nPixelHitPerTrackCandidateVsEta_2d",nPixelHit,0.);
    histograms_.fill("rs_nTIBHitPerTrackCandidateVsEta_2d",nTIBHit,0.);
    histograms_.fill("rs_nTOBHitPerTrackCandidateVsEta_2d",nTOBHit,0.);
    histograms_.fill("rs_nTIDHitPerTrackCandidateVsEta_2d",nTIDHit,0.);
    histograms_.fill("rs_nTECHitPerTrackCandidateVsEta_2d",nTECHit,0.);
    histograms_.fill("rs_nPXBHitPerTrackCandidateVsEta_2d",nPXBHit,0.);
    histograms_.fill("rs_nPXFHitPerTrackCandidateVsEta_2d",nPXFHit,0.);

  }

  // Ckf tracks
  histograms_.fill("ckf_numTracks_1d",ckfTrackCollection->size());
  for ( reco::TrackCollection::const_iterator track = ckfTrackCollection->begin();
	track != ckfTrackCollection->end();
	++track ) {
    histograms_.fill("ckf_chi2_1d",track->normalizedChi2());
    histograms_.fill("ckf_pt_1d",track->pt());
    histograms_.fill("ckf_nhit_1d",track->found());
    histograms_.fill("ckf_eta_1d",track->momentum().eta());
    histograms_.fill("ckf_phi_1d",track->momentum().phi());
    histograms_.fill("ckf_transCurv_1d",track->transverseCurvature());
    histograms_.fill("ckf_phiZero_1d",track->phi0());
    histograms_.fill("ckf_theta_1d",track->theta());
    histograms_.fill("ckf_dZero_1d",track->d0());
    histograms_.fill("ckf_dZ_1d",track->dz());

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
      if ( (*recHit)->isValid() ) {
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
    }
    
    histograms_.fill("ckf_nHitPerTrackVsEta_2d",nHit,track->momentum().eta());
    histograms_.fill("ckf_nStripHitPerTrackVsEta_2d",nStripHit,track->momentum().eta());
    histograms_.fill("ckf_nPixelHitPerTrackVsEta_2d",nPixelHit,track->momentum().eta());
    histograms_.fill("ckf_nTIBHitPerTrackVsEta_2d",nTIBHit,track->momentum().eta());
    histograms_.fill("ckf_nTOBHitPerTrackVsEta_2d",nTOBHit,track->momentum().eta());
    histograms_.fill("ckf_nTIDHitPerTrackVsEta_2d",nTIDHit,track->momentum().eta());
    histograms_.fill("ckf_nTECHitPerTrackVsEta_2d",nTECHit,track->momentum().eta());
    histograms_.fill("ckf_nPXBHitPerTrackVsEta_2d",nPXBHit,track->momentum().eta());
    histograms_.fill("ckf_nPXFHitPerTrackVsEta_2d",nPXFHit,track->momentum().eta());
    histograms_.fill("ckf_nHitPerTrackVsPhi_2d",nHit,track->momentum().phi());

  }

  histograms_.fill("ckf_numSeeds_1d",ckfSeedCollection->size());

  // CKF track candidates
  histograms_.fill("ckf_numTrackCandidates_1d",ckfTrackCandidateCollection->size());
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
    
    histograms_.fill("ckf_nHitPerTrackCandidateVsEta_2d",nHit,0.);
    histograms_.fill("ckf_nStripHitPerTrackCandidateVsEta_2d",nStripHit,0.);
    histograms_.fill("ckf_nPixelHitPerTrackCandidateVsEta_2d",nPixelHit,0.);
    histograms_.fill("ckf_nTIBHitPerTrackCandidateVsEta_2d",nTIBHit,0.);
    histograms_.fill("ckf_nTOBHitPerTrackCandidateVsEta_2d",nTOBHit,0.);
    histograms_.fill("ckf_nTIDHitPerTrackCandidateVsEta_2d",nTIDHit,0.);
    histograms_.fill("ckf_nTECHitPerTrackCandidateVsEta_2d",nTECHit,0.);
    histograms_.fill("ckf_nPXBHitPerTrackCandidateVsEta_2d",nPXBHit,0.);
    histograms_.fill("ckf_nPXFHitPerTrackCandidateVsEta_2d",nPXFHit,0.);

  }

  if ( (ckfTrackCollection->size() == sameNumberOfTracks) &&
       (rsTrackCollection->size() == sameNumberOfTracks ) ) {
    // RS tracks
    histograms_.fill("rs_same_numTracks_1d",rsTrackCollection->size());
    for ( reco::TrackCollection::const_iterator track = rsTrackCollection->begin();
	  track != rsTrackCollection->end();
	  ++track ) {
      histograms_.fill("rs_same_chi2_1d",track->normalizedChi2());
      histograms_.fill("rs_same_pt_1d",track->pt());
      histograms_.fill("rs_same_nhit_1d",track->found());
      histograms_.fill("rs_same_eta_1d",track->momentum().eta());
      histograms_.fill("rs_same_phi_1d",track->momentum().phi());
      histograms_.fill("rs_same_transCurv_1d",track->transverseCurvature());
      histograms_.fill("rs_same_phiZero_1d",track->phi0());
      histograms_.fill("rs_same_theta_1d",track->theta());
      histograms_.fill("rs_same_dZero_1d",track->d0());
      histograms_.fill("rs_same_dZ_1d",track->dz());

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
	if ( (*recHit)->isValid() ) {
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
      }
    
      histograms_.fill("rs_same_nHitPerTrackVsEta_2d",nHit,track->momentum().eta());
      histograms_.fill("rs_same_nStripHitPerTrackVsEta_2d",nStripHit,track->momentum().eta());
      histograms_.fill("rs_same_nPixelHitPerTrackVsEta_2d",nPixelHit,track->momentum().eta());
      histograms_.fill("rs_same_nTIBHitPerTrackVsEta_2d",nTIBHit,track->momentum().eta());
      histograms_.fill("rs_same_nTOBHitPerTrackVsEta_2d",nTOBHit,track->momentum().eta());
      histograms_.fill("rs_same_nTIDHitPerTrackVsEta_2d",nTIDHit,track->momentum().eta());
      histograms_.fill("rs_same_nTECHitPerTrackVsEta_2d",nTECHit,track->momentum().eta());
      histograms_.fill("rs_same_nPXBHitPerTrackVsEta_2d",nPXBHit,track->momentum().eta());
      histograms_.fill("rs_same_nPXFHitPerTrackVsEta_2d",nPXFHit,track->momentum().eta());

    }

    // RS seeds, check for doubled seeds, deactivated
    //   histograms_.fill("rs_same_numSeeds_1d",rsSeedCollection->size());
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
    histograms_.fill("rs_same_numRawClouds_1d",rsRawCloudCollection->size());
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
    
      histograms_.fill("rs_same_nHitPerRawCloudVsEta_2d",nHit,0.);
      histograms_.fill("rs_same_nStripHitPerRawCloudVsEta_2d",nStripHit,0.);
      histograms_.fill("rs_same_nPixelHitPerRawCloudVsEta_2d",nPixelHit,0.);
      histograms_.fill("rs_same_nTIBHitPerRawCloudVsEta_2d",nTIBHit,0.);
      histograms_.fill("rs_same_nTOBHitPerRawCloudVsEta_2d",nTOBHit,0.);
      histograms_.fill("rs_same_nTIDHitPerRawCloudVsEta_2d",nTIDHit,0.);
      histograms_.fill("rs_same_nTECHitPerRawCloudVsEta_2d",nTECHit,0.);
      histograms_.fill("rs_same_nPXBHitPerRawCloudVsEta_2d",nPXBHit,0.);
      histograms_.fill("rs_same_nPXFHitPerRawCloudVsEta_2d",nPXFHit,0.);

    }

    // RS clean clouds
    histograms_.fill("rs_same_numCleanClouds_1d",rsCleanCloudCollection->size());
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
    
      histograms_.fill("rs_same_nHitPerCleanCloudVsEta_2d",nHit,0.);
      histograms_.fill("rs_same_nStripHitPerCleanCloudVsEta_2d",nStripHit,0.);
      histograms_.fill("rs_same_nPixelHitPerCleanCloudVsEta_2d",nPixelHit,0.);
      histograms_.fill("rs_same_nTIBHitPerCleanCloudVsEta_2d",nTIBHit,0.);
      histograms_.fill("rs_same_nTOBHitPerCleanCloudVsEta_2d",nTOBHit,0.);
      histograms_.fill("rs_same_nTIDHitPerCleanCloudVsEta_2d",nTIDHit,0.);
      histograms_.fill("rs_same_nTECHitPerCleanCloudVsEta_2d",nTECHit,0.);
      histograms_.fill("rs_same_nPXBHitPerCleanCloudVsEta_2d",nPXBHit,0.);
      histograms_.fill("rs_same_nPXFHitPerCleanCloudVsEta_2d",nPXFHit,0.);

    }

    // RS track candidates
    histograms_.fill("rs_same_numTrackCandidates_1d",rsTrackCandidateCollection->size());
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
    
      histograms_.fill("rs_same_nHitPerTrackCandidateVsEta_2d",nHit,0.);
      histograms_.fill("rs_same_nStripHitPerTrackCandidateVsEta_2d",nStripHit,0.);
      histograms_.fill("rs_same_nPixelHitPerTrackCandidateVsEta_2d",nPixelHit,0.);
      histograms_.fill("rs_same_nTIBHitPerTrackCandidateVsEta_2d",nTIBHit,0.);
      histograms_.fill("rs_same_nTOBHitPerTrackCandidateVsEta_2d",nTOBHit,0.);
      histograms_.fill("rs_same_nTIDHitPerTrackCandidateVsEta_2d",nTIDHit,0.);
      histograms_.fill("rs_same_nTECHitPerTrackCandidateVsEta_2d",nTECHit,0.);
      histograms_.fill("rs_same_nPXBHitPerTrackCandidateVsEta_2d",nPXBHit,0.);
      histograms_.fill("rs_same_nPXFHitPerTrackCandidateVsEta_2d",nPXFHit,0.);

    }

    // Ckf tracks
    histograms_.fill("ckf_same_numTracks_1d",ckfTrackCollection->size());
    for ( reco::TrackCollection::const_iterator track = ckfTrackCollection->begin();
	  track != ckfTrackCollection->end();
	  ++track ) {
      histograms_.fill("ckf_same_chi2_1d",track->normalizedChi2());
      histograms_.fill("ckf_same_pt_1d",track->pt());
      histograms_.fill("ckf_same_nhit_1d",track->found());
      histograms_.fill("ckf_same_eta_1d",track->momentum().eta());
      histograms_.fill("ckf_same_phi_1d",track->momentum().phi());
      histograms_.fill("ckf_same_transCurv_1d",track->transverseCurvature());
      histograms_.fill("ckf_same_phiZero_1d",track->phi0());
      histograms_.fill("ckf_same_theta_1d",track->theta());
      histograms_.fill("ckf_same_dZero_1d",track->d0());
      histograms_.fill("ckf_same_dZ_1d",track->dz());

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
	if ( (*recHit)->isValid() ) {
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
      }
    
      histograms_.fill("ckf_same_nHitPerTrackVsEta_2d",nHit,track->momentum().eta());
      histograms_.fill("ckf_same_nStripHitPerTrackVsEta_2d",nStripHit,track->momentum().eta());
      histograms_.fill("ckf_same_nPixelHitPerTrackVsEta_2d",nPixelHit,track->momentum().eta());
      histograms_.fill("ckf_same_nTIBHitPerTrackVsEta_2d",nTIBHit,track->momentum().eta());
      histograms_.fill("ckf_same_nTOBHitPerTrackVsEta_2d",nTOBHit,track->momentum().eta());
      histograms_.fill("ckf_same_nTIDHitPerTrackVsEta_2d",nTIDHit,track->momentum().eta());
      histograms_.fill("ckf_same_nTECHitPerTrackVsEta_2d",nTECHit,track->momentum().eta());
      histograms_.fill("ckf_same_nPXBHitPerTrackVsEta_2d",nPXBHit,track->momentum().eta());
      histograms_.fill("ckf_same_nPXFHitPerTrackVsEta_2d",nPXFHit,track->momentum().eta());

    }

    histograms_.fill("ckf_same_numSeeds_1d",ckfSeedCollection->size());

    // CKF track candidates
    histograms_.fill("ckf_same_numTrackCandidates_1d",ckfTrackCandidateCollection->size());
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
    
      histograms_.fill("ckf_same_nHitPerTrackCandidateVsEta_2d",nHit,0.);
      histograms_.fill("ckf_same_nStripHitPerTrackCandidateVsEta_2d",nStripHit,0.);
      histograms_.fill("ckf_same_nPixelHitPerTrackCandidateVsEta_2d",nPixelHit,0.);
      histograms_.fill("ckf_same_nTIBHitPerTrackCandidateVsEta_2d",nTIBHit,0.);
      histograms_.fill("ckf_same_nTOBHitPerTrackCandidateVsEta_2d",nTOBHit,0.);
      histograms_.fill("ckf_same_nTIDHitPerTrackCandidateVsEta_2d",nTIDHit,0.);
      histograms_.fill("ckf_same_nTECHitPerTrackCandidateVsEta_2d",nTECHit,0.);
      histograms_.fill("ckf_same_nPXBHitPerTrackCandidateVsEta_2d",nPXBHit,0.);
      histograms_.fill("ckf_same_nPXFHitPerTrackCandidateVsEta_2d",nPXFHit,0.);

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
