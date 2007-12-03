#ifndef CMS1_ElectronStreamer_h
#define CMS1_ElectronStreamer_h

#include "CMS1/Base/interface/TrackStreamer.h"
#include "DataFormats/EgammaReco/interface/BasicClusterShapeAssociation.h"

#include <vector>

namespace cms1 {
  class ElectronStreamer: public TrackStreamer {
  public:
    ElectronStreamer();
    void fill(const reco::Candidate*, bool reset = true);
    void addCollections(const reco::BasicClusterShapeAssociationCollection*,
                        const reco::BasicClusterShapeAssociationCollection*);
    void addCollections(const std::vector<reco::Track>*);

    void fill(const StreamerArguments&, bool reset = true);
    
  protected:
    int *nSeed, *cms_class, *looseId, *tightId, *robustId, *simpleId, *oldlooseId, *oldtightId;
    float *hOverE, *fBrem, *dEtaIn, *dEtaOut, *dPhiIn, *dPhiOut, *varEsc, *varRawEsc, *vare3x3, *vare5x5;
    float *eOverPIn, *eOverPOut, *eSeed, *sPhiPhi, *sEtaEta, *tkIso;

  private:
    const reco::BasicClusterShapeAssociationCollection* barrelClShp, *endcapClShp;
    const std::vector<reco::Track>* tracks;
  };
}

#endif
