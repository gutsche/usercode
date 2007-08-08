#ifndef Base_ElectronUtil
#define Base_ElectronUtil

#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"
#include "DataFormats/EgammaReco/interface/BasicClusterShapeAssociation.h"
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"

namespace cms1 {
  void R9_25(const reco::BasicClusterShapeAssociationCollection*, 
             const reco::BasicClusterShapeAssociationCollection*, 
             const reco::PixelMatchGsfElectron*,
             float&, float&, float&, float&, float&);

  bool identify(const reco::PixelMatchGsfElectron*, const reco::BasicClusterShapeAssociationCollection* barrelClShp,
                const reco::BasicClusterShapeAssociationCollection* endcapClShp, int);
  int classify(const reco::PixelMatchGsfElectron*);
}

#endif
