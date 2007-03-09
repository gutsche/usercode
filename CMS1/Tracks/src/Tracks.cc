//
// Package:         CMS1/Tracks
// Class:           Tracks
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

#include "CMS1/Tracks/interface/Tracks.h"

std::vector<const reco::Track*> cms1::Tracks::getTracks(const TrackType trackType,
							const Cuts& cuts,
							const bool isolated )
{
   // this is the output list
   std::vector<const reco::Track*> output_list;
   
   switch (trackType) {
    case AllTracks:
	{
	   if (! data_.trackCollection) {
	      std::cout << "ERROR: track collection is not set" << std::endl;
	      return output_list;
	   }
	   for ( std::vector<reco::Track>::const_iterator track = data_.trackCollection->begin();
		 track != data_.trackCollection->end();
		 ++track ) 
	     {
		// Here we make simple cuts in a standard way
		if ( cuts.testTrack(*track) ) output_list.push_back(&*track);
	     }
	}
      break;
      // You get here if you have requested a "muonType" that is not implemented
    default:
      std::cout << "Tracks::getTracks - Unkown or not implemented type" << std::endl;
   }
   return output_list;
}
