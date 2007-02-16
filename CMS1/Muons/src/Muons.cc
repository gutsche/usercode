//
// Package:         CMS1/Muons
// Class:           Muons
// 
// Description:     analysis functions for muons
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Mon Jan 29 16:40:39 UTC 2007
//
// $Author: gutsche $
// $Date: 2007/01/29 18:15:28 $
// $Revision: 1.1 $
//

#include "CMS1/Muons/interface/Muons.h"

std::vector<const reco::Muon*> cms1::Muons::getMuons(const MuonType muonType,
						     const Cuts& cuts,
						     const bool isolated )
{
   // this is the output list
   std::vector<const reco::Muon*> output_list;
   
   // Only TightGlobalMuons are implemented for now
   switch (muonType) {
    case TightGlobalMuons:
	{
	   if (! data_.globalMuonCollection) {
	      std::cout << "ERROR: global muon collection is not set" << std::endl;
	      return output_list;
	   }
	   for ( std::vector<reco::Muon>::const_iterator muon = data_.globalMuonCollection->begin();
		 muon != data_.globalMuonCollection->end();
		 ++muon ) 
	     {
		// Here we make simple cuts in a standard way
		if ( cuts.testTrack(*muon->track().get()) ) output_list.push_back(&*muon);
	     }
      
	   // At this point the output_list has been filled with muons passing the simple cuts
	   // In general, we will want to add more cuts.  This could go here.  It can be
	   // done several ways, for example:
	   //     Make another empty vector "final_output_list"
	   //     loop over the vector "output_list" that we just made and add to final_output_list
	   //     those muons that pass more complicated cuts
	   // For now, none of this is implemented    

	}
      break;
      // You get here if you have requested a "muonType" that is not implemented
    default:
      std::cout << "Unkown or not implemented type" << std::endl;
   }
   return output_list;
}
