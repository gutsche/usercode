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
      double MET;
      //		should probably add commonPrimaryVertex
      enum DiLeptonType { MuMu, MuEl, ElMu, ElEl };
      DiLeptonType candidateType;

      int nJets() const { return jets.size(); };

      std::string candidateTypeString() const;
      std::string tightLeptonTypeString() const;
      std::string looseLeptonTypeString() const;

      static DiLeptonCandidate* returnDiLeptonCandidate(
							const reco::Candidate* lt, 
							const reco::Candidate* ll,  
							std::vector<const reco::Candidate*> jets, 
							double met, DiLeptonType t
							);
      static void resetStore();		
    protected:
      static int iStore;
      static std::vector<DiLeptonCandidate> dlStore;
      static DiLeptonCandidate* nextStore();
   };
   
   struct DiLeptonUserBlock: public BaseUserBlock
     {
	UserDataVP4*         vTightP4;
	UserDataVP4*         vLooseP4;
	// four leading jets should be enough for most 
	// interactive studies otherwise look at the 
	// EventUserData that has all jets.
	UserDataVP4*         vJet1P4;
	UserDataVP4*         vJet2P4;
	UserDataVP4*         vJet3P4;
	UserDataVP4*         vJet4P4;
	
	UserDataVFloat*      vMET;
	UserDataVFloat*      vMass;
	
	UserDataVInt*        vType;
	UserDataVInt*        vNJets;
	
	DiLeptonUserBlock(): vTightP4(0), vLooseP4(0), vJet1P4(0), vJet2P4(0), vJet3P4(0), vJet4P4(0),
	vMET(0),vMass(0),vType(0),vNJets(0)
	  {}

	void registerBlock(EventData& event, const std::string& name_prefix, const std::string& alias_prefix="")
	  {
	     addEntry(event.vp4UserData,    vTightP4,         "_tight_p4",   name_prefix, alias_prefix);
	     addEntry(event.vp4UserData,    vLooseP4,         "_loose_p4",   name_prefix, alias_prefix);
	     addEntry(event.vp4UserData,    vJet1P4,          "_jet1_p4" ,   name_prefix, alias_prefix);
	     addEntry(event.vp4UserData,    vJet2P4,          "_jet2_p4" ,   name_prefix, alias_prefix);
	     addEntry(event.vp4UserData,    vJet3P4,          "_jet3_p4" ,   name_prefix, alias_prefix);
	     addEntry(event.vp4UserData,    vJet4P4,          "_jet4_p4" ,   name_prefix, alias_prefix);
	     addEntry(event.vfloatUserData, vMET,             "_met",        name_prefix, alias_prefix);
	     addEntry(event.vfloatUserData, vMass,            "_mass",       name_prefix, alias_prefix);
	     addEntry(event.vintUserData,   vType,            "_type",       name_prefix, alias_prefix);
	     addEntry(event.vintUserData,   vNJets,           "_njets",      name_prefix, alias_prefix);
	  }
	void fill(const DiLeptonCandidate& candidate){
	   if ( ! usable() ) return;
	   vTightP4->get()->push_back( candidate.lTight->p4() );
	   vLooseP4->get()->push_back( candidate.lLoose->p4() );
	   vNJets->get()->push_back( candidate.jets.size() );
	   vJet1P4->get()->push_back( candidate.jets.size()> 0 ? candidate.jets[0]->p4() : LorentzVector(0,0,0,0) );
	   vJet2P4->get()->push_back( candidate.jets.size()> 1 ? candidate.jets[1]->p4() : LorentzVector(0,0,0,0) );
	   vJet3P4->get()->push_back( candidate.jets.size()> 2 ? candidate.jets[2]->p4() : LorentzVector(0,0,0,0) );
	   vJet4P4->get()->push_back( candidate.jets.size()> 3 ? candidate.jets[3]->p4() : LorentzVector(0,0,0,0) );
	   vType->get()->push_back( candidate.candidateType );
	   vMET->get()->push_back( candidate.MET );
	   vMass->get()->push_back( (candidate.lTight->p4()+candidate.lLoose->p4()).M() );
	}
	bool usable(){
	   if ( vTightP4==0||vLooseP4==0||vJet1P4==0||vJet2P4==0||vJet3P4==0||vJet4P4==0||
		vMET==0||vMass==0||vType==0||vNJets==0)
	     {
		std::cout << "ERROR: attempt to fill data for non-registered JetUserBlock" << std::endl;
		return false;
	     }
	   return true;
	}
     };

}

#endif
