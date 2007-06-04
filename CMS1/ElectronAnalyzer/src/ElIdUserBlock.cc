#include "CMS1/ElectronAnalyzer/interface/ElIdUserBlock.h"
#include "CMS1/Electrons/interface/Electrons.h"
#include "CMS1/Base/interface/UserDataBlocks.h"
#include "CMS1/Base/interface/TrackStreamer.h"
#include "CMS1/CommonTools/interface/UserDataTools.h"

cms1::ElIdUserBlock::ElIdUserBlock() { 

   isCandidate_ = true;
   el.setCandidateFlag(true);
}

void cms1::ElIdUserBlock::registerBlock(EventData& event, const std::string& name_prefix, const std::string& alias_prefix) {

   el.registerBlock(event, name_prefix+"elid_", alias_prefix );
}

void cms1::ElIdUserBlock::fill(EventData& event, std::vector<const reco::Candidate*> candidate) {
  el.fill(getStreamerArguments(&event, candidate));
}

