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
      // should probably add commonPrimaryVertex
      enum DiLeptonType { MuMu, MuEl, ElMu, ElEl };
      
      DiLeptonType candidateType;
      const reco::Candidate* lTight;
      const reco::Candidate* lLoose;
      double lTightIso;
      double lLooseIso;
      std::vector<const reco::Candidate*> jets; // sorted by energy
      // MET (uncorrected)
      double MET_uncorr;
      double METphi_uncorr;
      // best MET (only muon correction)
      double MET;    
      double METphi; 


      int nJets() const { return jets.size(); };

      std::string candidateTypeString() const;
      std::string tightLeptonTypeString() const;
      std::string looseLeptonTypeString() const;

      DiLeptonCandidate( EventData* event,
			 const reco::Candidate* lt, 
			 const reco::Candidate* ll,  
			 std::vector<const reco::Candidate*> jets, 
			 double met, double metPhi, DiLeptonType t
			 );
   };
}

#endif
