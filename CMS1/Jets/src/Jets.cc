//
// Package:         CMS1/Jets
// Class:           Jets
// 
// Description:     analysis functions for jets
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Wed Feb 21 00:50:30 UTC 2007
//
// $Author: dmytro $
// $Date: 2007/08/30 13:32:26 $
// $Revision: 1.17 $
//

#include "CMS1/Jets/interface/Jets.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "CMS1/CommonTools/interface/UserDataTools.h"

std::vector<const reco::Candidate*> cms1::Jets::getJets( const std::string jetType,
							 const Cuts& userCuts )
{
   // this is the output list
   std::vector<const reco::Candidate*> output_list;
   if (! data_ ) {
      std::cout << "ERROR: jet black box doesn't know where to find EvenData." << std::endl;
      assert(0);
   }
   
   ///////////////////////////////////////////////////////////////////
   
   if ( jetType.compare("AllMidPointCone5CaloJets")==0 )
     {
	const std::vector<reco::CaloJet>* collection = data_->getData<std::vector<reco::CaloJet> >("midPointCone5CaloJets");
	if ( ! collection ) {
	   std::cout << "ERROR: jet collection is not found in the event. Return nothing." << std::endl;
	   return output_list;
	}
       
	Cuts cuts;
	cuts.AND(userCuts);
       
	for ( std::vector<reco::CaloJet>::const_iterator jet = collection->begin(); jet != collection->end(); ++jet ) 
	  if ( cuts.testCandidate(*jet) ) output_list.push_back(&*jet);
	return output_list;
     }
   
   ///////////////////////////////////////////////////////////////////
   
   if ( jetType.compare("LooseMidPointCone5CaloJets")==0 )
     {
	const std::vector<const reco::Candidate*> collection = getJets( "AllMidPointCone5CaloJets" );
	Cuts cuts;
	cuts.pt_min = 15;
	cuts.eta_min = -3.0;
	cuts.eta_max = +3.0;
	cuts.AND(userCuts);
       
	for ( std::vector<const reco::Candidate*>::const_iterator jet = collection.begin(); jet != collection.end(); ++jet ) 
	  if ( cuts.testCandidate(**jet) ) output_list.push_back(*jet);
	return output_list;
     }
   
   ///////////////////////////////////////////////////////////////////

   if ( jetType.compare("AllIterativeCone5CaloJets")==0 )
     {
	const std::vector<reco::CaloJet>* collection = data_->getData<std::vector<reco::CaloJet> >("iterativeCone5CaloJets");
	if ( ! collection ) {
	   std::cout << "ERROR: jet collection is not found in the event. Return nothing." << std::endl;
	   return output_list;
	}
       
	Cuts cuts;
	cuts.AND(userCuts);
       
	for ( std::vector<reco::CaloJet>::const_iterator jet = collection->begin(); jet != collection->end(); ++jet ) 
	  if ( cuts.testCandidate(*jet) ) output_list.push_back(&*jet);
	return output_list;
     }

   ///////////////////////////////////////////////////////////////////
 
   if ( jetType.compare("LooseIterativeCone5CaloJets")==0 )
     {
	const std::vector<const reco::Candidate*> collection = getJets( "AllIterativeCone5CaloJets" );
	Cuts cuts;
	cuts.pt_min = 15;
	cuts.eta_min = -3.0;
	cuts.eta_max = +3.0;
	cuts.AND(userCuts);
       
	for ( std::vector<const reco::Candidate*>::const_iterator jet = collection.begin(); jet != collection.end(); ++jet ) 
	  if ( cuts.testCandidate(**jet) ) output_list.push_back(*jet);
	return output_list;
     }
   
   ///////////////////////////////////////////////////////////////////
 
   if ( jetType.compare("MCJetCorJetIcone5")==0 )
     {
	const std::vector<reco::CaloJet>* collection = data_->getData<std::vector<reco::CaloJet> >("MCJetCorJetIcone5");
	if ( ! collection ) {
	   std::cout << "ERROR: jet collection is not found in the event. Return nothing." << std::endl;
	   return output_list;
	}
	for ( std::vector<reco::CaloJet>::const_iterator jet = collection->begin(); jet != collection->end(); ++jet ) 
	  output_list.push_back(&*jet);
	return output_list;
     }

   ///////////////////////////////////////////////////////////////////
 
   if ( jetType.compare("MCJetCorrectorIcone5")==0 )
     {
	const std::vector<reco::CaloJet>* collection = data_->getData<std::vector<reco::CaloJet> >("MCJetCorrectorIcone5");
	if ( ! collection ) {
	   std::cout << "ERROR: jet collection is not found in the event. Return nothing." << std::endl;
	   return output_list;
	}
	for ( std::vector<reco::CaloJet>::const_iterator jet = collection->begin(); jet != collection->end(); ++jet ) 
	  output_list.push_back(&*jet);
	return output_list;
     }
   
   std::cout << "ERROR: unkown or not implemented jet type: " << jetType << "\nAbort" << std::endl;
   assert(0);
   return output_list;
}

void cms1::Jets::dump(std::ostream& o, std::vector<const reco::Candidate*> ml) {
	for ( std::vector<const reco::Candidate*>::iterator i = ml.begin(), ie = ml.end(); i != ie; ++i ) {
		const reco::Candidate* cp = *i;
		o << "Jet      "; 
		o << "Pt = " << cp->pt(); 
		o << ", Eta = " << cp->eta(); 
		o << ", Phi = " << cp->phi(); 
		o << std::endl; 
	}
}

void cms1::Jets::registerEventUserData()
{
//    evtJets.registerBlock( *data_, "jets_", "reference jets,");
//    data_->intUserData.push_back( new UserData<int>("evt_njets", "number of jets in the reference collection", false) );
//    nJets = data_->intUserData.back();
}

void cms1::Jets::fillEventUserData()
{
//   std::vector<const reco::Candidate*> jets = getJets("AllIterativeCone5CaloJets", Cuts());
//   data_->addBBCollection("refJets", jets);
//   evtJets.fill( getStreamerArguments(data_, jets) );
//   nJets->addData( jets.size() );
}
