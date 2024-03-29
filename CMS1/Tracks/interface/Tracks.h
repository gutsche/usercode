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
// $Date: 2007/06/14 05:59:59 $
// $Revision: 1.6 $
//

#include "DataFormats/TrackReco/interface/Track.h"
#include "CMS1/Base/interface/Cuts.h"
#include "CMS1/Base/interface/BlackBox.h"
#include "CMS1/Base/interface/UserDataBlocks.h"
#include "CMS1/Base/interface/TrackStreamer.h"
#include "CMS1/Base/interface/JetStreamer.h"

namespace cms1 {
   class Tracks: public BlackBox  {
    public:
      Tracks():BlackBox() {}
      
      // This is the function that does all the work
      std::vector<const reco::Track*> getTracks (const std::string& trackType,
						 const Cuts& = Cuts(),
						 Cuts::IsolationType isolated = Cuts::NotIsolated);
      void registerEventUserData();
      void fillEventUserData();
    private:
      VectorUserBlock<TrackStreamer> tracks;
      UserDataInt*        runNumber;
      UserDataInt*        eventNumber;
      UserDataInt*        nTracks;
  };
}

#endif
