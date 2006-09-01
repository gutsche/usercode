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
// $Date: 2006/03/28 22:54:06 $
// $Revision: 1.3 $
//

#include <string>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH1I.h"

class RoadSearchTrackValidation : public edm::EDAnalyzer {
 public:
  explicit RoadSearchTrackValidation(const edm::ParameterSet&);
  ~RoadSearchTrackValidation();

 private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  TFile* file_;
  TH1F* rs_chi2;
  TH1F* rs_pt;
  TH1F* rs_nhit;
  TH1F* rs_eta;
  TH1F* rs_phi;
  TH1F* rs_transCurv;
  TH1F* rs_phiZero;
  TH1F* rs_theta;
  TH1F* rs_dZero;
  TH1F* rs_dZ;
  TH1I* rs_numSeeds;
  TH1I* rs_numRawClouds;
  TH1I* rs_numCleanClouds;
  TH1I* rs_numTrackCandidates;
  TH1I* rs_numTracks;

  TH1F* ckf_chi2;
  TH1F* ckf_pt;
  TH1F* ckf_nhit;
  TH1F* ckf_eta;
  TH1F* ckf_phi;
  TH1F* ckf_transCurv;
  TH1F* ckf_phiZero;
  TH1F* ckf_theta;
  TH1F* ckf_dZero;
  TH1F* ckf_dZ;
  TH1I* ckf_numSeeds;
  TH1I* ckf_numRawClouds;
  TH1I* ckf_numCleanClouds;
  TH1I* ckf_numTrackCandidates;
  TH1I* ckf_numTracks;

  std::string rsSeedProducerLabel_;
  std::string rsRawCloudProducerLabel_;
  std::string rsCleanCloudProducerLabel_;
  std::string rsTrackCandidateProducerLabel_;
  std::string rsTrackProducerLabel_;

  std::string ckfSeedProducerLabel_;
  std::string ckfTrackCandidateProducerLabel_;
  std::string ckfTrackProducerLabel_;

};

#endif
