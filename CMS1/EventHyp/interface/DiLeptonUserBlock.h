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
	void fill(EventData& event, 
		  const DiLeptonCandidate& candidate, 
		  std::vector<const reco::Candidate*>& allJets);
	bool usable();
      private:
	ScalarUserBlock<TrackStreamer>   lTight;
	ScalarUserBlock<TrackStreamer>   lLoose;
	ScalarUserBlock<TrackStreamer>   elec1;
	ScalarUserBlock<TrackStreamer>   elec2;
	ScalarUserBlock<TrackStreamer>   muon1;
	ScalarUserBlock<TrackStreamer>   muon2;
	
	VectorUserBlock<JetStreamer>     jets;
	VectorUserBlock<JetStreamer>     otherJets;
	
	UserDataFloat*      met;
	UserDataFloat*      mass;
	
	UserDataInt*        type;
	UserDataInt*        nJets;
	UserDataInt*        nOtherJets;
     };
}

#endif
