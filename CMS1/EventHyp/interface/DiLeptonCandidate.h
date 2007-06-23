#ifndef CMS1DiLeptonCandidate_h
#define CMS1DiLeptonCandidate_h
/*
 *  DiLeptonCandidate.cc
 *  CMS1
 *
 *  Created by LATBauerdick on 3/22/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include <vector>
#include <string>
#include "CMS1/Muons/interface/Muons.h"
#include "CMS1/Electrons/interface/Electrons.h"
#include "CMS1/Jets/interface/Jets.h"
#include "CMS1/MET/interface/MET.h"
#include "CMS1/Base/interface/UserDataBlocks.h"
namespace cms1 {
   class DiLeptonCandidate {
    public:
      const reco::Candidate* lTight;
      const reco::Candidate* lLoose;
      std::vector<const reco::Candidate*> jets; // sorted by energy
      
      // MET
      double MET_muon_corr;
      double METphi_muon_corr;
      // best MET (all corrections are applied)
      double MET;    
      double METphi; 
      
      //		should probably add commonPrimaryVertex
      enum DiLeptonType { MuMu, MuEl, ElMu, ElEl };
      DiLeptonType candidateType;

      int nJets() const { return jets.size(); };

      std::string candidateTypeString() const;
      std::string tightLeptonTypeString() const;
      std::string looseLeptonTypeString() const;

      static DiLeptonCandidate* returnDiLeptonCandidate( EventData* event,
							 const reco::Candidate* lt, 
							 const reco::Candidate* ll,  
							 std::vector<const reco::Candidate*> jets, 
							 double met, double metPhi, DiLeptonType t
							 );
      static void resetStore();		
    protected:
      static int iStore;
      static std::vector<DiLeptonCandidate> dlStore;
      static DiLeptonCandidate* nextStore();
   };
}

#endif
