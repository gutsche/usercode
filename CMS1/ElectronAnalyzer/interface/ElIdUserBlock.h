#ifndef CMS1_ElIdUserBlock_h
#define CMS1_ElIdUserBlock_h

#include <vector>
#include <string>
#include "CMS1/Base/interface/EventData.h"
#include "CMS1/Electrons/interface/Electrons.h"
#include "CMS1/Base/interface/TrackStreamer.h"
#include "CMS1/Base/interface/GenParticleStreamer.h"

namespace cms1 {
  struct ElIdUserBlock: public BaseUserBlock {
    ElIdUserBlock();
    
    void registerBlock(EventData& event, const std::string& name_prefix, const std::string& title_prefix);
    void fill(EventData&, std::vector<const reco::Candidate*>, std::vector<const HepMC::GenParticle*>);
    
    void R9_25(EventData&, const reco::PixelMatchGsfElectron*, float&, float&, float&, float&, float&);
    
    private:
    VectorUserBlock<TrackStreamer> el;
    VectorUserBlock<GenParticleStreamer> gp;
    
    UserDataInt1D *nSeed, *cms_class;
    UserDataFloat1D *hOverE, *fBrem, *dEtaIn, *dEtaOut, *dPhiIn, *dPhiOut;
    UserDataFloat1D *vareMax, *vare3x3, *vare5x5;
    UserDataFloat1D *eOverPIn, *eOverPOut, *eSeed, *sPhiPhi, *sEtaEta, *tkIso;
  };
}

#endif
