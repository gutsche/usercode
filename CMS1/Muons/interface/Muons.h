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
// $Author: dmytro $
// $Date: 2007/08/07 11:13:36 $
// $Revision: 1.11 $
//
#include <iostream>

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuIsoDeposit.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "CMS1/Base/interface/Cuts.h"
#include "CMS1/Base/interface/BlackBox.h"
#include "CMS1/Base/interface/UserDataBlocks.h"
#include "CMS1/Base/interface/MuonStreamer.h"

namespace cms1 {
   class Muons: public BlackBox {
    public:
      Muons(): BlackBox(){}
      
      std::vector<const reco::Candidate*> getMuons (const std::string& muonType,
						    const Cuts& = Cuts(),
						    Cuts::IsolationType isolated = Cuts::NotIsolated);
      void dump(std::ostream& o, std::vector<const reco::Candidate*>);
      void registerEventUserData();
      void fillEventUserData();
    private:
      VectorUserBlock<MuonStreamer> evtMuons;
      UserDataInt*        nMuons;
      UserDataInt1D*      map_LooseTrackerMuons;
      UserDataInt1D*      map_TightTrackerMuons;
  };
}

#endif
