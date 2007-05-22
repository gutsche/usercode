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
// $Author: dmytro $
// $Date: 2007/05/11 04:21:23 $
// $Revision: 1.3 $
//

#include "DataFormats/TrackReco/interface/Track.h"
#include "CMS1/Base/interface/Cuts.h"
#include "CMS1/Base/interface/BlackBox.h"
#include "CMS1/Base/interface/UserDataBlocks.h"

namespace cms1 {
   class Tracks: public BlackBox  {
    public:
      Tracks():BlackBox() {}
      
      // track types
      enum TrackType { AllTracks };
      
      // This is the function that does all the work
      std::vector<const reco::Track*> getTracks (const TrackType,
						 const Cuts&,
						 Cuts::IsolationType isolated = Cuts::NotIsolated);
      void registerEventUserData();
      void fillEventUserData();
    private:
      // TrackUserBlock tracks;
  };
}

#endif
