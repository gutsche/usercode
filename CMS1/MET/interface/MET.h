#ifndef CMS1MET_h
#define CMS1MET_h

//
// Package:         CMS1/MET
// Class:           MET
// 
// Description:     analysis functions for MET
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Wed Feb 21 00:15:42 UTC 2007
//
// $Author: dmytro $
// $Date: 2007/02/16 11:53:32 $
// $Revision: 1.2 $
//

#include "DataFormats/METReco/interface/CaloMETCollection.h"

#include "CMS1/Base/interface/Cuts.h"

namespace cms1 {
  class MET  {
  public:
    struct METData
    {
      METData(): globalMETCollection(0)
      {}
      
      const std::vector<reco::CaloMET>*              globalMETCollection;
    };
     
    // These are "types of electrons" that we define.  We can add as amany as we want as
    // people invent new electron requirements
    enum METType { GlobalMET };
     
    // We need to have pointers from the event to the collections that we might want to use.
    // This is done by setting the data_ private member at the beginning of the analysis of 
    // each event.  
    void setData( METData& data ) { data_ = data; }  // sets data_
    const METData& getData() const { return data_; } // gets data_ back
    METData& getData() { return data_; }            // gaets data_ back
    
    // This is the function that does all the work
    std::vector<const reco::CaloMET*> getMET (const METType, 
					      const Cuts&,
					       bool isolated = true);
      
    // a trivial function that uses getMET to return number of 
    // electrons of a given type in the event
    int numberOfMET(const METType type, const Cuts& cuts) {
      return getMET(type,cuts).size();
    }
    
  private:
    METData data_;
  };
}

#endif
