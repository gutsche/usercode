//
// Package:         CMS1/MuonAnalyzer
// Class:           MuonAnalyzer

// 
// Description:     EDAnalyzer using cms1 muons object
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Mon Jan 29 17:42:34 UTC 2007
//
// $Author: gutsche $
// $Date: 2007/01/22 01:35:05 $
// $Revision: 1.2 $
//

#include "CMS1/MuonAnalyzer/interface/MuonAnalyzer.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/MuonReco/interface/Muon.h"

cms1::MuonAnalyzer::MuonAnalyzer(const edm::ParameterSet& iConfig)
{

  // input tags
  globalMuonInputTag_ = iConfig.getUntrackedParameter<edm::InputTag>("GlobalMuonInputTag");

  // parameter
  number_             = iConfig.getUntrackedParameter<unsigned int>("number");
  pt_                 = iConfig.getUntrackedParameter<double>("pt");

}


cms1::MuonAnalyzer::~MuonAnalyzer()
{
 
}


void
cms1::MuonAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  // get muon collection
  const reco::MuonCollection *muonCollection = 0;
  edm::Handle<reco::MuonCollection> muonCollectionHandle;
  iEvent.getByLabel(globalMuonInputTag_,muonCollectionHandle);
  muonCollection = muonCollectionHandle.product();

  if ( muons_.GlobalMuons(muonCollection,number_,pt_) ) {
    edm::LogInfo("CMS1MuonAnalyzer") << "Two or more muons found with pT > 2 GeV";
  }

}


void 
cms1::MuonAnalyzer::beginJob(const edm::EventSetup& setup)
{
}

void 
cms1::MuonAnalyzer::endJob() {
}
