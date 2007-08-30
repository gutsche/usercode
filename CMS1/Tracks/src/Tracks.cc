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
// $Date: 2007/08/07 11:13:37 $
// $Revision: 1.11 $
//

#include "CMS1/Tracks/interface/Tracks.h"
#include <Math/GenVector/VectorUtil.h>
#include "CMS1/CommonTools/interface/UserDataTools.h"

std::vector<const reco::Track*> cms1::Tracks::getTracks(const std::string& trackType,
							const Cuts& userCuts,
							Cuts::IsolationType isolationType)
{
   // this is the output list
   std::vector<const reco::Track*> output_list;
   if (! data_ || ! data_->getData<std::vector<reco::Track> >("ctfWithMaterialTracks") ) {
      std::cout << "ERROR: track collection is not set" << std::endl;
      assert(0);
   }
   
   if ( trackType.compare("AllTracks")==0 )
     {
	// set the default cuts for this type
	Cuts cuts;
	cuts.isolated = isolationType;
	cuts.setEventData( data_ );
	cuts.AND(userCuts);
	
	const std::vector<reco::Track>* collection = data_->getData<std::vector<reco::Track> >("ctfWithMaterialTracks");
	if ( ! collection ) {
	   std::cout << "ERROR: track collection is not found in the event. Return nothing." << std::endl;
	   return output_list;
	}

	for ( std::vector<reco::Track>::const_iterator track = collection->begin(); track != collection->end(); ++track ) 
	  if ( cuts.testTrack(*track) ) output_list.push_back(&*track);
	return output_list;
     }
   
   /////////////////////////////////////////////////////
   
   if ( trackType.compare("LooseTracks")==0 )
     {
	Cuts cuts;
	cuts.pt_min = 2;
	cuts.AND(userCuts);
	cuts.setEventData(data_);
	return getTracks("AllTracks", cuts, isolationType);
     }
   
   /////////////////////////////////////////////////////
   
   std::cout << "ERROR: unkown or not implemented track type: " << trackType << "\nAbort" << std::endl;
   assert(0);
   return output_list;
}
void cms1::Tracks::registerEventUserData()
{
   tracks.registerBlock( *data_, "trks_", "reference tracks,");
   data_->intUserData.push_back( new UserData<int>("evt_ntrks", "number of tracks in the reference collection", false) );
   nTracks = data_->intUserData.back();
   data_->intUserData.push_back( new UserData<int>("evt_run", "run number", false) );
   runNumber = data_->intUserData.back();
   data_->intUserData.push_back( new UserData<int>("evt_event", "event number", false) );
   eventNumber = data_->intUserData.back();
}

void cms1::Tracks::fillEventUserData()
{
   // std::vector<const reco::Track*> trks = getTracks("LooseTracks");
   std::vector<const reco::Track*> trks = getTracks("AllTracks");
   data_->refTracks = trks;
   tracks.fill( getStreamerArguments(data_, trks) );
   runNumber->addData( data_->iEvent->id().run() );
   eventNumber->addData( data_->iEvent->id().event() );
   nTracks->addData( trks.size() );
}


