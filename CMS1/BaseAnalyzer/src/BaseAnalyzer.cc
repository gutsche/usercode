//
// Package:         CMS1/Base
// Class:           BaseAnalyzer
// 
// Description:     abstract base class for TnS analysis
//
// Original Author: Dmytro Kovalskyi
//
// $Author: dmytro $
// $Date: 2007/04/17 04:51:18 $
// $Revision: 1.4 $
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
   theTracks.setEventData    ( &theData );
   
   // ntuples & userdata registration
   theMuons.registerEventUserData();
   theTracks.registerEventUserData();
   theElectrons.registerEventUserData();
   theJets.registerEventUserData();
   
   // ntuple stuff
   theRootFile = new TFile("ntuple.root","RECREATE");
   theTree = new TTree("event","Event data");
   
   branchesInitialized = false;
}

void cms1::BaseAnalyzer::processEvent(const edm::Event& iEvent)
{
   if (! branchesInitialized) {
      theData.addBranches(*theTree);
      branchesInitialized = true;
   }
   
   theData.iEvent = &iEvent;
   
   // ntuples
   fillUserData( theData );
   
   // get MC Event
   edm::Handle<edm::HepMCProduct> mcCollectionHandle;
   iEvent.getByLabel("VtxSmeared", mcCollectionHandle);
   const HepMC::GenEvent* genEvent = mcCollectionHandle->GetEvent();
   theData.mcInfo.clear();
   for (HepMC::GenEvent::particle_const_iterator p = genEvent->particles_begin(); p != genEvent->particles_end(); ++p)
     theData.mcInfo.push_back(**p);
}

void cms1::BaseAnalyzer::finishEvent()
{
   // store data in the ntuple
   theTree->Fill();
   // clear stored data to avoid memory leaks
   theData.clearUserData();
}

void cms1::BaseAnalyzer::fillUserData( EventData& event )
{
   // fill event data for ntuples and userdata
   theMuons.fillEventUserData();
   theTracks.fillEventUserData();
   theElectrons.fillEventUserData();
   theJets.fillEventUserData();
}

void cms1::BaseAnalyzer::finishProcessing()
{
   theRootFile->Write();
}

