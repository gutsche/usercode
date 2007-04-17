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
	   if (! data_ || ! data_->getData<std::vector<reco::Track> >("ctfWithMaterialTracks") ) {
	      std::cout << "ERROR: track collection is not set" << std::endl;
	      return output_list;
	   }
	   // set the default cuts for this type
	   Cuts cuts;
	   cuts.isolated = isolated;
	   cuts.setEventData( data_ );
	   cuts.AND(userCuts);
	   const std::vector<reco::Track>* collection = data_->getData<std::vector<reco::Track> >("ctfWithMaterialTracks");
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


