#ifndef CMS1_DiLeptonUserBlock_h
#define CMS1_DiLeptonUserBlock_h

#include <vector>
#include <string>
#include "CMS1/Muons/interface/Muons.h"
#include "CMS1/Electrons/interface/Electrons.h"
#include "CMS1/Jets/interface/Jets.h"
#include "CMS1/MET/interface/MET.h"
#include "CMS1/Base/interface/UserDataBlocks.h"
#include "CMS1/Base/interface/TrackStreamer.h"
#include "CMS1/Base/interface/JetStreamer.h"
#include "CMS1/CommonTools/interface/UserDataTools.h"
#include "CMS1/EventHyp/interface/DiLeptonCandidate.h"
namespace cms1 {
   struct DiLeptonUserBlock: public BaseUserBlock
     {
	DiLeptonUserBlock();

	void registerBlock(EventData& event, const std::string& name_prefix, const std::string& alias_prefix="");
	void fill(EventData& event, const DiLeptonCandidate& candidate);
	bool usable();
      private:
	ScalarUserBlock<TrackStreamer>   lTight;
	ScalarUserBlock<TrackStreamer>   lLoose;
	
	VectorUserBlock<JetStreamer>     jets;
	VectorUserBlock<JetStreamer>     otherJets;
	
	UserDataP4*         p4Hyp;
	UserDataFloat*      met;     // default met
	UserDataFloat*      metPhi;  // default met
	UserDataFloat*      metCone;     // met corrected with energy in a cone around track projection on calo
	UserDataFloat*      metPhiCone;  // met corrected with energy in a cone around track projection on calo
	UserDataFloat*      metNoCalo;     // met with partial muon correction (no calo MIP)
	UserDataFloat*      metPhiNoCalo;  // default met
	UserDataFloat*      metJes5;     // jet energy scale correction applied for jets with p_t>5 GeV
	UserDataFloat*      metPhiJes5;  // jet energy scale correction applied for jets with p_t>5 GeV
	UserDataFloat*      metJes10;    // jet energy scale correction applied for jets with p_t>15 GeV
	UserDataFloat*      metPhiJes10; // jet energy scale correction applied for jets with p_t>15 GeV
	UserDataFloat*      metJes15;    // jet energy scale correction applied for jets with p_t>15 GeV
	UserDataFloat*      metPhiJes15; // jet energy scale correction applied for jets with p_t>15 GeV
	UserDataFloat*      metJes30;    // jet energy scale correction applied for jets with p_t>30 GeV
	UserDataFloat*      metPhiJes30;  // jet energy scale correction applied for jets with p_t>30 GeV
	UserDataFloat*      metJes50;    // jet energy scale correction applied for jets with p_t>30 GeV
	UserDataFloat*      metPhiJes50;  // jet energy scale correction applied for jets with p_t>30 GeV
	UserDataFloat*      ltIso;
	UserDataFloat*      llIso;
	
        // UserDataFloat*      metMuonCorr;
        // UserDataFloat*      metPhiMuonCorr;
	UserDataFloat*      metDPhiJet10;
	UserDataFloat*      metDPhiJet15;
	UserDataFloat*      metDPhiJet20;
	UserDataFloat*      metDPhiTrk10;
	UserDataFloat*      metDPhiTrk25;
	UserDataFloat*      metDPhiTrk50;
	
	UserDataInt*        type;
	UserDataInt*        nJets;
	UserDataInt*        nOtherJets;
	UserDataInt*        ltIndex;
	UserDataInt*        llIndex;
	UserDataInt*        ltId;
	UserDataInt*        llId;
	
     };
}

#endif
