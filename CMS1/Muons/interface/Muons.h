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
// $Author$
// $Date:$
// $Revision:$
//

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuIsoDeposit.h"
#include "DataFormats/MuonReco/interface/MuonWithMatchInfo.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "CMS1/Base/interface/Cuts.h"

namespace cms1 {
   class Muons  {
    public:
      struct MuonData
	{
	   MuonData(): globalMuonCollection(0), muonIdCollection(0),
	   muonIsolationCollection(0), trackCollection(0) 
	     {}

	   const std::vector<reco::Muon>*              globalMuonCollection;
	   const std::vector<reco::MuonWithMatchInfo>* muonIdCollection;
	   const std::vector<reco::MuIsoDeposit>*      muonIsolationCollection;
	   const std::vector<reco::Track>*             trackCollection;
	};
     
      // These are "types of muons" that we define.  We can add as amany as we want as
      // people invent new muon requirements
      enum MuonType { AllTracks, AllGlobalMuons, TightGlobalMuons, AllMuonWithDetectorInfos };
     
      // We need to have pointers from the event to the collections that we might want to use.
      // This is done by setting the data_ private member at the beginning of the analysis of 
      // each event.  
      void setData( MuonData& data ) { data_ = data; }  // sets data_
      const MuonData& getData() const { return data_; } // gets data_ back
      MuonData& getData() { return data_; }            // gaets data_ back
     
      // This is the function that does all the work
      std::vector<const reco::Muon*> getMuons (const MuonType, 
					       const Cuts&,
					       bool isolated = true);
      
      // a trivial function that uses getMuons to return number of 
      // muons of a given type in the event
      int numberOfMuons(const MuonType type, const Cuts& cuts) {
	 return getMuons(type,cuts).size();
      }
      
    private:
      MuonData data_;
  };
}

#endif
