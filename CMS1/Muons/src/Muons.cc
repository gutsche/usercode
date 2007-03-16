//
// Package:         CMS1/Muons
// Class:           Muons
// 
// Description:     analysis functions for muons
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Mon Jan 29 16:40:39 UTC 2007
//
// $Author: latb $
// $Date: 2007/03/01 19:08:02 $
// $Revision: 1.4 $
//

#include "CMS1/Muons/interface/Muons.h"

std::vector<const reco::Muon*> cms1::Muons::getMuons(const MuonType muonType,
						     const Cuts& userCuts,
						     Cuts::IsolationType isolated )
{
   // this is the output list
   std::vector<const reco::Muon*> output_list;
   
   // Only TightGlobalMuons are implemented for now
   switch (muonType) {
    case AllGlobalMuons:
	{
	   if (! data_ || ! data_->globalMuonCollection) {
	      std::cout << "ERROR: global muon collection is not set" << std::endl;
	      return output_list;
	   }
	   	   
	   // set the default cuts for this type
	   Cuts cuts;
	   cuts.isolated = isolated;
	   cuts.setEventData( data_ );
	   cuts.AND(userCuts);
	   
	   for ( std::vector<reco::Muon>::const_iterator muon = data_->globalMuonCollection->begin();
		 muon != data_->globalMuonCollection->end();
		 ++muon ) 
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
