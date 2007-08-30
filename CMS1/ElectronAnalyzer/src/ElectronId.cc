//
// Package:         CMS1/ElectronAnalyzer
// Class:           ElectronId

// 
// Description:     EDAnalyzer filling ntuple for ElectronId
//
// Original Author: Matteo Sani, matteo.sani@cern.ch

#include <vector>
#include <algorithm> 
#include <cmath>
#include <utility>
#include <sstream>
#include <iomanip>

#include "CMS1/ElectronAnalyzer/interface/ElectronId.h"
#include "CMS1/TableMaker/src/UtilFunctions_isolation.h"

void cms1::ElectronId::finishProcessing() {

  // let the base analyzer finish its work
  BaseAnalyzer::finishProcessing();
}

void cms1::ElectronId::processEvent(const edm::Event& iEvent) {
  
  std::vector<const reco::Candidate*> electrons = theElectrons.getElectrons("AllElectrons", Cuts());
  std::vector<const HepMC::GenParticle*> mcel = theMCInfo.getMCInfo(MCInfo::Electrons, Cuts());
  elidUserData.fill(theData, electrons, mcel);  
  nCandidates->addData(electrons.size());
  nPythiaEl->addData(mcel.size());

}

void cms1::ElectronId::configure(const edm::ParameterSet& iConfig) {
  elidUserData.registerBlock(theData, "", "");
  theData.intUserData.push_back( new UserData<int>("elid_nCand", "???", false) );
  nCandidates = theData.intUserData.back();
  theData.intUserData.push_back( new UserData<int>("elid_nPythia", "???", false) );
  nPythiaEl = theData.intUserData.back();
}
