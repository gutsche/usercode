#ifndef CMS1Tracks_h
#define CMS1Tracks_h

//
// Package:         CMS1/Tracks
// Class:           Tracks
// 
// Description:     analysis functions for muons
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Mon Jan 29 16:40:39 UTC 2007
//
// $Author: gutsche $
// $Date: 2007/02/22 23:10:53 $
// $Revision: 1.3 $
//

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuIsoDeposit.h"
#include "DataFormats/MuonReco/interface/MuonWithMatchInfo.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "CMS1/Base/interface/Cuts.h"

namespace cms1 {
   class Tracks  {
    public:
      struct TrackData
	{
	   TrackData(): trackCollection(0) 
	     {}

	   const std::vector<reco::Track>*             trackCollection;
	};
     
      // track types
      enum TrackType { AllTracks };
     
      // We need to have pointers from the event to the collections that we might want to use.
      // This is done by setting the data_ private member at the beginning of the analysis of 
      // each event.  
      void setData( TrackData& data ) { data_ = data; }  // sets data_
      const TrackData& getData() const { return data_; } // gets data_ back
      TrackData& getData() { return data_; }            // gaets data_ back
     
      // This is the function that does all the work
      std::vector<const reco::Track*> getTracks (const TrackType,
						 const Cuts&,
						 bool isolated = true);
      
      // a trivial function that uses getTracks to return number of 
      // muons of a given type in the event
      int numberOfTracks(const TrackType type, const Cuts& cuts) {
	 return getTracks(type,cuts).size();
      }
      
    private:
      TrackData data_;
  };
}

#endif
