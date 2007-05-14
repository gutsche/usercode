//
// Package:         CMS1/MCInfo
// Class:           testMCInfo
//

#include "CMS1/MCInfo/interface/testMCInfo.h"
#include "CMS1/Base/interface/Cuts.h"

void cms1::testMCInfo::configure(const edm::ParameterSet& iConfig) {

   // some cuts
  ept_min = iConfig.getUntrackedParameter<double>("ElPtMinCut", 0.);
  mpt_min = iConfig.getUntrackedParameter<double>("MuPtMinCut", 0.);
  tpt_min = iConfig.getUntrackedParameter<double>("TauPtMinCut", 0.);
  jet_min = iConfig.getUntrackedParameter<double>("JetEtMinCut", 0.);
}

void cms1::testMCInfo::processEvent(const edm::Event& iEvent) {
  Cuts elCut, muCut, tauCut, jetCut;
  elCut.pt_min = ept_min;
  muCut.pt_min = mpt_min;
  tauCut.pt_min = tpt_min;
  jetCut.pt_min = jet_min;

  // dump the MC information of the event without cut
  theMCInfo.dumpLeptons();
  theMCInfo.dumpJets();
  
  // dump the MC information of the event with cut
  theMCInfo.dumpLeptons(elCut, muCut, tauCut);
  theMCInfo.dumpJets(jetCut);
}

