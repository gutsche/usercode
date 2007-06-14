//
// Package:         CMS1/Base
// Class:           BaseAnalyzer
// 
// Description:     abstract base class for TnS analysis
//
// Original Author: Dmytro Kovalskyi
//
// $Author: dmytro $
// $Date: 2007/06/02 18:03:49 $
// $Revision: 1.11 $
//
#include "CMS1/BaseAnalyzer/interface/BaseAnalyzer.h"
#include "FWCore/Framework/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"

#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "Utilities/Timing/interface/TimingReport.h"
#include "TrackingTools/TrackAssociator/interface/TimerStack.h"

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
      theMCInfo.registerEventUserData();
      
      // ntuple stuff
      theRootFile = new TFile(ntupleFileName.c_str(),"RECREATE");
      theTree = new TTree("event","Event data");
      legoPtr = new TH2F("lego","CaloTower Et distribution",120,-5.220,5.220,36,-3.1416,3.1416);
   }
   branchesInitialized = false;
}

void cms1::BaseAnalyzer::processEvent(const edm::Event& iEvent)
{
   TimerStack timers;
   timers.push("BaseAnalyzer::processEvent");
   if (! branchesInitialized && makeNtuples) {
      theData.addBranches(*theTree, candidateBasedNtuples);
      branchesInitialized = true;
      // event display
      theTree->Branch("evt_lego","TH2F",&legoPtr);
   }
   
   timers.push("BaseAnalyzer::processEvent::makeLegoPlot");
   // fill CaloTowers
   edm::Handle<CaloTowerCollection> caloTowers;
   iEvent.getByLabel( "towerMaker", caloTowers );
   legoPtr->Reset();
   for(CaloTowerCollection::const_iterator tower = caloTowers->begin(); tower != caloTowers->end(); ++tower)
     legoPtr->Fill(tower->eta(), tower->phi(), tower->et());
   
   theData.setEvent( &iEvent );
   
   timers.pop_and_push("BaseAnalyzer::processEvent::getMCInfo");
   // get MC Event
   edm::Handle<edm::HepMCProduct> mcCollectionHandle;
   iEvent.getByLabel("VtxSmeared", mcCollectionHandle);
   const HepMC::GenEvent* genEvent = mcCollectionHandle->GetEvent();
   theData.mcInfo.clear();
   for (HepMC::GenEvent::particle_const_iterator p = genEvent->particles_begin(); p != genEvent->particles_end(); ++p)
     theData.mcInfo.push_back(*p);

   edm::Handle<reco::GenJetCollection> jetCollectionHandle;
   iEvent.getByLabel(genJetInputTag_,jetCollectionHandle);
   theData.jetInfo = *(jetCollectionHandle.product());

   timers.pop_and_push("BaseAnalyzer::processEvent::fillUserData");
   // ntuples
   if (makeNtuples) fillUserData( theData );
}

void cms1::BaseAnalyzer::finishEvent()
{
   TimerStack timers;
   timers.push("BaseAnalyzer::finishEvent");
   // store data in the ntuple
   if (makeNtuples) theData.fillEvent(*theTree, candidateBasedNtuples);
   // clear stored data to avoid memory leaks
   if (makeNtuples) theData.clearUserData();
}

void cms1::BaseAnalyzer::fillUserData( EventData& event )
{
   // fill event data for ntuples and userdata
   // please don't mess with the order - it's important, since BlackBoxes
   // at the end might need output of earlier block outputs. Currently
   // Muon,Track, Electron and Jet BBs create reference collections in
   // the EventData. MET uses this output.
   if (makeNtuples){
      TimerStack timers;
      timers.push("BaseAnalyzer::fillUserData::Event::Muons");
      theMuons.fillEventUserData();
      timers.pop_and_push("BaseAnalyzer::fillUserData::Event::Tracks");
      theTracks.fillEventUserData();
      timers.pop_and_push("BaseAnalyzer::fillUserData::Event::Electrons");
      theElectrons.fillEventUserData();
      timers.pop_and_push("BaseAnalyzer::fillUserData::Event::Jets");
      theJets.fillEventUserData();
      timers.pop_and_push("BaseAnalyzer::fillUserData::Event::MET");
      theMET.fillEventUserData();
      timers.pop_and_push("BaseAnalyzer::fillUserData::Event::MCInfo");
      theMCInfo.fillEventUserData();
   }
}

void cms1::BaseAnalyzer::finishProcessing()
{
   if (makeNtuples) theRootFile->Write();
   TimingReport::current()->dump(std::cout);
}

