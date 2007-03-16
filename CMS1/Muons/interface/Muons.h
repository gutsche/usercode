#ifndef CMS1Muons_h
#define CMS1Muons_h

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
// $Date: 2007/02/22 23:10:53 $
// $Revision: 1.3 $
//

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuIsoDeposit.h"
#include "DataFormats/MuonReco/interface/MuonWithMatchInfo.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "CMS1/Base/interface/Cuts.h"
#include "CMS1/Base/interface/BlackBox.h"

namespace cms1 {
   class Muons: public BlackBox {
    public:
      Muons(): BlackBox(){}
      
      // These are "types of muons" that we define.  We can add as amany as we want as
      // people invent new muon requirements
      enum MuonType { AllTracks, AllGlobalMuons, TightGlobalMuons, LooseGlobalMuons, AllMuonWithDetectorInfos };
     
      // This is the function that does all the work
      std::vector<const reco::Muon*> getMuons (const MuonType, 
					       const Cuts&,
					       Cuts::IsolationType isolated = Cuts::NotIsolated);
      // a trivial function that uses getMuons to return number of 
      //  muons of a given type in the event
      int numberOfMuons(const MuonType type, const Cuts& cuts) {
	 return getMuons(type,cuts).size();
      }
  };
}

#endif
