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

#include "RoadSearchValidation/RoadSearchTrackValidation/interface/RoadSearchTrackValidation.h"

#include "DataFormats/TrajectorySeed/interface/TrajectorySeed.h"
#include "DataFormats/TrajectorySeed/interface/TrajectorySeedCollection.h"
#include "DataFormats/RoadSearchCloud/interface/RoadSearchCloud.h"
#include "DataFormats/RoadSearchCloud/interface/RoadSearchCloudCollection.h"
#include "DataFormats/TrackCandidate/interface/TrackCandidate.h"
#include "DataFormats/TrackCandidate/interface/TrackCandidateCollection.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

RoadSearchTrackValidation::RoadSearchTrackValidation(const edm::ParameterSet& iConfig)
{

  file_ = new TFile(iConfig.getUntrackedParameter<std::string>("OutputFileName").c_str(),"RECREATE");
  
  // RoadSearch plots
  rs_chi2               = new TH1F("rs_chi2","Track reduced chi^{2}",50,0.,50.);
  rs_pt                 = new TH1F("rs_pt","Track p_{T} (GeV)",40,0.0,20.0);
  rs_nhit               = new TH1F("rs_nhit","Number of Hits",31,-0.5,30.5);
  rs_eta                = new TH1F("rs_eta","Track #eta",30,-3.,3.);
  rs_phi                = new TH1F("rs_phi","Track #phi",36,-3.1416,3.1416);
  rs_transCurv          = new TH1F("rs_transCurv","Transverse curvature",50,-0.5,0.5);
  rs_phiZero            = new TH1F("rs_phiZero","phiZero",100,-5.,5.);
  rs_theta              = new TH1F("rs_theta","Theta",40,0.,4.);
  rs_dZero              = new TH1F("rs_dZero","dZero",100,-50.,50.);
  rs_dZ                 = new TH1F("rs_dZ","dZ",100,-100.,100.);
  rs_numSeeds           = new TH1I("rs_numSeeds","Number of Seeds per event",100000,0,100000);
  rs_numRawClouds       = new TH1I("rs_numRawClouds","Number of raw clouds per event",100000,0,100000);
  rs_numCleanClouds     = new TH1I("rs_numCleanClouds","Number of clean clouds per event",100000,0,100000);
  rs_numTrackCandidates = new TH1I("rs_numTrackCandidates","Number of track candidates per event",100000,0,100000);
  rs_numTracks          = new TH1I("rs_numTracks","Number of tracks per event",100000,0,100000);

  // CkF plots
  ckf_chi2               = new TH1F("ckf_chi2","Track reduced chi^{2}",50,0.,50.);
  ckf_pt                 = new TH1F("ckf_pt","Track p_{T} (GeV)",40,0.0,20.0);
  ckf_nhit               = new TH1F("ckf_nhit","Number of Hits",31,-0.5,30.5);
  ckf_eta                = new TH1F("ckf_eta","Track #eta",30,-3.,3.);
  ckf_phi                = new TH1F("ckf_phi","Track #phi",36,-3.1416,3.1416);
  ckf_transCurv          = new TH1F("ckf_transCurv","Transverse curvature",50,-0.5,0.5);
  ckf_phiZero            = new TH1F("ckf_phiZero","phiZero",100,-5.,5.);
  ckf_theta              = new TH1F("ckf_theta","Theta",40,0.,4.);
  ckf_dZero              = new TH1F("ckf_dZero","dZero",100,-50.,50.);
  ckf_dZ                 = new TH1F("ckf_dZ","dZ",100,-100.,100.);
  ckf_numSeeds           = new TH1I("ckf_numSeeds","Number of Seeds per event",100000,0,100000);
  ckf_numRawClouds       = new TH1I("ckf_numRawClouds","Number of raw clouds per event",100000,0,100000);
  ckf_numCleanClouds     = new TH1I("ckf_numCleanClouds","Number of clean clouds per event",100000,0,100000);
  ckf_numTrackCandidates = new TH1I("ckf_numTrackCandidates","Number of track candidates per event",100000,0,100000);
  ckf_numTracks          = new TH1I("ckf_numTracks","Number of tracks per event",100000,0,100000);
  
  rsSeedProducerLabel_ = iConfig.getUntrackedParameter<std::string>("rsSeedProducerLabel");
  rsRawCloudProducerLabel_ = iConfig.getUntrackedParameter<std::string>("rsRawCloudProducerLabel");
  rsCleanCloudProducerLabel_ = iConfig.getUntrackedParameter<std::string>("rsCleanCloudProducerLabel");
  rsTrackCandidateProducerLabel_ = iConfig.getUntrackedParameter<std::string>("rsTrackCandidateProducerLabel");
  rsTrackProducerLabel_ = iConfig.getUntrackedParameter<std::string>("rsTrackProducerLabel");

  ckfSeedProducerLabel_ = iConfig.getUntrackedParameter<std::string>("ckfSeedProducerLabel");
  ckfTrackCandidateProducerLabel_ = iConfig.getUntrackedParameter<std::string>("ckfTrackCandidateProducerLabel");
  ckfTrackProducerLabel_ = iConfig.getUntrackedParameter<std::string>("ckfTrackProducerLabel");

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
  }

  rs_numSeeds->Fill(rsSeedCollection->size());
  rs_numRawClouds->Fill(rsRawCloudCollection->size());
  rs_numCleanClouds->Fill(rsCleanCloudCollection->size());
  rs_numTrackCandidates->Fill(rsTrackCandidateCollection->size());
  rs_numTracks->Fill(rsTrackCollection->size());
  
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
  }

  ckf_numSeeds->Fill(ckfSeedCollection->size());
  ckf_numTrackCandidates->Fill(ckfTrackCandidateCollection->size());
  ckf_numTracks->Fill(ckfTrackCollection->size());

}


void 
RoadSearchTrackValidation::beginJob(const edm::EventSetup&)
{
}

void 
RoadSearchTrackValidation::endJob() {
}
