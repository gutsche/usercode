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
// $Date: 2006/09/08 23:31:15 $
// $Revision: 1.5 $
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

#include "GutSoftTools/HistogramFactory/interface/HistogramFactory.h"

RoadSearchTrackValidation::RoadSearchTrackValidation(const edm::ParameterSet& iConfig)
{

  // HistogramFactory
  HistogramFactory histogramFactory;

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
//   rs_chi2               = histogramFactory.TH1DFactory("rs_chi2","Track reduced chi^{2}",chi2_nbins,chi2_low,chi2_high,"#chi^2","Events");
//   rs_pt                 = histogramFactory.TH1DFactory("rs_pt","Track p_{T} (GeV)",pt_nbins,pt_low,pt_high,"p_{T} [GeV]","Events");
//   rs_eta                = histogramFactory.TH1DFactory("rs_eta","Track #eta",eta_nbins,eta_low,eta_high,"#eta","Events");
//   rs_phi                = histogramFactory.TH1DFactory("rs_phi","Track #phi",phi_nbins,phi_low,phi_high,"#phi","Events");
//   rs_transCurv          = histogramFactory.TH1DFactory("rs_transCurv","Transverse curvature",transCurv_nbins,transCurv_low,transCurv_high,"transCurv","");
//   rs_phiZero            = histogramFactory.TH1DFactory("rs_phiZero","phiZero",phiZero_nbins,phiZero_low,phiZero_high,"#phi_{0}","Events");
//   rs_theta              = histogramFactory.TH1DFactory("rs_theta","Theta",theta_nbins,theta_low,theta_high,"#theta","Events");
//   rs_dZero              = histogramFactory.TH1DFactory("rs_dZero",",dZero",dZero_nbins,dZero_low,dZero_high,"d_{0}","Events");
//   rs_dZ                 = histogramFactory.TH1DFactory("rs_dZ","dZ",dZ_nbins,dZ_low,dZ_high,"d_{Z}","Events");
//   rs_nhit               = histogramFactory.TH1DFactory("rs_nhit","Number of hits per track",nhit_nbins,nhit_low,nhit_high,"n_{hit}","Events");

//   rs_numSeeds           = histogramFactory.TH1IFactory("rs_numSeeds","Number of Seeds per event",numSeeds_nbins,numSeeds_low,numSeeds_high,"n_{seeds}","Events");
//   rs_numRawClouds       = histogramFactory.TH1IFactory("rs_numRawClouds","Number of raw clouds per event",numRawClouds_nbins,numRawClouds_low,numRawClouds_high,"n_{rawClouds}","Events");
//   rs_numCleanClouds     = histogramFactory.TH1IFactory("rs_numCleanClouds","Number of clean clouds per event",numCleanClouds_nbins,numCleanClouds_low,numCleanClouds_high,"n_{cleanClouds}","Events");
//   rs_numTrackCandidates = histogramFactory.TH1IFactory("rs_numTrackCandidates","Number of track candidates per event",numTrackCandidates_nbins,numTrackCandidates_low,numTrackCandidates_high,"n_{TrackCandidates}","Events");
//   rs_numTracks          = histogramFactory.TH1IFactory("rs_numTracks","Number of tracks per event",numTracks_nbins,numTracks_low,numTracks_high,"n_{Tracks}","Events");

//   rs_nHitPerTrackVsEta = histogramFactory.TH2DFactory("rs_nHitPerTrackVsEta","Number of hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   rs_nHitPerTrackCandidateVsEta = histogramFactory.TH2DFactory("rs_nHitPerTrackCandidateVsEta","Number of hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   rs_nHitPerCleanCloudVsEta = histogramFactory.TH2DFactory("rs_nHitPerCleanCloudVsEta","Number of hits per clean cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   rs_nHitPerRawCloudVsEta = histogramFactory.TH2DFactory("rs_nHitPerRawCloudVsEta","Number of hits per raw cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

//   rs_nHitPerTrackVsPhi = histogramFactory.TH2DFactory("rs_nHitPerTrackVsPhi","Number of hits per track vs. #phi",nhit_nbins,nhit_low,nhit_high,phi_nbins,phi_low,phi_high,"n_{Hit}","#phi","Events");

//   rs_nStripHitPerTrackVsEta = histogramFactory.TH2DFactory("rs_nStripHitPerTrackVsEta","Number of strip hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   rs_nStripHitPerTrackCandidateVsEta = histogramFactory.TH2DFactory("rs_nStripHitPerTrackCandidateVsEta","Number of strip hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   rs_nStripHitPerCleanCloudVsEta = histogramFactory.TH2DFactory("rs_nStripHitPerCleanCloudVsEta","Number of strip hits per clean cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   rs_nStripHitPerRawCloudVsEta = histogramFactory.TH2DFactory("rs_nStripHitPerRawCloudVsEta","Number of strip hits per raw cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

//   rs_nPixelHitPerTrackVsEta = histogramFactory.TH2DFactory("rs_nPixelHitPerTrackVsEta","Number of pixel hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   rs_nPixelHitPerTrackCandidateVsEta = histogramFactory.TH2DFactory("rs_nPixelHitPerTrackCandidateVsEta","Number of pixel hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   rs_nPixelHitPerCleanCloudVsEta = histogramFactory.TH2DFactory("rs_nPixelHitPerCleanCloudVsEta","Number of pixel hits per clean cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   rs_nPixelHitPerRawCloudVsEta = histogramFactory.TH2DFactory("rs_nPixelHitPerRawCloudVsEta","Number of pixel hits per raw cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

//   rs_nTIBHitPerTrackVsEta = histogramFactory.TH2DFactory("rs_nTIBHitPerTrackVsEta","Number of TIB hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   rs_nTIBHitPerTrackCandidateVsEta = histogramFactory.TH2DFactory("rs_nTIBHitPerTrackCandidateVsEta","Number of TIB hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   rs_nTIBHitPerCleanCloudVsEta = histogramFactory.TH2DFactory("rs_nTIBHitPerCleanCloudVsEta","Number of TIB hits per clean cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   rs_nTIBHitPerRawCloudVsEta = histogramFactory.TH2DFactory("rs_nTIBHitPerRawCloudVsEta","Number of TIB hits per raw cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

//   rs_nTOBHitPerTrackVsEta = histogramFactory.TH2DFactory("rs_nTOBHitPerTrackVsEta","Number of TOB hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   rs_nTOBHitPerTrackCandidateVsEta = histogramFactory.TH2DFactory("rs_nTOBHitPerTrackCandidateVsEta","Number of TOB hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   rs_nTOBHitPerCleanCloudVsEta = histogramFactory.TH2DFactory("rs_nTOBHitPerCleanCloudVsEta","Number of TOB hits per clean cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   rs_nTOBHitPerRawCloudVsEta = histogramFactory.TH2DFactory("rs_nTOBHitPerRawCloudVsEta","Number of TOB hits per raw cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

//   rs_nTIDHitPerTrackVsEta = histogramFactory.TH2DFactory("rs_nTIDHitPerTrackVsEta","Number of TID hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   rs_nTIDHitPerTrackCandidateVsEta = histogramFactory.TH2DFactory("rs_nTIDHitPerTrackCandidateVsEta","Number of TID hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   rs_nTIDHitPerCleanCloudVsEta = histogramFactory.TH2DFactory("rs_nTIDHitPerCleanCloudVsEta","Number of TID hits per clean cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   rs_nTIDHitPerRawCloudVsEta = histogramFactory.TH2DFactory("rs_nTIDHitPerRawCloudVsEta","Number of TID hits per raw cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

//   rs_nTECHitPerTrackVsEta = histogramFactory.TH2DFactory("rs_nTECHitPerTrackVsEta","Number of TEC hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   rs_nTECHitPerTrackCandidateVsEta = histogramFactory.TH2DFactory("rs_nTECHitPerTrackCandidateVsEta","Number of TEC hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   rs_nTECHitPerCleanCloudVsEta = histogramFactory.TH2DFactory("rs_nTECHitPerCleanCloudVsEta","Number of TEC hits per clean cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   rs_nTECHitPerRawCloudVsEta = histogramFactory.TH2DFactory("rs_nTECHitPerRawCloudVsEta","Number of TEC hits per raw cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

//   rs_nPXBHitPerTrackVsEta = histogramFactory.TH2DFactory("rs_nPXBHitPerTrackVsEta","Number of PXB hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   rs_nPXBHitPerTrackCandidateVsEta = histogramFactory.TH2DFactory("rs_nPXBHitPerTrackCandidateVsEta","Number of PXB hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   rs_nPXBHitPerCleanCloudVsEta = histogramFactory.TH2DFactory("rs_nPXBHitPerCleanCloudVsEta","Number of PXB hits per clean cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   rs_nPXBHitPerRawCloudVsEta = histogramFactory.TH2DFactory("rs_nPXBHitPerRawCloudVsEta","Number of PXB hits per raw cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

//   rs_nPXFHitPerTrackVsEta = histogramFactory.TH2DFactory("rs_nPXFHitPerTrackVsEta","Number of PXF hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   rs_nPXFHitPerTrackCandidateVsEta = histogramFactory.TH2DFactory("rs_nPXFHitPerTrackCandidateVsEta","Number of PXF hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   rs_nPXFHitPerCleanCloudVsEta = histogramFactory.TH2DFactory("rs_nPXFHitPerCleanCloudVsEta","Number of PXF hits per clean cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   rs_nPXFHitPerRawCloudVsEta = histogramFactory.TH2DFactory("rs_nPXFHitPerRawCloudVsEta","Number of PXF hits per raw cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

//   // RS plots where CkF and RS have the same number of tracks (steered by parameter)
//   rs_same_chi2               = histogramFactory.TH1DFactory("rs_same_chi2","Track reduced chi^{2}",chi2_nbins,chi2_low,chi2_high,"#chi^2","Events");
//   rs_same_pt                 = histogramFactory.TH1DFactory("rs_same_pt","Track p_{T} (GeV)",pt_nbins,pt_low,pt_high,"p_{T} [GeV]","Events");
//   rs_same_eta                = histogramFactory.TH1DFactory("rs_same_eta","Track #eta",eta_nbins,eta_low,eta_high,"#eta","Events");
//   rs_same_phi                = histogramFactory.TH1DFactory("rs_same_phi","Track #phi",phi_nbins,phi_low,phi_high,"#phi","Events");
//   rs_same_transCurv          = histogramFactory.TH1DFactory("rs_same_transCurv","Transverse curvature",transCurv_nbins,transCurv_low,transCurv_high,"transCurv","");
//   rs_same_phiZero            = histogramFactory.TH1DFactory("rs_same_phiZero","phiZero",phiZero_nbins,phiZero_low,phiZero_high,"#phi_{0}","Events");
//   rs_same_theta              = histogramFactory.TH1DFactory("rs_same_theta","Theta",theta_nbins,theta_low,theta_high,"#theta","Events");
//   rs_same_dZero              = histogramFactory.TH1DFactory("rs_same_dZero",",dZero",dZero_nbins,dZero_low,dZero_high,"d_{0}","Events");
//   rs_same_dZ                 = histogramFactory.TH1DFactory("rs_same_dZ","dZ",dZ_nbins,dZ_low,dZ_high,"d_{Z}","Events");
//   rs_same_nhit               = histogramFactory.TH1DFactory("rs_same_nhit","Number of hits per track",nhit_nbins,nhit_low,nhit_high,"n_{hit}","Events");

//   rs_same_numSeeds           = histogramFactory.TH1IFactory("rs_same_numSeeds","Number of Seeds per event",numSeeds_nbins,numSeeds_low,numSeeds_high,"n_{seeds}","Events");
//   rs_same_numRawClouds       = histogramFactory.TH1IFactory("rs_same_numRawClouds","Number of raw clouds per event",numRawClouds_nbins,numRawClouds_low,numRawClouds_high,"n_{rawClouds}","Events");
//   rs_same_numCleanClouds     = histogramFactory.TH1IFactory("rs_same_numCleanClouds","Number of clean clouds per event",numCleanClouds_nbins,numCleanClouds_low,numCleanClouds_high,"n_{cleanClouds}","Events");
//   rs_same_numTrackCandidates = histogramFactory.TH1IFactory("rs_same_numTrackCandidates","Number of track candidates per event",numTrackCandidates_nbins,numTrackCandidates_low,numTrackCandidates_high,"n_{TrackCandidates}","Events");
//   rs_same_numTracks          = histogramFactory.TH1IFactory("rs_same_numTracks","Number of tracks per event",numTracks_nbins,numTracks_low,numTracks_high,"n_{Tracks}","Events");

//   rs_same_nHitPerTrackVsEta = histogramFactory.TH2DFactory("rs_same_nHitPerTrackVsEta","Number of hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   rs_same_nHitPerTrackCandidateVsEta = histogramFactory.TH2DFactory("rs_same_nHitPerTrackCandidateVsEta","Number of hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   rs_same_nHitPerCleanCloudVsEta = histogramFactory.TH2DFactory("rs_same_nHitPerCleanCloudVsEta","Number of hits per clean cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   rs_same_nHitPerRawCloudVsEta = histogramFactory.TH2DFactory("rs_same_nHitPerRawCloudVsEta","Number of hits per raw cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

//   rs_same_nStripHitPerTrackVsEta = histogramFactory.TH2DFactory("rs_same_nStripHitPerTrackVsEta","Number of strip hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   rs_same_nStripHitPerTrackCandidateVsEta = histogramFactory.TH2DFactory("rs_same_nStripHitPerTrackCandidateVsEta","Number of strip hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   rs_same_nStripHitPerCleanCloudVsEta = histogramFactory.TH2DFactory("rs_same_nStripHitPerCleanCloudVsEta","Number of strip hits per clean cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   rs_same_nStripHitPerRawCloudVsEta = histogramFactory.TH2DFactory("rs_same_nStripHitPerRawCloudVsEta","Number of strip hits per raw cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

//   rs_same_nPixelHitPerTrackVsEta = histogramFactory.TH2DFactory("rs_same_nPixelHitPerTrackVsEta","Number of pixel hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   rs_same_nPixelHitPerTrackCandidateVsEta = histogramFactory.TH2DFactory("rs_same_nPixelHitPerTrackCandidateVsEta","Number of pixel hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   rs_same_nPixelHitPerCleanCloudVsEta = histogramFactory.TH2DFactory("rs_same_nPixelHitPerCleanCloudVsEta","Number of pixel hits per clean cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   rs_same_nPixelHitPerRawCloudVsEta = histogramFactory.TH2DFactory("rs_same_nPixelHitPerRawCloudVsEta","Number of pixel hits per raw cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

//   rs_same_nTIBHitPerTrackVsEta = histogramFactory.TH2DFactory("rs_same_nTIBHitPerTrackVsEta","Number of TIB hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   rs_same_nTIBHitPerTrackCandidateVsEta = histogramFactory.TH2DFactory("rs_same_nTIBHitPerTrackCandidateVsEta","Number of TIB hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   rs_same_nTIBHitPerCleanCloudVsEta = histogramFactory.TH2DFactory("rs_same_nTIBHitPerCleanCloudVsEta","Number of TIB hits per clean cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   rs_same_nTIBHitPerRawCloudVsEta = histogramFactory.TH2DFactory("rs_same_nTIBHitPerRawCloudVsEta","Number of TIB hits per raw cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

//   rs_same_nTOBHitPerTrackVsEta = histogramFactory.TH2DFactory("rs_same_nTOBHitPerTrackVsEta","Number of TOB hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   rs_same_nTOBHitPerTrackCandidateVsEta = histogramFactory.TH2DFactory("rs_same_nTOBHitPerTrackCandidateVsEta","Number of TOB hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   rs_same_nTOBHitPerCleanCloudVsEta = histogramFactory.TH2DFactory("rs_same_nTOBHitPerCleanCloudVsEta","Number of TOB hits per clean cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   rs_same_nTOBHitPerRawCloudVsEta = histogramFactory.TH2DFactory("rs_same_nTOBHitPerRawCloudVsEta","Number of TOB hits per raw cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

//   rs_same_nTIDHitPerTrackVsEta = histogramFactory.TH2DFactory("rs_same_nTIDHitPerTrackVsEta","Number of TID hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   rs_same_nTIDHitPerTrackCandidateVsEta = histogramFactory.TH2DFactory("rs_same_nTIDHitPerTrackCandidateVsEta","Number of TID hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   rs_same_nTIDHitPerCleanCloudVsEta = histogramFactory.TH2DFactory("rs_same_nTIDHitPerCleanCloudVsEta","Number of TID hits per clean cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   rs_same_nTIDHitPerRawCloudVsEta = histogramFactory.TH2DFactory("rs_same_nTIDHitPerRawCloudVsEta","Number of TID hits per raw cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

//   rs_same_nTECHitPerTrackVsEta = histogramFactory.TH2DFactory("rs_same_nTECHitPerTrackVsEta","Number of TEC hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   rs_same_nTECHitPerTrackCandidateVsEta = histogramFactory.TH2DFactory("rs_same_nTECHitPerTrackCandidateVsEta","Number of TEC hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   rs_same_nTECHitPerCleanCloudVsEta = histogramFactory.TH2DFactory("rs_same_nTECHitPerCleanCloudVsEta","Number of TEC hits per clean cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   rs_same_nTECHitPerRawCloudVsEta = histogramFactory.TH2DFactory("rs_same_nTECHitPerRawCloudVsEta","Number of TEC hits per raw cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

//   rs_same_nPXBHitPerTrackVsEta = histogramFactory.TH2DFactory("rs_same_nPXBHitPerTrackVsEta","Number of PXB hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   rs_same_nPXBHitPerTrackCandidateVsEta = histogramFactory.TH2DFactory("rs_same_nPXBHitPerTrackCandidateVsEta","Number of PXB hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   rs_same_nPXBHitPerCleanCloudVsEta = histogramFactory.TH2DFactory("rs_same_nPXBHitPerCleanCloudVsEta","Number of PXB hits per clean cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   rs_same_nPXBHitPerRawCloudVsEta = histogramFactory.TH2DFactory("rs_same_nPXBHitPerRawCloudVsEta","Number of PXB hits per raw cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

//   rs_same_nPXFHitPerTrackVsEta = histogramFactory.TH2DFactory("rs_same_nPXFHitPerTrackVsEta","Number of PXF hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   rs_same_nPXFHitPerTrackCandidateVsEta = histogramFactory.TH2DFactory("rs_same_nPXFHitPerTrackCandidateVsEta","Number of PXF hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   rs_same_nPXFHitPerCleanCloudVsEta = histogramFactory.TH2DFactory("rs_same_nPXFHitPerCleanCloudVsEta","Number of PXF hits per clean cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   rs_same_nPXFHitPerRawCloudVsEta = histogramFactory.TH2DFactory("rs_same_nPXFHitPerRawCloudVsEta","Number of PXF hits per raw cloud vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

  // CKF plots
  ckf_chi2               = histogramFactory.TH1DFactory("ckf_chi2","Track reduced chi^{2}",chi2_nbins,chi2_low,chi2_high,"#chi^2","Events");
  ckf_pt                 = histogramFactory.TH1DFactory("ckf_pt","Track p_{T} (GeV)",pt_nbins,pt_low,pt_high,"p_{T} [GeV]","Events");
  ckf_eta                = histogramFactory.TH1DFactory("ckf_eta","Track #eta",eta_nbins,eta_low,eta_high,"#eta","Events");
  ckf_phi                = histogramFactory.TH1DFactory("ckf_phi","Track #phi",phi_nbins,phi_low,phi_high,"#phi","Events");
  ckf_transCurv          = histogramFactory.TH1DFactory("ckf_transCurv","Transveckfe curvature",transCurv_nbins,transCurv_low,transCurv_high,"transCurv","");
  ckf_phiZero            = histogramFactory.TH1DFactory("ckf_phiZero","phiZero",phiZero_nbins,phiZero_low,phiZero_high,"#phi_{0}","Events");
  ckf_theta              = histogramFactory.TH1DFactory("ckf_theta","Theta",theta_nbins,theta_low,theta_high,"#theta","Events");
  ckf_dZero              = histogramFactory.TH1DFactory("ckf_dZero",",dZero",dZero_nbins,dZero_low,dZero_high,"d_{0}","Events");
  ckf_dZ                 = histogramFactory.TH1DFactory("ckf_dZ","dZ",dZ_nbins,dZ_low,dZ_high,"d_{Z}","Events");
  ckf_nhit               = histogramFactory.TH1DFactory("ckf_nhit","Number of hits per track",nhit_nbins,nhit_low,nhit_high,"n_{hit}","Events");

  ckf_numSeeds           = histogramFactory.TH1IFactory("ckf_numSeeds","Number of Seeds per event",numSeeds_nbins,numSeeds_low,numSeeds_high,"n_{seeds}","Events");
  ckf_numTrackCandidates = histogramFactory.TH1IFactory("ckf_numTrackCandidates","Number of track candidates per event",numTrackCandidates_nbins,numTrackCandidates_low,numTrackCandidates_high,"n_{TrackCandidates}","Events");
  ckf_numTracks          = histogramFactory.TH1IFactory("ckf_numTracks","Number of tracks per event",numTracks_nbins,numTracks_low,numTracks_high,"n_{Tracks}","Events");

  ckf_nHitPerTrackVsEta = histogramFactory.TH2DFactory("ckf_nHitPerTrackVsEta","Number of hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  ckf_nHitPerTrackCandidateVsEta = histogramFactory.TH2DFactory("ckf_nHitPerTrackCandidateVsEta","Number of hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

  ckf_nHitPerTrackVsPhi = histogramFactory.TH2DFactory("ckf_nHitPerTrackVsPhi","Number of hits per track vs. #phi",nhit_nbins,nhit_low,nhit_high,phi_nbins,phi_low,phi_high,"n_{Hit}","#phi","Events");

  ckf_nStripHitPerTrackVsEta = histogramFactory.TH2DFactory("ckf_nStripHitPerTrackVsEta","Number of strip hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  ckf_nStripHitPerTrackCandidateVsEta = histogramFactory.TH2DFactory("ckf_nStripHitPerTrackCandidateVsEta","Number of strip hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

  ckf_nPixelHitPerTrackVsEta = histogramFactory.TH2DFactory("ckf_nPixelHitPerTrackVsEta","Number of pixel hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  ckf_nPixelHitPerTrackCandidateVsEta = histogramFactory.TH2DFactory("ckf_nPixelHitPerTrackCandidateVsEta","Number of pixel hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

  ckf_nTIBHitPerTrackVsEta = histogramFactory.TH2DFactory("ckf_nTIBHitPerTrackVsEta","Number of TIB hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  ckf_nTIBHitPerTrackCandidateVsEta = histogramFactory.TH2DFactory("ckf_nTIBHitPerTrackCandidateVsEta","Number of TIB hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

  ckf_nTOBHitPerTrackVsEta = histogramFactory.TH2DFactory("ckf_nTOBHitPerTrackVsEta","Number of TOB hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  ckf_nTOBHitPerTrackCandidateVsEta = histogramFactory.TH2DFactory("ckf_nTOBHitPerTrackCandidateVsEta","Number of TOB hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

  ckf_nTIDHitPerTrackVsEta = histogramFactory.TH2DFactory("ckf_nTIDHitPerTrackVsEta","Number of TID hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  ckf_nTIDHitPerTrackCandidateVsEta = histogramFactory.TH2DFactory("ckf_nTIDHitPerTrackCandidateVsEta","Number of TID hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

  ckf_nTECHitPerTrackVsEta = histogramFactory.TH2DFactory("ckf_nTECHitPerTrackVsEta","Number of TEC hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  ckf_nTECHitPerTrackCandidateVsEta = histogramFactory.TH2DFactory("ckf_nTECHitPerTrackCandidateVsEta","Number of TEC hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

  ckf_nPXBHitPerTrackVsEta = histogramFactory.TH2DFactory("ckf_nPXBHitPerTrackVsEta","Number of PXB hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  ckf_nPXBHitPerTrackCandidateVsEta = histogramFactory.TH2DFactory("ckf_nPXBHitPerTrackCandidateVsEta","Number of PXB hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

  ckf_nPXFHitPerTrackVsEta = histogramFactory.TH2DFactory("ckf_nPXFHitPerTrackVsEta","Number of PXF hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
  ckf_nPXFHitPerTrackCandidateVsEta = histogramFactory.TH2DFactory("ckf_nPXFHitPerTrackCandidateVsEta","Number of PXF hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

//   // CKF plots where CkF and CKF have the same number of tracks (steered by parameter)
//   ckf_same_chi2               = histogramFactory.TH1DFactory("ckf_same_chi2","Track reduced chi^{2}",chi2_nbins,chi2_low,chi2_high,"#chi^2","Events");
//   ckf_same_pt                 = histogramFactory.TH1DFactory("ckf_same_pt","Track p_{T} (GeV)",pt_nbins,pt_low,pt_high,"p_{T} [GeV]","Events");
//   ckf_same_eta                = histogramFactory.TH1DFactory("ckf_same_eta","Track #eta",eta_nbins,eta_low,eta_high,"#eta","Events");
//   ckf_same_phi                = histogramFactory.TH1DFactory("ckf_same_phi","Track #phi",phi_nbins,phi_low,phi_high,"#phi","Events");
//   ckf_same_transCurv          = histogramFactory.TH1DFactory("ckf_same_transCurv","Transveckfe curvature",transCurv_nbins,transCurv_low,transCurv_high,"transCurv","");
//   ckf_same_phiZero            = histogramFactory.TH1DFactory("ckf_same_phiZero","phiZero",phiZero_nbins,phiZero_low,phiZero_high,"#phi_{0}","Events");
//   ckf_same_theta              = histogramFactory.TH1DFactory("ckf_same_theta","Theta",theta_nbins,theta_low,theta_high,"#theta","Events");
//   ckf_same_dZero              = histogramFactory.TH1DFactory("ckf_same_dZero",",dZero",dZero_nbins,dZero_low,dZero_high,"d_{0}","Events");
//   ckf_same_dZ                 = histogramFactory.TH1DFactory("ckf_same_dZ","dZ",dZ_nbins,dZ_low,dZ_high,"d_{Z}","Events");
//   ckf_same_nhit               = histogramFactory.TH1DFactory("ckf_same_nhit","Number of hits per track",nhit_nbins,nhit_low,nhit_high,"n_{hit}","Events");

//   ckf_same_numSeeds           = histogramFactory.TH1IFactory("ckf_same_numSeeds","Number of Seeds per event",numSeeds_nbins,numSeeds_low,numSeeds_high,"n_{seeds}","Events");
//   ckf_same_numTrackCandidates = histogramFactory.TH1IFactory("ckf_same_numTrackCandidates","Number of track candidates per event",numTrackCandidates_nbins,numTrackCandidates_low,numTrackCandidates_high,"n_{TrackCandidates}","Events");
//   ckf_same_numTracks          = histogramFactory.TH1IFactory("ckf_same_numTracks","Number of tracks per event",numTracks_nbins,numTracks_low,numTracks_high,"n_{Tracks}","Events");

//   ckf_same_nHitPerTrackVsEta = histogramFactory.TH2DFactory("ckf_same_nHitPerTrackVsEta","Number of hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   ckf_same_nHitPerTrackCandidateVsEta = histogramFactory.TH2DFactory("ckf_same_nHitPerTrackCandidateVsEta","Number of hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

//   ckf_same_nStripHitPerTrackVsEta = histogramFactory.TH2DFactory("ckf_same_nStripHitPerTrackVsEta","Number of strip hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   ckf_same_nStripHitPerTrackCandidateVsEta = histogramFactory.TH2DFactory("ckf_same_nStripHitPerTrackCandidateVsEta","Number of strip hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

//   ckf_same_nPixelHitPerTrackVsEta = histogramFactory.TH2DFactory("ckf_same_nPixelHitPerTrackVsEta","Number of pixel hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   ckf_same_nPixelHitPerTrackCandidateVsEta = histogramFactory.TH2DFactory("ckf_same_nPixelHitPerTrackCandidateVsEta","Number of pixel hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

//   ckf_same_nTIBHitPerTrackVsEta = histogramFactory.TH2DFactory("ckf_same_nTIBHitPerTrackVsEta","Number of TIB hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   ckf_same_nTIBHitPerTrackCandidateVsEta = histogramFactory.TH2DFactory("ckf_same_nTIBHitPerTrackCandidateVsEta","Number of TIB hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

//   ckf_same_nTOBHitPerTrackVsEta = histogramFactory.TH2DFactory("ckf_same_nTOBHitPerTrackVsEta","Number of TOB hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   ckf_same_nTOBHitPerTrackCandidateVsEta = histogramFactory.TH2DFactory("ckf_same_nTOBHitPerTrackCandidateVsEta","Number of TOB hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

//   ckf_same_nTIDHitPerTrackVsEta = histogramFactory.TH2DFactory("ckf_same_nTIDHitPerTrackVsEta","Number of TID hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   ckf_same_nTIDHitPerTrackCandidateVsEta = histogramFactory.TH2DFactory("ckf_same_nTIDHitPerTrackCandidateVsEta","Number of TID hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

//   ckf_same_nTECHitPerTrackVsEta = histogramFactory.TH2DFactory("ckf_same_nTECHitPerTrackVsEta","Number of TEC hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   ckf_same_nTECHitPerTrackCandidateVsEta = histogramFactory.TH2DFactory("ckf_same_nTECHitPerTrackCandidateVsEta","Number of TEC hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

//   ckf_same_nPXBHitPerTrackVsEta = histogramFactory.TH2DFactory("ckf_same_nPXBHitPerTrackVsEta","Number of PXB hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   ckf_same_nPXBHitPerTrackCandidateVsEta = histogramFactory.TH2DFactory("ckf_same_nPXBHitPerTrackCandidateVsEta","Number of PXB hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

//   ckf_same_nPXFHitPerTrackVsEta = histogramFactory.TH2DFactory("ckf_same_nPXFHitPerTrackVsEta","Number of PXF hits per track vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");
//   ckf_same_nPXFHitPerTrackCandidateVsEta = histogramFactory.TH2DFactory("ckf_same_nPXFHitPerTrackCandidateVsEta","Number of PXF hits per track candidate vs. #eta",nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,"n_{Hit}","#eta","Events");

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
//   rs_numTracks->Fill(rsTrackCollection->size());
//   for ( reco::TrackCollection::const_iterator track = rsTrackCollection->begin();
// 	track != rsTrackCollection->end();
// 	++track ) {
//     rs_chi2->Fill(track->normalizedChi2());
//     rs_pt->Fill(track->pt());
//     rs_nhit->Fill(track->found());
//     rs_eta->Fill(track->momentum().eta());
//     rs_phi->Fill(track->momentum().phi());
//     rs_transCurv->Fill(track->transverseCurvature());
//     rs_phiZero->Fill(track->phi0());
//     rs_theta->Fill(track->theta());
//     rs_dZero->Fill(track->d0());
//     rs_dZ->Fill(track->dz());

//     // loop over hits in tracks, count
//     unsigned int nHit      = 0;
//     unsigned int nStripHit = 0;
//     unsigned int nPixelHit = 0;
//     unsigned int nTIBHit   = 0;
//     unsigned int nTOBHit   = 0;
//     unsigned int nTIDHit   = 0;
//     unsigned int nTECHit   = 0;
//     unsigned int nPXBHit   = 0;
//     unsigned int nPXFHit   = 0;

//     for ( trackingRecHit_iterator recHit = track->recHitsBegin();
// 	  recHit != track->recHitsEnd();
// 	  ++recHit ) {
//       if ( (*recHit)->isValid() ) {
// 	++nHit;
// 	DetId id((*recHit)->geographicalId());

// 	if ( (unsigned int)id.subdetId() == StripSubdetector::TIB ) {
// 	  ++nStripHit;
// 	  ++nTIBHit;
// 	} else if ( (unsigned int)id.subdetId() == StripSubdetector::TOB ) {
// 	  ++nStripHit;
// 	  ++nTOBHit;
// 	} else if ( (unsigned int)id.subdetId() == StripSubdetector::TID ) {
// 	  ++nStripHit;
// 	  ++nTIDHit;
// 	} else if ( (unsigned int)id.subdetId() == StripSubdetector::TEC ) {
// 	  ++nStripHit;
// 	  ++nTECHit;
// 	} else if ( (unsigned int)id.subdetId() == PixelSubdetector::PixelBarrel ) {
// 	  ++nPixelHit;
// 	  ++nPXBHit;
// 	} else if ( (unsigned int)id.subdetId() == PixelSubdetector::PixelEndcap ) {
// 	  ++nPixelHit;
// 	  ++nPXFHit;
// 	}
//       }
//     }
    
//     rs_nHitPerTrackVsEta->Fill(nHit,track->momentum().eta());
//     rs_nStripHitPerTrackVsEta->Fill(nStripHit,track->momentum().eta());
//     rs_nPixelHitPerTrackVsEta->Fill(nPixelHit,track->momentum().eta());
//     rs_nTIBHitPerTrackVsEta->Fill(nTIBHit,track->momentum().eta());
//     rs_nTOBHitPerTrackVsEta->Fill(nTOBHit,track->momentum().eta());
//     rs_nTIDHitPerTrackVsEta->Fill(nTIDHit,track->momentum().eta());
//     rs_nTECHitPerTrackVsEta->Fill(nTECHit,track->momentum().eta());
//     rs_nPXBHitPerTrackVsEta->Fill(nPXBHit,track->momentum().eta());
//     rs_nPXFHitPerTrackVsEta->Fill(nPXFHit,track->momentum().eta());
//     rs_nHitPerTrackVsPhi->Fill(nHit,track->momentum().phi());

//   }

//   // RS seeds
//   rs_numSeeds->Fill(rsSeedCollection->size());

//   // RS raw clouds
//   rs_numRawClouds->Fill(rsRawCloudCollection->size());
//   for ( RoadSearchCloudCollection::const_iterator cloud = rsRawCloudCollection->begin();
// 	cloud != rsRawCloudCollection->end();
// 	++cloud ) {

//     // loop over hits in tracks, count
//     unsigned int nHit      = 0;
//     unsigned int nStripHit = 0;
//     unsigned int nPixelHit = 0;
//     unsigned int nTIBHit   = 0;
//     unsigned int nTOBHit   = 0;
//     unsigned int nTIDHit   = 0;
//     unsigned int nTECHit   = 0;
//     unsigned int nPXBHit   = 0;
//     unsigned int nPXFHit   = 0;

//     for ( RoadSearchCloud::RecHitOwnVector::const_iterator recHit = cloud->begin_hits();
// 	  recHit != cloud->end_hits();
// 	  ++recHit ) {
//       ++nHit;
//       DetId id(recHit->geographicalId());
      
//       if ( (unsigned int)id.subdetId() == StripSubdetector::TIB ) {
// 	++nStripHit;
// 	++nTIBHit;
//       } else if ( (unsigned int)id.subdetId() == StripSubdetector::TOB ) {
// 	++nStripHit;
// 	++nTOBHit;
//       } else if ( (unsigned int)id.subdetId() == StripSubdetector::TID ) {
// 	++nStripHit;
// 	++nTIDHit;
//       } else if ( (unsigned int)id.subdetId() == StripSubdetector::TEC ) {
// 	++nStripHit;
// 	++nTECHit;
//       } else if ( (unsigned int)id.subdetId() == PixelSubdetector::PixelBarrel ) {
// 	++nPixelHit;
// 	++nPXBHit;
//       } else if ( (unsigned int)id.subdetId() == PixelSubdetector::PixelEndcap ) {
// 	++nPixelHit;
// 	++nPXFHit;
//       }
//     }
    
//     rs_nHitPerRawCloudVsEta->Fill(nHit,0.);
//     rs_nStripHitPerRawCloudVsEta->Fill(nStripHit,0.);
//     rs_nPixelHitPerRawCloudVsEta->Fill(nPixelHit,0.);
//     rs_nTIBHitPerRawCloudVsEta->Fill(nTIBHit,0.);
//     rs_nTOBHitPerRawCloudVsEta->Fill(nTOBHit,0.);
//     rs_nTIDHitPerRawCloudVsEta->Fill(nTIDHit,0.);
//     rs_nTECHitPerRawCloudVsEta->Fill(nTECHit,0.);
//     rs_nPXBHitPerRawCloudVsEta->Fill(nPXBHit,0.);
//     rs_nPXFHitPerRawCloudVsEta->Fill(nPXFHit,0.);

//   }

//   // RS clean clouds
//   rs_numCleanClouds->Fill(rsCleanCloudCollection->size());
//   for ( RoadSearchCloudCollection::const_iterator cloud = rsCleanCloudCollection->begin();
// 	cloud != rsCleanCloudCollection->end();
// 	++cloud ) {

//     // loop over hits in tracks, count
//     unsigned int nHit      = 0;
//     unsigned int nStripHit = 0;
//     unsigned int nPixelHit = 0;
//     unsigned int nTIBHit   = 0;
//     unsigned int nTOBHit   = 0;
//     unsigned int nTIDHit   = 0;
//     unsigned int nTECHit   = 0;
//     unsigned int nPXBHit   = 0;
//     unsigned int nPXFHit   = 0;

//     for ( RoadSearchCloud::RecHitOwnVector::const_iterator recHit = cloud->begin_hits();
// 	  recHit != cloud->end_hits();
// 	  ++recHit ) {
//       ++nHit;
//       DetId id(recHit->geographicalId());
      
//       if ( (unsigned int)id.subdetId() == StripSubdetector::TIB ) {
// 	++nStripHit;
// 	++nTIBHit;
//       } else if ( (unsigned int)id.subdetId() == StripSubdetector::TOB ) {
// 	++nStripHit;
// 	++nTOBHit;
//       } else if ( (unsigned int)id.subdetId() == StripSubdetector::TID ) {
// 	++nStripHit;
// 	++nTIDHit;
//       } else if ( (unsigned int)id.subdetId() == StripSubdetector::TEC ) {
// 	++nStripHit;
// 	++nTECHit;
//       } else if ( (unsigned int)id.subdetId() == PixelSubdetector::PixelBarrel ) {
// 	++nPixelHit;
// 	++nPXBHit;
//       } else if ( (unsigned int)id.subdetId() == PixelSubdetector::PixelEndcap ) {
// 	++nPixelHit;
// 	++nPXFHit;
//       }
//     }
    
//     rs_nHitPerCleanCloudVsEta->Fill(nHit,0.);
//     rs_nStripHitPerCleanCloudVsEta->Fill(nStripHit,0.);
//     rs_nPixelHitPerCleanCloudVsEta->Fill(nPixelHit,0.);
//     rs_nTIBHitPerCleanCloudVsEta->Fill(nTIBHit,0.);
//     rs_nTOBHitPerCleanCloudVsEta->Fill(nTOBHit,0.);
//     rs_nTIDHitPerCleanCloudVsEta->Fill(nTIDHit,0.);
//     rs_nTECHitPerCleanCloudVsEta->Fill(nTECHit,0.);
//     rs_nPXBHitPerCleanCloudVsEta->Fill(nPXBHit,0.);
//     rs_nPXFHitPerCleanCloudVsEta->Fill(nPXFHit,0.);

//   }

//   // RS track candidates
//   rs_numTrackCandidates->Fill(rsTrackCandidateCollection->size());
//   for ( TrackCandidateCollection::const_iterator candidate = rsTrackCandidateCollection->begin();
// 	candidate != rsTrackCandidateCollection->end();
// 	++candidate ) {

//     // loop over hits in tracks, count
//     unsigned int nHit      = 0;
//     unsigned int nStripHit = 0;
//     unsigned int nPixelHit = 0;
//     unsigned int nTIBHit   = 0;
//     unsigned int nTOBHit   = 0;
//     unsigned int nTIDHit   = 0;
//     unsigned int nTECHit   = 0;
//     unsigned int nPXBHit   = 0;
//     unsigned int nPXFHit   = 0;

//     for ( TrackCandidate::RecHitContainer::const_iterator recHit = candidate->recHits().first;
// 	  recHit != candidate->recHits().second;
// 	  ++recHit ) {
//       ++nHit;
//       DetId id(recHit->geographicalId());
      
//       if ( (unsigned int)id.subdetId() == StripSubdetector::TIB ) {
// 	++nStripHit;
// 	++nTIBHit;
//       } else if ( (unsigned int)id.subdetId() == StripSubdetector::TOB ) {
// 	++nStripHit;
// 	++nTOBHit;
//       } else if ( (unsigned int)id.subdetId() == StripSubdetector::TID ) {
// 	++nStripHit;
// 	++nTIDHit;
//       } else if ( (unsigned int)id.subdetId() == StripSubdetector::TEC ) {
// 	++nStripHit;
// 	++nTECHit;
//       } else if ( (unsigned int)id.subdetId() == PixelSubdetector::PixelBarrel ) {
// 	++nPixelHit;
// 	++nPXBHit;
//       } else if ( (unsigned int)id.subdetId() == PixelSubdetector::PixelEndcap ) {
// 	++nPixelHit;
// 	++nPXFHit;
//       }
//     }
    
//     rs_nHitPerTrackCandidateVsEta->Fill(nHit,0.);
//     rs_nStripHitPerTrackCandidateVsEta->Fill(nStripHit,0.);
//     rs_nPixelHitPerTrackCandidateVsEta->Fill(nPixelHit,0.);
//     rs_nTIBHitPerTrackCandidateVsEta->Fill(nTIBHit,0.);
//     rs_nTOBHitPerTrackCandidateVsEta->Fill(nTOBHit,0.);
//     rs_nTIDHitPerTrackCandidateVsEta->Fill(nTIDHit,0.);
//     rs_nTECHitPerTrackCandidateVsEta->Fill(nTECHit,0.);
//     rs_nPXBHitPerTrackCandidateVsEta->Fill(nPXBHit,0.);
//     rs_nPXFHitPerTrackCandidateVsEta->Fill(nPXFHit,0.);

//   }

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

//   if ( (ckfTrackCollection->size() == sameNumberOfTracks) &&
//        (rsTrackCollection->size() == sameNumberOfTracks ) ) {
//     // RS tracks
//     rs_same_numTracks->Fill(rsTrackCollection->size());
//     for ( reco::TrackCollection::const_iterator track = rsTrackCollection->begin();
// 	  track != rsTrackCollection->end();
// 	  ++track ) {
//       rs_same_chi2->Fill(track->normalizedChi2());
//       rs_same_pt->Fill(track->pt());
//       rs_same_nhit->Fill(track->found());
//       rs_same_eta->Fill(track->momentum().eta());
//       rs_same_phi->Fill(track->momentum().phi());
//       rs_same_transCurv->Fill(track->transverseCurvature());
//       rs_same_phiZero->Fill(track->phi0());
//       rs_same_theta->Fill(track->theta());
//       rs_same_dZero->Fill(track->d0());
//       rs_same_dZ->Fill(track->dz());

//       // loop over hits in tracks, count
//       unsigned int nHit      = 0;
//       unsigned int nStripHit = 0;
//       unsigned int nPixelHit = 0;
//       unsigned int nTIBHit   = 0;
//       unsigned int nTOBHit   = 0;
//       unsigned int nTIDHit   = 0;
//       unsigned int nTECHit   = 0;
//       unsigned int nPXBHit   = 0;
//       unsigned int nPXFHit   = 0;

//       for ( trackingRecHit_iterator recHit = track->recHitsBegin();
// 	    recHit != track->recHitsEnd();
// 	    ++ recHit ) {
// 	if ( (*recHit)->isValid() ) {
// 	  ++nHit;
// 	  DetId id((*recHit)->geographicalId());

// 	  if ( (unsigned int)id.subdetId() == StripSubdetector::TIB ) {
// 	    ++nStripHit;
// 	    ++nTIBHit;
// 	  } else if ( (unsigned int)id.subdetId() == StripSubdetector::TOB ) {
// 	    ++nStripHit;
// 	    ++nTOBHit;
// 	  } else if ( (unsigned int)id.subdetId() == StripSubdetector::TID ) {
// 	    ++nStripHit;
// 	    ++nTIDHit;
// 	  } else if ( (unsigned int)id.subdetId() == StripSubdetector::TEC ) {
// 	    ++nStripHit;
// 	    ++nTECHit;
// 	  } else if ( (unsigned int)id.subdetId() == PixelSubdetector::PixelBarrel ) {
// 	    ++nPixelHit;
// 	    ++nPXBHit;
// 	  } else if ( (unsigned int)id.subdetId() == PixelSubdetector::PixelEndcap ) {
// 	    ++nPixelHit;
// 	    ++nPXFHit;
// 	  }
// 	}
//       }
    
//       rs_same_nHitPerTrackVsEta->Fill(nHit,track->momentum().eta());
//       rs_same_nStripHitPerTrackVsEta->Fill(nStripHit,track->momentum().eta());
//       rs_same_nPixelHitPerTrackVsEta->Fill(nPixelHit,track->momentum().eta());
//       rs_same_nTIBHitPerTrackVsEta->Fill(nTIBHit,track->momentum().eta());
//       rs_same_nTOBHitPerTrackVsEta->Fill(nTOBHit,track->momentum().eta());
//       rs_same_nTIDHitPerTrackVsEta->Fill(nTIDHit,track->momentum().eta());
//       rs_same_nTECHitPerTrackVsEta->Fill(nTECHit,track->momentum().eta());
//       rs_same_nPXBHitPerTrackVsEta->Fill(nPXBHit,track->momentum().eta());
//       rs_same_nPXFHitPerTrackVsEta->Fill(nPXFHit,track->momentum().eta());

//     }

//     // RS seeds, check for doubled seeds, deactivated
//     //   rs_same_numSeeds->Fill(rsSeedCollection->size());
//     //   for ( TrajectorySeedCollection::const_iterator seed1 = rsSeedCollection->begin();
//     // 	seed1 != rsSeedCollection->end();
//     // 	++seed1 ) {

//     //     for ( TrajectorySeedCollection::const_iterator seed2 = seed1+1;
//     // 	  seed2 != rsSeedCollection->end();
//     // 	  ++seed2 ) {

//     //       unsigned int counter = 0;
//     //       for (TrajectorySeed::const_iterator hit1 = seed1->recHits().first;
//     // 	   hit1 != seed1->recHits().second;
//     // 	   ++hit1 ) {
//     // 	for (TrajectorySeed::const_iterator hit2 = seed2->recHits().first;
//     // 	     hit2 != seed2->recHits().second;
//     // 	     ++hit2 ) {
//     // 	  if (hit1->geographicalId() == hit2->geographicalId())
//     // 	    if (hit1->localPosition().x() == hit2->localPosition().x())
//     // 	      if (hit1->localPosition().y() == hit2->localPosition().y())
//     // 		++counter;
//     // 	}
//     //       }
//     //       if ( counter > 1 ) {
//     // 	edm::LogVerbatim("RoadSearch") << "two seeds with the same hits";
//     // 	edm::LogVerbatim("RoadSearch") << " Seed 1 hit 1 id: " << seed1->recHits().first->geographicalId().rawId()
//     // 				       << " x: " << seed1->recHits().first->localPosition().x()
//     // 				       << " y: " << seed1->recHits().first->localPosition().y()
//     // 				       << " hit 2 id: " << (--(seed1->recHits().second))->geographicalId().rawId()
//     // 				       << " x: " << (--(seed1->recHits().second))->localPosition().x()
//     // 				       << " y: " << (--(seed1->recHits().second))->localPosition().y();
//     // 	edm::LogVerbatim("RoadSearch") << " Seed 2 hit 1 id: " << seed2->recHits().first->geographicalId().rawId()
//     // 				       << " x: " << seed2->recHits().first->localPosition().x()
//     // 				       << " y: " << seed2->recHits().first->localPosition().y()
//     // 				       << " hit 2 id: " << (--(seed2->recHits().second))->geographicalId().rawId()
//     // 				       << " x: " << (--(seed2->recHits().second))->localPosition().x()
//     // 				       << " y: " << (--(seed2->recHits().second))->localPosition().y();
//     //       }
//     //     }
//     //   }


//     // RS raw clouds
//     rs_same_numRawClouds->Fill(rsRawCloudCollection->size());
//     for ( RoadSearchCloudCollection::const_iterator cloud = rsRawCloudCollection->begin();
// 	  cloud != rsRawCloudCollection->end();
// 	  ++cloud ) {

//       // loop over hits in tracks, count
//       unsigned int nHit      = 0;
//       unsigned int nStripHit = 0;
//       unsigned int nPixelHit = 0;
//       unsigned int nTIBHit   = 0;
//       unsigned int nTOBHit   = 0;
//       unsigned int nTIDHit   = 0;
//       unsigned int nTECHit   = 0;
//       unsigned int nPXBHit   = 0;
//       unsigned int nPXFHit   = 0;

//       for ( RoadSearchCloud::RecHitOwnVector::const_iterator recHit = cloud->begin_hits();
// 	    recHit != cloud->end_hits();
// 	    ++recHit ) {
// 	++nHit;
// 	DetId id(recHit->geographicalId());
      
// 	if ( (unsigned int)id.subdetId() == StripSubdetector::TIB ) {
// 	  ++nStripHit;
// 	  ++nTIBHit;
// 	} else if ( (unsigned int)id.subdetId() == StripSubdetector::TOB ) {
// 	  ++nStripHit;
// 	  ++nTOBHit;
// 	} else if ( (unsigned int)id.subdetId() == StripSubdetector::TID ) {
// 	  ++nStripHit;
// 	  ++nTIDHit;
// 	} else if ( (unsigned int)id.subdetId() == StripSubdetector::TEC ) {
// 	  ++nStripHit;
// 	  ++nTECHit;
// 	} else if ( (unsigned int)id.subdetId() == PixelSubdetector::PixelBarrel ) {
// 	  ++nPixelHit;
// 	  ++nPXBHit;
// 	} else if ( (unsigned int)id.subdetId() == PixelSubdetector::PixelEndcap ) {
// 	  ++nPixelHit;
// 	  ++nPXFHit;
// 	}
//       }
    
//       rs_same_nHitPerRawCloudVsEta->Fill(nHit,0.);
//       rs_same_nStripHitPerRawCloudVsEta->Fill(nStripHit,0.);
//       rs_same_nPixelHitPerRawCloudVsEta->Fill(nPixelHit,0.);
//       rs_same_nTIBHitPerRawCloudVsEta->Fill(nTIBHit,0.);
//       rs_same_nTOBHitPerRawCloudVsEta->Fill(nTOBHit,0.);
//       rs_same_nTIDHitPerRawCloudVsEta->Fill(nTIDHit,0.);
//       rs_same_nTECHitPerRawCloudVsEta->Fill(nTECHit,0.);
//       rs_same_nPXBHitPerRawCloudVsEta->Fill(nPXBHit,0.);
//       rs_same_nPXFHitPerRawCloudVsEta->Fill(nPXFHit,0.);

//     }

//     // RS clean clouds
//     rs_same_numCleanClouds->Fill(rsCleanCloudCollection->size());
//     for ( RoadSearchCloudCollection::const_iterator cloud = rsCleanCloudCollection->begin();
// 	  cloud != rsCleanCloudCollection->end();
// 	  ++cloud ) {

//       // loop over hits in tracks, count
//       unsigned int nHit      = 0;
//       unsigned int nStripHit = 0;
//       unsigned int nPixelHit = 0;
//       unsigned int nTIBHit   = 0;
//       unsigned int nTOBHit   = 0;
//       unsigned int nTIDHit   = 0;
//       unsigned int nTECHit   = 0;
//       unsigned int nPXBHit   = 0;
//       unsigned int nPXFHit   = 0;

//       for ( RoadSearchCloud::RecHitOwnVector::const_iterator recHit = cloud->begin_hits();
// 	    recHit != cloud->end_hits();
// 	    ++recHit ) {
// 	++nHit;
// 	DetId id(recHit->geographicalId());
      
// 	if ( (unsigned int)id.subdetId() == StripSubdetector::TIB ) {
// 	  ++nStripHit;
// 	  ++nTIBHit;
// 	} else if ( (unsigned int)id.subdetId() == StripSubdetector::TOB ) {
// 	  ++nStripHit;
// 	  ++nTOBHit;
// 	} else if ( (unsigned int)id.subdetId() == StripSubdetector::TID ) {
// 	  ++nStripHit;
// 	  ++nTIDHit;
// 	} else if ( (unsigned int)id.subdetId() == StripSubdetector::TEC ) {
// 	  ++nStripHit;
// 	  ++nTECHit;
// 	} else if ( (unsigned int)id.subdetId() == PixelSubdetector::PixelBarrel ) {
// 	  ++nPixelHit;
// 	  ++nPXBHit;
// 	} else if ( (unsigned int)id.subdetId() == PixelSubdetector::PixelEndcap ) {
// 	  ++nPixelHit;
// 	  ++nPXFHit;
// 	}
//       }
    
//       rs_same_nHitPerCleanCloudVsEta->Fill(nHit,0.);
//       rs_same_nStripHitPerCleanCloudVsEta->Fill(nStripHit,0.);
//       rs_same_nPixelHitPerCleanCloudVsEta->Fill(nPixelHit,0.);
//       rs_same_nTIBHitPerCleanCloudVsEta->Fill(nTIBHit,0.);
//       rs_same_nTOBHitPerCleanCloudVsEta->Fill(nTOBHit,0.);
//       rs_same_nTIDHitPerCleanCloudVsEta->Fill(nTIDHit,0.);
//       rs_same_nTECHitPerCleanCloudVsEta->Fill(nTECHit,0.);
//       rs_same_nPXBHitPerCleanCloudVsEta->Fill(nPXBHit,0.);
//       rs_same_nPXFHitPerCleanCloudVsEta->Fill(nPXFHit,0.);

//     }

//     // RS track candidates
//     rs_same_numTrackCandidates->Fill(rsTrackCandidateCollection->size());
//     for ( TrackCandidateCollection::const_iterator candidate = rsTrackCandidateCollection->begin();
// 	  candidate != rsTrackCandidateCollection->end();
// 	  ++candidate ) {

//       // loop over hits in tracks, count
//       unsigned int nHit      = 0;
//       unsigned int nStripHit = 0;
//       unsigned int nPixelHit = 0;
//       unsigned int nTIBHit   = 0;
//       unsigned int nTOBHit   = 0;
//       unsigned int nTIDHit   = 0;
//       unsigned int nTECHit   = 0;
//       unsigned int nPXBHit   = 0;
//       unsigned int nPXFHit   = 0;

//       for ( TrackCandidate::RecHitContainer::const_iterator recHit = candidate->recHits().first;
// 	    recHit != candidate->recHits().second;
// 	    ++recHit ) {
// 	++nHit;
// 	DetId id(recHit->geographicalId());
      
// 	if ( (unsigned int)id.subdetId() == StripSubdetector::TIB ) {
// 	  ++nStripHit;
// 	  ++nTIBHit;
// 	} else if ( (unsigned int)id.subdetId() == StripSubdetector::TOB ) {
// 	  ++nStripHit;
// 	  ++nTOBHit;
// 	} else if ( (unsigned int)id.subdetId() == StripSubdetector::TID ) {
// 	  ++nStripHit;
// 	  ++nTIDHit;
// 	} else if ( (unsigned int)id.subdetId() == StripSubdetector::TEC ) {
// 	  ++nStripHit;
// 	  ++nTECHit;
// 	} else if ( (unsigned int)id.subdetId() == PixelSubdetector::PixelBarrel ) {
// 	  ++nPixelHit;
// 	  ++nPXBHit;
// 	} else if ( (unsigned int)id.subdetId() == PixelSubdetector::PixelEndcap ) {
// 	  ++nPixelHit;
// 	  ++nPXFHit;
// 	}
//       }
    
//       rs_same_nHitPerTrackCandidateVsEta->Fill(nHit,0.);
//       rs_same_nStripHitPerTrackCandidateVsEta->Fill(nStripHit,0.);
//       rs_same_nPixelHitPerTrackCandidateVsEta->Fill(nPixelHit,0.);
//       rs_same_nTIBHitPerTrackCandidateVsEta->Fill(nTIBHit,0.);
//       rs_same_nTOBHitPerTrackCandidateVsEta->Fill(nTOBHit,0.);
//       rs_same_nTIDHitPerTrackCandidateVsEta->Fill(nTIDHit,0.);
//       rs_same_nTECHitPerTrackCandidateVsEta->Fill(nTECHit,0.);
//       rs_same_nPXBHitPerTrackCandidateVsEta->Fill(nPXBHit,0.);
//       rs_same_nPXFHitPerTrackCandidateVsEta->Fill(nPXFHit,0.);

//     }

//     // Ckf tracks
//     ckf_same_numTracks->Fill(ckfTrackCollection->size());
//     for ( reco::TrackCollection::const_iterator track = ckfTrackCollection->begin();
// 	  track != ckfTrackCollection->end();
// 	  ++track ) {
//       ckf_same_chi2->Fill(track->normalizedChi2());
//       ckf_same_pt->Fill(track->pt());
//       ckf_same_nhit->Fill(track->found());
//       ckf_same_eta->Fill(track->momentum().eta());
//       ckf_same_phi->Fill(track->momentum().phi());
//       ckf_same_transCurv->Fill(track->transverseCurvature());
//       ckf_same_phiZero->Fill(track->phi0());
//       ckf_same_theta->Fill(track->theta());
//       ckf_same_dZero->Fill(track->d0());
//       ckf_same_dZ->Fill(track->dz());

//       // loop over hits in tracks, count
//       unsigned int nHit      = 0;
//       unsigned int nStripHit = 0;
//       unsigned int nPixelHit = 0;
//       unsigned int nTIBHit   = 0;
//       unsigned int nTOBHit   = 0;
//       unsigned int nTIDHit   = 0;
//       unsigned int nTECHit   = 0;
//       unsigned int nPXBHit   = 0;
//       unsigned int nPXFHit   = 0;

//       for ( trackingRecHit_iterator recHit = track->recHitsBegin();
// 	    recHit != track->recHitsEnd();
// 	    ++ recHit ) {
// 	if ( (*recHit)->isValid() ) {
// 	  ++nHit;
// 	  DetId id((*recHit)->geographicalId());

// 	  if ( (unsigned int)id.subdetId() == StripSubdetector::TIB ) {
// 	    ++nStripHit;
// 	    ++nTIBHit;
// 	  } else if ( (unsigned int)id.subdetId() == StripSubdetector::TOB ) {
// 	    ++nStripHit;
// 	    ++nTOBHit;
// 	  } else if ( (unsigned int)id.subdetId() == StripSubdetector::TID ) {
// 	    ++nStripHit;
// 	    ++nTIDHit;
// 	  } else if ( (unsigned int)id.subdetId() == StripSubdetector::TEC ) {
// 	    ++nStripHit;
// 	    ++nTECHit;
// 	  } else if ( (unsigned int)id.subdetId() == PixelSubdetector::PixelBarrel ) {
// 	    ++nPixelHit;
// 	    ++nPXBHit;
// 	  } else if ( (unsigned int)id.subdetId() == PixelSubdetector::PixelEndcap ) {
// 	    ++nPixelHit;
// 	    ++nPXFHit;
// 	  }
// 	}
//       }
    
//       ckf_same_nHitPerTrackVsEta->Fill(nHit,track->momentum().eta());
//       ckf_same_nStripHitPerTrackVsEta->Fill(nStripHit,track->momentum().eta());
//       ckf_same_nPixelHitPerTrackVsEta->Fill(nPixelHit,track->momentum().eta());
//       ckf_same_nTIBHitPerTrackVsEta->Fill(nTIBHit,track->momentum().eta());
//       ckf_same_nTOBHitPerTrackVsEta->Fill(nTOBHit,track->momentum().eta());
//       ckf_same_nTIDHitPerTrackVsEta->Fill(nTIDHit,track->momentum().eta());
//       ckf_same_nTECHitPerTrackVsEta->Fill(nTECHit,track->momentum().eta());
//       ckf_same_nPXBHitPerTrackVsEta->Fill(nPXBHit,track->momentum().eta());
//       ckf_same_nPXFHitPerTrackVsEta->Fill(nPXFHit,track->momentum().eta());

//     }

//     ckf_same_numSeeds->Fill(ckfSeedCollection->size());

//     // CKF track candidates
//     ckf_same_numTrackCandidates->Fill(ckfTrackCandidateCollection->size());
//     for ( TrackCandidateCollection::const_iterator candidate = ckfTrackCandidateCollection->begin();
// 	  candidate != ckfTrackCandidateCollection->end();
// 	  ++candidate ) {

//       // loop over hits in tracks, count
//       unsigned int nHit      = 0;
//       unsigned int nStripHit = 0;
//       unsigned int nPixelHit = 0;
//       unsigned int nTIBHit   = 0;
//       unsigned int nTOBHit   = 0;
//       unsigned int nTIDHit   = 0;
//       unsigned int nTECHit   = 0;
//       unsigned int nPXBHit   = 0;
//       unsigned int nPXFHit   = 0;

//       for ( TrackCandidate::RecHitContainer::const_iterator recHit = candidate->recHits().first;
// 	    recHit != candidate->recHits().second;
// 	    ++recHit ) {
// 	++nHit;
// 	DetId id(recHit->geographicalId());
      
// 	if ( (unsigned int)id.subdetId() == StripSubdetector::TIB ) {
// 	  ++nStripHit;
// 	  ++nTIBHit;
// 	} else if ( (unsigned int)id.subdetId() == StripSubdetector::TOB ) {
// 	  ++nStripHit;
// 	  ++nTOBHit;
// 	} else if ( (unsigned int)id.subdetId() == StripSubdetector::TID ) {
// 	  ++nStripHit;
// 	  ++nTIDHit;
// 	} else if ( (unsigned int)id.subdetId() == StripSubdetector::TEC ) {
// 	  ++nStripHit;
// 	  ++nTECHit;
// 	} else if ( (unsigned int)id.subdetId() == PixelSubdetector::PixelBarrel ) {
// 	  ++nPixelHit;
// 	  ++nPXBHit;
// 	} else if ( (unsigned int)id.subdetId() == PixelSubdetector::PixelEndcap ) {
// 	  ++nPixelHit;
// 	  ++nPXFHit;
// 	}
//       }
    
//       ckf_same_nHitPerTrackCandidateVsEta->Fill(nHit,0.);
//       ckf_same_nStripHitPerTrackCandidateVsEta->Fill(nStripHit,0.);
//       ckf_same_nPixelHitPerTrackCandidateVsEta->Fill(nPixelHit,0.);
//       ckf_same_nTIBHitPerTrackCandidateVsEta->Fill(nTIBHit,0.);
//       ckf_same_nTOBHitPerTrackCandidateVsEta->Fill(nTOBHit,0.);
//       ckf_same_nTIDHitPerTrackCandidateVsEta->Fill(nTIDHit,0.);
//       ckf_same_nTECHitPerTrackCandidateVsEta->Fill(nTECHit,0.);
//       ckf_same_nPXBHitPerTrackCandidateVsEta->Fill(nPXBHit,0.);
//       ckf_same_nPXFHitPerTrackCandidateVsEta->Fill(nPXFHit,0.);

//     }

//   }

}


void 
RoadSearchTrackValidation::beginJob(const edm::EventSetup&)
{
}

void 
RoadSearchTrackValidation::endJob() {
}
