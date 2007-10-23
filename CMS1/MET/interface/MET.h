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
// $Date: 2007/08/30 13:33:37 $
// $Revision: 1.13 $
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
      // all muon corrections assume p4 correction is applied
      enum CorrectionType { NoCaloCorrection, CrossedEnergyCorrection, S9EnergyCorrection, ExpectedMipEnergyCorrection };
	   
      MET(): BlackBox()	{}

      const reco::CaloMET* getMET ( const std::string& metType );
      
      // here should be all corrections
      static void correctMETmuons(EventData& event, const std::vector<const reco::Candidate*>& muons,
				  double& met, double& metPhi, 
				  CorrectionType type = CrossedEnergyCorrection);
      static void correctedJetMET(EventData& event, const std::vector<const reco::Candidate*>& jets,
				  double& met, double& metPhi,
				  const double min_pt=30);
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
