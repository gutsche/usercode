//
// Package:         CMS1/NtupleMaker
// Class:           NtupleMaker
//
// $Author: dmytro $
// $Date: 2007/08/30 20:27:39 $
// $Revision: 1.2 $
//

#include <vector>
#include <algorithm> 
#include <cmath>
#include <utility>
#include <sstream>
#include <iomanip>

#include "CMS1/NtupleMaker/interface/NtupleMaker.h"
#include "CMS1/Base/interface/Dump.h"
#include "CMS1/EventHyp/interface/EventHyp.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/EgammaCandidates/interface/Electron.h"
#include "DataFormats/EgammaCandidates/interface/ElectronFwd.h"

#include "DataFormats/JetReco/interface/CaloJet.h"

#include "DataFormats/METReco/interface/CaloMETCollection.h"

#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "Utilities/Timing/interface/TimingReport.h"
#include "Utilities/Timing/interface/TimerStack.h" 

void cms1::NtupleMaker::finishProcessing()
{
  // let the base analyzer finish its work
  BaseAnalyzer::finishProcessing();
}

void
cms1::NtupleMaker::processEvent( const edm::Event& iEvent )
{
   TimerStack timers;
   timers.push("NtupleMaker::processEvent");
  
   timers.push("NtupleMaker::processEvent::getInfoFromBlackBoxes");
   // get leptons
   std::vector<const reco::Candidate*> tightMuons     = theMuons.getMuons( tightMuonType );
   std::vector<const reco::Candidate*> looseMuons     = theMuons.getMuons( looseMuonType );
   theData.addBBCollection("MuonsForMETCorrection", theMuons.getMuons( metMuonType ) );
   
   std::vector<const reco::Candidate*> tightElectrons = theElectrons.getElectrons( tightElectronType );
   std::vector<const reco::Candidate*> looseElectrons = theElectrons.getElectrons( looseElectronType );

   // get vector of Jets
   std::vector<const reco::Candidate*> jets = theJets.getJets( jetType );
   // add AllJets to EventData
   theData.addBBCollection("AllJets",        theJets.getJets( referenceJetType ) );
   theData.addBBCollection("CorrectedJets",  theJets.getJets( correctedJetType ) );
   
   // get MET without cut and corrections
   const reco::Candidate* metObj = theMET.getMET( "DefaultMET" );
   double met = metObj->pt();
   double metphi = metObj->phi();
   
   timers.pop_and_push("NtupleMaker::processEvent::makeHypothesis");
   EventHyp eventHyp_; 
   eventHyp_.setEventData(&theData);
   std::vector<const cms1::DiLeptonCandidate*> dlCandidates = 
     eventHyp_.getEventHyp(tightElectrons, looseElectrons, tightMuons, looseMuons, jets, met, metphi );
   /*
     {
	const std::vector<reco::CaloJet>* jetColl =
	  theData.getData<std::vector<reco::CaloJet> >("midPointCone5CaloJets");

	std::cout << "------------------------------------------------------------------------" << std::endl; 
	std::cout << "Dump of Event, " 
	      << " Ne = " << looseElectrons.size() 
	      << " Nmu = " << looseMuons.size() 
	      << " Nj = " << jets.size()		 
	      << " Nj (def.Jets) = " << ( jetColl == 0 ? -1 : int(jetColl->size()) )
	      << std::endl;	
	theElectrons.dump(std::cout, looseElectrons);
	theMuons.dump(std::cout, looseMuons);
	theJets.dump(std::cout, jets);
	theMET.dump(std::cout, metObj);
	std::cout << "------------------------------------------------------------------------" << std::endl; 
     }
    std::cout << "Number of candidates: " << dlCandidates.size() << std::endl;
   */
   
   timers.pop_and_push("NtupleMaker::processEvent::storeHypothesisInfo");
   nCandidates->addData(dlCandidates.size());
   inclusiveCrossSection->addData(inclusiveCrossSectionValue);
   exclusiveCrossSection->addData(exclusiveCrossSectionValue);
   kfactor->addData(kfactorValue);
   
   for ( std::vector<const cms1::DiLeptonCandidate*>::iterator cand = dlCandidates.begin(); cand != dlCandidates.end(); ++cand )
     diLeptonUserData.fill(theData,**cand);
}

void cms1::NtupleMaker::configure(const edm::ParameterSet& iConfig)
{
   tightMuonType =     iConfig.getUntrackedParameter<string>("tightMuonType");
   looseMuonType =     iConfig.getUntrackedParameter<string>("looseMuonType");
   
   metMuonType =           iConfig.getUntrackedParameter<string>("referenceMuonType");
   tightElectronType =     iConfig.getUntrackedParameter<string>("tightElectronType");
   looseElectronType =     iConfig.getUntrackedParameter<string>("looseElectronType");
   
   jetType =           iConfig.getUntrackedParameter<string>("jetType");
   correctedJetType =  iConfig.getUntrackedParameter<string>("correctedJetType");
   referenceJetType =  iConfig.getUntrackedParameter<string>("referenceJetType");
   
   inclusiveCrossSectionValue = iConfig.getUntrackedParameter<double>("inclusiveCrossSection");
   exclusiveCrossSectionValue = iConfig.getUntrackedParameter<double>("exclusiveCrossSection");
   kfactorValue = iConfig.getUntrackedParameter<double>("kfactor");
      
   diLeptonUserData.registerBlock(theData, "hyp_", "hypothesis");
   theData.intUserData.push_back( new UserData<int>("evt_nCand", "number of dilepton hypotheses in the event", false) );
   nCandidates = theData.intUserData.back();
   theData.floatUserData.push_back( new UserData<float>("evt_xsec_incl", "Best estimate of the process inclusive crossection to all standard model final states", false) );
   inclusiveCrossSection = theData.floatUserData.back();
   theData.floatUserData.push_back( new UserData<float>("evt_xsec_excl", "Best estimate of the process exclusive crossection (inclusive x branching ratio)", false) );
   exclusiveCrossSection = theData.floatUserData.back();
   theData.floatUserData.push_back( new UserData<float>("evt_kfactor", "K-factor used in calculation of luminosity scale factor", false) );
   kfactor = theData.floatUserData.back();
}
