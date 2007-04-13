//
// Package:         CMS1/Tracks
// Class:           Tracks
// 
// Description:     analysis functions for muons
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Mon Jan 29 16:40:39 UTC 2007
//
// $Author: dmytro $
// $Date: 2007/03/16 07:24:25 $
// $Revision: 1.2 $
//

#include "CMS1/Tracks/interface/Tracks.h"
#include <Math/GenVector/VectorUtil.h>

std::vector<const reco::Track*> cms1::Tracks::getTracks(const TrackType trackType,
							const Cuts& userCuts,
							Cuts::IsolationType isolated)
{
   // this is the output list
   std::vector<const reco::Track*> output_list;
   
   switch (trackType) {
    case AllTracks:
	{
	   if (! data_ ) {
	      std::cout << "ERROR: muon track box doesn't know where to find EvenData." << std::endl;
	      return output_list;
	   }
	   const std::vector<reco::Track>* collection = data_->container_reco_Track.getCollection(edm::InputTag("ctfWithMaterialTracks",""));
	   if ( ! collection ) {
	     std::cout << "ERROR: track collection is not found in the event. Return nothing." << std::endl;
	     return output_list;
	   }

	   // set the default cuts for this type
	   Cuts cuts;
	   cuts.isolated = isolated;
	   cuts.setEventData( data_ );
	   cuts.AND(userCuts);
	   
	   for ( std::vector<reco::Track>::const_iterator track = collection->begin();
		 track != collection->end();
		 ++track ) 
	     {
		if ( ! cuts.testTrack(*track) ) continue;
		output_list.push_back(&*track);
	     }
	}
      break;
      // You get here if you have requested a "muonType" that is not implemented
    default:
      std::cout << "Tracks::getTracks - Unkown or not implemented type" << std::endl;
   }
   return output_list;
}


