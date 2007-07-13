//
// Package:         CMS1/Muons
// Class:           Muons
// 
// Description:     analysis functions for muons
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Mon Jan 29 16:40:39 UTC 2007
//
// $Author: dmytro $
// $Date: 2007/06/23 05:36:36 $
// $Revision: 1.14 $
//

#include "CMS1/Muons/interface/Muons.h"
#include <iostream>
#include "CMS1/CommonTools/interface/UserDataTools.h"

std::vector<const reco::Candidate*> cms1::Muons::getMuons(const MuonType muonType,
						     const Cuts& userCuts,
						     Cuts::IsolationType isolated )
{
   // this is the output list
   std::vector<const reco::Candidate*> output_list;
   
   // Only TightGlobalMuons are implemented for now
   switch (muonType) {
    case AllGlobalMuons:
	{

	   if (! data_ ) {
	      std::cout << "ERROR: muon black box doesn't know where to find EvenData." << std::endl;
	      return output_list;
	   }
	   const std::vector<reco::Muon>* collection = data_->getData<std::vector<reco::Muon> >("globalMuons");
	   if ( ! collection ) {
	      std::cout << "ERROR: global muon collection is not found in the event. Return nothing." << std::endl;
	      return output_list;
	   }
	   	   
	   // set the default cuts for this type
	   Cuts cuts;
	   cuts.isolated = isolated;
	   cuts.setEventData( data_ );
	   cuts.AND(userCuts);
	   
	   for ( std::vector<reco::Muon>::const_iterator muon = collection->begin();
		 muon != collection->end(); ++muon ) 
	     {
		// Here we make simple cuts in a standard way
		if ( cuts.testCandidate(*muon) ) output_list.push_back(&*muon);
	     }
	}
      break;
    case LooseGlobalMuons:
	{
	   // This list is based ont AllGlobalMuons
	   
	   // set the default cuts for this type and merge them with user Cuts
	   Cuts cuts;
	   cuts.pt_min = 15;
	   cuts.AND(userCuts);
	   cuts.setEventData(data_);
	   
	   return getMuons(AllGlobalMuons, cuts, isolated);
	}
      break;
    case TightGlobalMuons:
	{
	   // This list is based ont AllGlobalMuons
	   
	   // set the default cuts for this type and merge them with user Cuts
	   Cuts cuts;
	   cuts.pt_min = 20;
	   cuts.AND(userCuts);
	   cuts.setEventData(data_);
	   
	   return getMuons(AllGlobalMuons, cuts, isolated);
	}
      break;
      // You get here if you have requested a "muonType" that is not implemented
    default:
      std::cout << "Muons::getMuons: Unkown or not implemented type" << std::endl;
   }
   
   return output_list;
}

void cms1::Muons::dump(std::ostream& o, std::vector<const reco::Candidate*> ml) {
	for ( std::vector<const reco::Candidate*>::iterator i = ml.begin(), ie = ml.end(); i != ie; ++i ) {
		const reco::Candidate* cp = *i;
		o << "Muon     "; 
		o << "Pt = " << cp->pt(); 
		o << ", Eta = " << cp->eta(); 
		o << ", Phi = " << cp->phi(); 
	   const  std::vector<reco::Track>* tracks = data_->getData<std::vector<reco::Track> >("ctfWithMaterialTracks");
	   if ( tracks) {
	      double isoRel = cms1::Cuts::trackRelIsolation(cp->momentum(), cp->vertex(), tracks, 0.3, 0.01, 0.1, 999.9, 0.5, 1.5, 7);
	      o << ", isol = " << isoRel;
	   }
	   o << std::endl; 
	}
}

void cms1::Muons::registerEventUserData()
{
   evtMuons.registerBlock( *data_, "mus_", "cms1_mus_");
   data_->intUserData.push_back( new UserData<int>("nmus", "evt_", "cms1_evt_", false) );
   nMuons = data_->intUserData.back();
}

void cms1::Muons::fillEventUserData()
{
   std::vector<const reco::Candidate*> mus = getMuons(AllGlobalMuons,Cuts());
   data_->addBBCollection("refMuons", mus);
   evtMuons.fill( getStreamerArguments(data_, mus) );
   nMuons->addData( mus.size() );
}


