#ifndef CMS1Jets_h
#define CMS1Jets_h

//
// Package:         CMS1/Jets
// Class:           Jets
// 
// Description:     analysis functions for jets
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Wed Feb 21 00:15:42 UTC 2007
//
// $Author: gutsche $
// $Date: 2007/02/22 23:10:51 $
// $Revision: 1.1 $
//

#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/EgammaCandidates/interface/SiStripElectron.h"
#include "CMS1/Base/interface/Cuts.h"

#include "DataFormats/Math/interface/LorentzVector.h"
#include "Math/VectorUtil.h"

namespace cms1 {
  class Jets  {
  public:
    struct JetData
    {
      JetData(): globalJetCollection(0)
      {}
      
      const std::vector<reco::CaloJet>*              globalJetCollection;
    };
     
    // These are "types of electrons" that we define.  We can add as amany as we want as
    // people invent new electron requirements
    enum JetType { GlobalJets, JetsWithoutElectrons };
     
    // We need to have pointers from the event to the collections that we might want to use.
    // This is done by setting the data_ private member at the beginning of the analysis of 
    // each event.  
    void setData( JetData& data ) { data_ = data; }  // sets data_
    const JetData& getData() const { return data_; } // gets data_ back
    JetData& getData() { return data_; }            // gaets data_ back
    
    // This is the function that does all the work
    std::vector<const reco::CaloJet*> getJets (const JetType, 
					       const Cuts&,
					       bool isolated = true);
      
    // a trivial function that uses getJets to return number of 
    // electrons of a given type in the event
    int numberOfJets(const JetType type, const Cuts& cuts) {
      return getJets(type,cuts).size();
    }
      
  private:
    JetData data_;
  };
}

#endif
