#ifndef RoadSearchTrackValidation_h
#define RoadSearchTrackValidation_h

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
// $Date: 2006/09/01 16:49:02 $
// $Revision: 1.1 $
//

#include <string>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH1I.h"
#include "TH2D.h"

class RoadSearchTrackValidation : public edm::EDAnalyzer {
 public:
  explicit RoadSearchTrackValidation(const edm::ParameterSet&);
  ~RoadSearchTrackValidation();

 private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  TFile* file_;

  TH1D* rs_chi2;
  TH1D* rs_pt;
  TH1D* rs_eta;
  TH1D* rs_phi;
  TH1D* rs_transCurv;
  TH1D* rs_phiZero;
  TH1D* rs_theta;
  TH1D* rs_dZero;
  TH1D* rs_dZ;
  TH1D* rs_nhit;

  TH1I* rs_numSeeds;
  TH1I* rs_numRawClouds;
  TH1I* rs_numCleanClouds;
  TH1I* rs_numTrackCandidates;
  TH1I* rs_numTracks;

  TH2D* rs_nHitPerTrackVsEta;
  TH2D* rs_nHitPerTrackCandidateVsEta;
  TH2D* rs_nHitPerCleanCloudVsEta;
  TH2D* rs_nHitPerRawCloudVsEta;
  TH2D* rs_nStripHitPerTrackVsEta;
  TH2D* rs_nStripHitPerTrackCandidateVsEta;
  TH2D* rs_nStripHitPerCleanCloudVsEta;
  TH2D* rs_nStripHitPerRawCloudVsEta;
  TH2D* rs_nPixelHitPerTrackVsEta;
  TH2D* rs_nPixelHitPerTrackCandidateVsEta;
  TH2D* rs_nPixelHitPerCleanCloudVsEta;
  TH2D* rs_nPixelHitPerRawCloudVsEta;
  TH2D* rs_nTIBHitPerTrackVsEta;
  TH2D* rs_nTIBHitPerTrackCandidateVsEta;
  TH2D* rs_nTIBHitPerCleanCloudVsEta;
  TH2D* rs_nTIBHitPerRawCloudVsEta;
  TH2D* rs_nTOBHitPerTrackVsEta;
  TH2D* rs_nTOBHitPerTrackCandidateVsEta;
  TH2D* rs_nTOBHitPerCleanCloudVsEta;
  TH2D* rs_nTOBHitPerRawCloudVsEta;
  TH2D* rs_nTIDHitPerTrackVsEta;
  TH2D* rs_nTIDHitPerTrackCandidateVsEta;
  TH2D* rs_nTIDHitPerCleanCloudVsEta;
  TH2D* rs_nTIDHitPerRawCloudVsEta;
  TH2D* rs_nTECHitPerTrackVsEta;
  TH2D* rs_nTECHitPerTrackCandidateVsEta;
  TH2D* rs_nTECHitPerCleanCloudVsEta;
  TH2D* rs_nTECHitPerRawCloudVsEta;
  TH2D* rs_nPXBHitPerTrackVsEta;
  TH2D* rs_nPXBHitPerTrackCandidateVsEta;
  TH2D* rs_nPXBHitPerCleanCloudVsEta;
  TH2D* rs_nPXBHitPerRawCloudVsEta;
  TH2D* rs_nPXFHitPerTrackVsEta;
  TH2D* rs_nPXFHitPerTrackCandidateVsEta;
  TH2D* rs_nPXFHitPerCleanCloudVsEta;
  TH2D* rs_nPXFHitPerRawCloudVsEta;
  TH2D* rs_nHitPerTrackVsPhi;

  TH1D* ckf_chi2;
  TH1D* ckf_pt;
  TH1D* ckf_eta;
  TH1D* ckf_phi;
  TH1D* ckf_transCurv;
  TH1D* ckf_phiZero;
  TH1D* ckf_theta;
  TH1D* ckf_dZero;
  TH1D* ckf_dZ;
  TH1D* ckf_nhit;

  TH1I* ckf_numSeeds;
  TH1I* ckf_numRawClouds;
  TH1I* ckf_numCleanClouds;
  TH1I* ckf_numTrackCandidates;
  TH1I* ckf_numTracks;

  TH2D* ckf_nHitPerTrackVsEta;
  TH2D* ckf_nHitPerTrackCandidateVsEta;
  TH2D* ckf_nStripHitPerTrackVsEta;
  TH2D* ckf_nStripHitPerTrackCandidateVsEta;
  TH2D* ckf_nPixelHitPerTrackVsEta;
  TH2D* ckf_nPixelHitPerTrackCandidateVsEta;
  TH2D* ckf_nTIBHitPerTrackVsEta;
  TH2D* ckf_nTIBHitPerTrackCandidateVsEta;
  TH2D* ckf_nTOBHitPerTrackVsEta;
  TH2D* ckf_nTOBHitPerTrackCandidateVsEta;
  TH2D* ckf_nTIDHitPerTrackVsEta;
  TH2D* ckf_nTIDHitPerTrackCandidateVsEta;
  TH2D* ckf_nTECHitPerTrackVsEta;
  TH2D* ckf_nTECHitPerTrackCandidateVsEta;
  TH2D* ckf_nPXBHitPerTrackVsEta;
  TH2D* ckf_nPXBHitPerTrackCandidateVsEta;
  TH2D* ckf_nPXFHitPerTrackVsEta;
  TH2D* ckf_nPXFHitPerTrackCandidateVsEta;
  TH2D* ckf_nHitPerTrackVsPhi;

  // histograms if RS and Ckf have the same number of tracks (selecteable)
  TH1D* rs_same_chi2;
  TH1D* rs_same_pt;
  TH1D* rs_same_eta;
  TH1D* rs_same_phi;
  TH1D* rs_same_transCurv;
  TH1D* rs_same_phiZero;
  TH1D* rs_same_theta;
  TH1D* rs_same_dZero;
  TH1D* rs_same_dZ;
  TH1D* rs_same_nhit;

  TH1I* rs_same_numSeeds;
  TH1I* rs_same_numRawClouds;
  TH1I* rs_same_numCleanClouds;
  TH1I* rs_same_numTrackCandidates;
  TH1I* rs_same_numTracks;

  TH2D* rs_same_nHitPerTrackVsEta;
  TH2D* rs_same_nHitPerTrackCandidateVsEta;
  TH2D* rs_same_nHitPerCleanCloudVsEta;
  TH2D* rs_same_nHitPerRawCloudVsEta;
  TH2D* rs_same_nStripHitPerTrackVsEta;
  TH2D* rs_same_nStripHitPerTrackCandidateVsEta;
  TH2D* rs_same_nStripHitPerCleanCloudVsEta;
  TH2D* rs_same_nStripHitPerRawCloudVsEta;
  TH2D* rs_same_nPixelHitPerTrackVsEta;
  TH2D* rs_same_nPixelHitPerTrackCandidateVsEta;
  TH2D* rs_same_nPixelHitPerCleanCloudVsEta;
  TH2D* rs_same_nPixelHitPerRawCloudVsEta;
  TH2D* rs_same_nTIBHitPerTrackVsEta;
  TH2D* rs_same_nTIBHitPerTrackCandidateVsEta;
  TH2D* rs_same_nTIBHitPerCleanCloudVsEta;
  TH2D* rs_same_nTIBHitPerRawCloudVsEta;
  TH2D* rs_same_nTOBHitPerTrackVsEta;
  TH2D* rs_same_nTOBHitPerTrackCandidateVsEta;
  TH2D* rs_same_nTOBHitPerCleanCloudVsEta;
  TH2D* rs_same_nTOBHitPerRawCloudVsEta;
  TH2D* rs_same_nTIDHitPerTrackVsEta;
  TH2D* rs_same_nTIDHitPerTrackCandidateVsEta;
  TH2D* rs_same_nTIDHitPerCleanCloudVsEta;
  TH2D* rs_same_nTIDHitPerRawCloudVsEta;
  TH2D* rs_same_nTECHitPerTrackVsEta;
  TH2D* rs_same_nTECHitPerTrackCandidateVsEta;
  TH2D* rs_same_nTECHitPerCleanCloudVsEta;
  TH2D* rs_same_nTECHitPerRawCloudVsEta;
  TH2D* rs_same_nPXBHitPerTrackVsEta;
  TH2D* rs_same_nPXBHitPerTrackCandidateVsEta;
  TH2D* rs_same_nPXBHitPerCleanCloudVsEta;
  TH2D* rs_same_nPXBHitPerRawCloudVsEta;
  TH2D* rs_same_nPXFHitPerTrackVsEta;
  TH2D* rs_same_nPXFHitPerTrackCandidateVsEta;
  TH2D* rs_same_nPXFHitPerCleanCloudVsEta;
  TH2D* rs_same_nPXFHitPerRawCloudVsEta;

  TH1D* ckf_same_chi2;
  TH1D* ckf_same_pt;
  TH1D* ckf_same_eta;
  TH1D* ckf_same_phi;
  TH1D* ckf_same_transCurv;
  TH1D* ckf_same_phiZero;
  TH1D* ckf_same_theta;
  TH1D* ckf_same_dZero;
  TH1D* ckf_same_dZ;
  TH1D* ckf_same_nhit;

  TH1I* ckf_same_numSeeds;
  TH1I* ckf_same_numRawClouds;
  TH1I* ckf_same_numCleanClouds;
  TH1I* ckf_same_numTrackCandidates;
  TH1I* ckf_same_numTracks;

  TH2D* ckf_same_nHitPerTrackVsEta;
  TH2D* ckf_same_nHitPerTrackCandidateVsEta;
  TH2D* ckf_same_nStripHitPerTrackVsEta;
  TH2D* ckf_same_nStripHitPerTrackCandidateVsEta;
  TH2D* ckf_same_nPixelHitPerTrackVsEta;
  TH2D* ckf_same_nPixelHitPerTrackCandidateVsEta;
  TH2D* ckf_same_nTIBHitPerTrackVsEta;
  TH2D* ckf_same_nTIBHitPerTrackCandidateVsEta;
  TH2D* ckf_same_nTOBHitPerTrackVsEta;
  TH2D* ckf_same_nTOBHitPerTrackCandidateVsEta;
  TH2D* ckf_same_nTIDHitPerTrackVsEta;
  TH2D* ckf_same_nTIDHitPerTrackCandidateVsEta;
  TH2D* ckf_same_nTECHitPerTrackVsEta;
  TH2D* ckf_same_nTECHitPerTrackCandidateVsEta;
  TH2D* ckf_same_nPXBHitPerTrackVsEta;
  TH2D* ckf_same_nPXBHitPerTrackCandidateVsEta;
  TH2D* ckf_same_nPXFHitPerTrackVsEta;
  TH2D* ckf_same_nPXFHitPerTrackCandidateVsEta;

  std::string rsSeedProducerLabel_;
  std::string rsRawCloudProducerLabel_;
  std::string rsCleanCloudProducerLabel_;
  std::string rsTrackCandidateProducerLabel_;
  std::string rsTrackProducerLabel_;

  std::string ckfSeedProducerLabel_;
  std::string ckfTrackCandidateProducerLabel_;
  std::string ckfTrackProducerLabel_;

  unsigned int sameNumberOfTracks;

};

#endif
