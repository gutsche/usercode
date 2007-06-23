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
// $Date: 2007/06/14 05:59:58 $
// $Revision: 1.8 $
//
#include <iostream>

#include "DataFormats/METReco/interface/CaloMETCollection.h"
#include "DataFormats/MuonReco/interface/Muon.h"


#include "CMS1/Base/interface/Cuts.h"
#include "CMS1/Base/interface/BlackBox.h"
#include "CMS1/Base/interface/UserDataBlocks.h"

namespace cms1 {
   class MET: public BlackBox  {
  public:
      MET(): BlackBox()	{}
      
      enum METType { DefaultMET };
     
      // This is the function that does all the work
      const reco::CaloMET* getMET (const METType );
      
      // here should be all corrections
      static void correctMETmuons(EventData* event, double& et, double& phi);
      static void correctedJetMET(EventData* event, const std::vector<const reco::Candidate*>* , double& , double&);
      void dump(std::ostream&, const reco::Candidate*);
      void registerEventUserData();
      void fillEventUserData();
    private:
      UserDataFloat*        evtMet;
      UserDataFloat*        evtMetPhi;
      UserDataFloat*        evtSumEt;
      UserDataFloat*        evtMetSig;
      UserDataFloat*        evtMet_corr;
      UserDataFloat*        evtMetPhi_corr;
  };
}

#endif
