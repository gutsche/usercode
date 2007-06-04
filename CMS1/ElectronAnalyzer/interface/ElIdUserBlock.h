#ifndef CMS1_ElIdUserBlock_h
#define CMS1_ElIdUserBlock_h

#include <vector>
#include <string>
#include "CMS1/Base/interface/EventData.h"
#include "CMS1/Electrons/interface/Electrons.h"
#include "CMS1/Base/interface/TrackStreamer.h"


namespace cms1 {
  struct ElIdUserBlock: public BaseUserBlock {
    ElIdUserBlock();
    
    void registerBlock(EventData& event, const std::string& name_prefix, const std::string& alias_prefix="");
    void fill(EventData& event, std::vector<const reco::Candidate*> candidate);

    private:
    VectorUserBlock<TrackStreamer> el;
   };
}

#endif
