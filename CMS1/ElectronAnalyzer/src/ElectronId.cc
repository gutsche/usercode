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
  
  std::vector<const reco::Candidate*> electrons = theElectrons.getElectrons(Electrons::AllElectrons, Cuts());

  elidUserData.fill(theData, electrons);  
  nCandidates->addData(electrons.size());

}

void cms1::ElectronId::configure(const edm::ParameterSet& iConfig) {
  elidUserData.registerBlock(theData, "", "cms1_");
  theData.intUserData.push_back( new UserData<int>("nCand", "evt_", "cms1_evt_", false) );
  nCandidates = theData.intUserData.back();
}
