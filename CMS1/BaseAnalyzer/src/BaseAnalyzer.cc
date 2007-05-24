//
// Package:         CMS1/Base
// Class:           BaseAnalyzer
// 
// Description:     abstract base class for TnS analysis
//
// Original Author: Dmytro Kovalskyi
//
// $Author: dmytro $
// $Date: 2007/05/22 07:14:58 $
// $Revision: 1.7 $
//
#include "CMS1/BaseAnalyzer/interface/BaseAnalyzer.h"
#include "FWCore/Framework/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"

#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"

void cms1::BaseAnalyzer::configure(const edm::ParameterSet& iConfig)
{
   // let black boxes know where to read data from
   theMuons.setEventData     ( &theData );
   theElectrons.setEventData ( &theData );
   theJets.setEventData      ( &theData );
   theMET.setEventData       ( &theData );
   theMCInfo.setEventData    ( &theData );   

   genJetInputTag_ = iConfig.getUntrackedParameter<edm::InputTag>("GenJetInputTag", edm::InputTag("midPointCone5GenJets"));
   makeNtuples = iConfig.getUntrackedParameter<bool>("makeNtuples");
   candidateBasedNtuples = iConfig.getUntrackedParameter<bool>("candidateBasedNtuples");
   std::string ntupleFileName = iConfig.getUntrackedParameter<std::string>("fileName");
   
   theTracks.setEventData    ( &theData );
   
   if ( makeNtuples ) {
      // ntuples & userdata registration
      theMuons.registerEventUserData();
      theTracks.registerEventUserData();
      theElectrons.registerEventUserData();
      theJets.registerEventUserData();
      theMET.registerEventUserData();
   
      // ntuple stuff
      theRootFile = new TFile(ntupleFileName.c_str(),"RECREATE");
      theTree = new TTree("event","Event data");
   }
   branchesInitialized = false;
}

void cms1::BaseAnalyzer::processEvent(const edm::Event& iEvent)
{
   if (! branchesInitialized && makeNtuples) {
      theData.addBranches(*theTree, candidateBasedNtuples);
      branchesInitialized = true;
   }
   
   theData.iEvent = &iEvent;

   // ntuples
   if (makeNtuples) fillUserData( theData );
   
   // get MC Event
   edm::Handle<edm::HepMCProduct> mcCollectionHandle;
   iEvent.getByLabel("VtxSmeared", mcCollectionHandle);
   const HepMC::GenEvent* genEvent = mcCollectionHandle->GetEvent();
   theData.mcInfo.clear();
   for (HepMC::GenEvent::particle_const_iterator p = genEvent->particles_begin(); p != genEvent->particles_end(); ++p)
     theData.mcInfo.push_back(**p);

   edm::Handle<reco::GenJetCollection> jetCollectionHandle;
   iEvent.getByLabel(genJetInputTag_,jetCollectionHandle);
   theData.jetInfo = *(jetCollectionHandle.product());
}

void cms1::BaseAnalyzer::finishEvent()
{
   // store data in the ntuple
   if (makeNtuples) theData.fillEvent(*theTree, candidateBasedNtuples);
   // clear stored data to avoid memory leaks
   if (makeNtuples) theData.clearUserData();
}

void cms1::BaseAnalyzer::fillUserData( EventData& event )
{
   // fill event data for ntuples and userdata
   if (makeNtuples){
      theMuons.fillEventUserData();
      theTracks.fillEventUserData();
      theElectrons.fillEventUserData();
      theJets.fillEventUserData();
      theMET.fillEventUserData();
   }
}

void cms1::BaseAnalyzer::finishProcessing()
{
   if (makeNtuples) theRootFile->Write();
}

