#include "CMS1/Base/interface/ElectronUtil.h"

void cms1::R9_25(const reco::BasicClusterShapeAssociationCollection* barrelClShp,
                 const reco::BasicClusterShapeAssociationCollection* endcapClShp, 
                 const reco::PixelMatchGsfElectron* electron,
                 float& eMax, float& e3x3, float& e5x5, float& spp, float& see) { 

  reco::SuperClusterRef sclRef=electron->superCluster();
  reco::BasicClusterShapeAssociationCollection::const_iterator seedShpItr;
  DetId id = sclRef->seed()->getHitsByDetId()[0];
  if (id.subdetId() == EcalBarrel) {
    seedShpItr = barrelClShp->find(sclRef->seed());
  } else {
    seedShpItr = endcapClShp->find(sclRef->seed());
  }
  
  // Get the ClusterShapeRef corresponding to the BasicCluster
  const reco::ClusterShapeRef& seedShapeRef = seedShpItr->val;
  
  eMax = sclRef->energy();
  e3x3 = seedShapeRef->e3x3();
  e5x5 = seedShapeRef->e5x5();
  spp = sqrt(seedShapeRef->covPhiPhi());
  see = sqrt(seedShapeRef->covEtaEta());
}

bool cms1::identify(const reco::PixelMatchGsfElectron* electron, const reco::BasicClusterShapeAssociationCollection* barrelClShp,
                    const reco::BasicClusterShapeAssociationCollection* endcapClShp, int type) {
  
  float dummy, e3x3, e5x5, i, l; 
  R9_25(barrelClShp, endcapClShp, electron, dummy, e3x3, e5x5, l, i);
  
  if (type == 1) {
    
    const double EoverPInMax[]    = { 999.,  999., 999.,   999.,  999.,  999.,  999.,  999.,     // first row loose  
                                      1.3,   1.2,   1.3,   999.,  999.,  999.,  999.,  999. };   // second row tight
    const double EoverPInMin[]    = { 0.,    0.,    0.,    0.,    0.,    0.,    0.,    0.,   
                                      0.9,   0.9,   0.9,   0.6,   0.9,   0.9,   0.9,   0.7 };
    const double deltaEtaIn[]     = { 999.,  999.,  999.,  999.,  999.,  999.,  999.,  999., 
                                      0.004, 0.006, 0.005, 0.007, 0.007, 0.008, 0.007, 0.008 };
    const double deltaPhiIn[]     = { 999.,  999.,  999.,  999.,  999.,  999.,  999.,  999.,  
                                      0.04,  0.07,  0.04,  0.08,  0.06,  0.07,  0.06,  0.07 };
    const double HoverE[]         = { 999.,  999.,  999.,  999.,  999.,  999.,  999.,  999.,  
                                      0.06,  0.05,  0.06,  0.14,  0.1,   0.1,   0.1,   0.12 };
    const double E9overE25[]      = { 0.,    0.,    0.,    0.,    0.,    0.,    0.,    0.,
                                      0.7,   0.75,  0.8,   0.,    0.85,  0.75,  0.8,   0. };
    const double EoverPOutMax[]   = { 999.,  999.,  999.,  999.,  999.,  999.,  999.,  999.,
                                      2.5,   999.,  999.,  999.,  2.,    999.,  999.,  999. };
    const double EoverPOutMin[]   = { 0.,    0.,    0.,    0.,    0.,    0.,   0.,     0.,  
                                      0.6,   1.8,   1.,    0.75,  0.6,   1.5,   1.,    0.8 };  
    const double deltaPhiOut[]    = { 999.,  999.,  999.,  999., 999.,   999.,  999.,  999.,
                                      0.011, 999.,  999.,  999.,  0.02,  999.,  999.,  999.};
    const double sigmaEtaEtaMax[] = { 999.,  999.,  999.,  999.,  999.,  999.,  999.,  999.,
                                      0.011, 0.011, 0.011, 0.011, 0.022, 0.022, 0.022, 0.3 };
    const double sigmaEtaEtaMin[] = { 0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   00.,
                                      0.005, 0.005, 0.005, 0.005, 0.008, 0.008, 0.008, 00. };
    const double sigmaPhiPhiMax[] = { 999.,  999.,  999.,  999.,  999.,  999.,  999.,  999.,
                                      0.015, 999.,  999.,  999.,  0.02,  999.,  999.,  999. };
    const double sigmaPhiPhiMin[] = { 0.,    0.,    0.,    0.,    0.,    0.,    0.,    0.,  
                                      0.005, 0.,    0.,    0.,    0.,    0.,    0.,    0. };
    
    int icut;
    
    float b = electron->eSuperClusterOverP();
    float c = electron->deltaEtaSuperClusterTrackAtVtx();
    float d = electron->deltaPhiSuperClusterTrackAtVtx();
    float e = electron->hadronicOverEm();
    float f = e3x3/e5x5;
    float g = electron->eSeedClusterOverPout();
    float h = electron->deltaPhiSeedClusterTrackAtCalo();
    
    switch (electron->classification()) {
    case 0: icut=0;  break;
    case 10: icut=1; break;
    case 20: icut=2; break;
    case 30: icut=3; break;
    case 31: icut=3; break;
    case 32: icut=3; break;
    case 33: icut=3; break;
    case 34: icut=3; break;
    case 100: icut=4; break;
    case 110: icut=5; break;
    case 120: icut=6; break;
    case 130: icut=7; break;
    case 131: icut=7; break;
    case 132: icut=7; break;
    case 133: icut=7; break;
    case 134: icut=7; break;
    default:
      return false;
    }
    
    if (b < EoverPInMin[icut+type*8] || b > EoverPInMax[icut+type*8]) 
      return false;
    
    if (c > deltaEtaIn[icut+type*8]) 
      return false;
    
    if (d > deltaPhiIn[icut+type*8]) 
      return false;
    
    if (e > HoverE[icut+type*8]) 
      return false;
    
    if (f < E9overE25[icut+type*8]) 
      return false;
    
    if (g < EoverPOutMin[icut+type*8] || g > EoverPOutMax[icut+type*8]) 
      return false;
    
    if (h > deltaPhiOut[icut+type*8]) 
      return false;
    
    if (sqrt(i) < sigmaEtaEtaMin[icut+type*8] || sqrt(i) > sigmaEtaEtaMax[icut+type*8]) 
      return false;
    
    if (sqrt(l) < sigmaPhiPhiMin[icut+type*8] || sqrt(l) > sigmaPhiPhiMax[icut+type*8]) 
      return false;
    
    return true;
  }
  
  if (type == 0) { 
    int eb;	 
    float cutsee[] = {0.018, 0.015, 0.0125,0.0115, 0.010, 0.010,  0., 0.,
                      0.020, 0.020, 0.020, 0.020 , 0.020, 0.020,  0., 0.};  // first row barrel, second endcap
    float cutdeta[] = {0.010, 0.010, 0.010, 0.007,  0.009, 0.004,  0., 0.,
                       0.010, 0.010, 0.010, 0.007,  0.009, 0.004,  0., 0.};
    float cuteop2[] = {0.6,   0.8,   0.7,   0.5,   0.5,    0.9,    0., 0.,
                       0.6,   0.8,   0.7,   0.5,   0.5,    0.9,    0., 0.};
    
    int cat = classify(electron);
    if(fabs(electron->p4().Eta()) < 1.45) 
      eb = 0;
    else 
      eb = 1;
    
    if(cat < 6) {
      if(pow(i, 0.5) < cutsee[cat+eb*8]) {
        if(electron->deltaEtaSuperClusterTrackAtVtx() < cutdeta[cat+eb*8]) {
          if(electron->eSeedClusterOverPout() > cuteop2[cat+eb*8]) {
            return true;
          }
        }
      }
    }
    return false;
  }
  return false;
}

int cms1::classify(const reco::PixelMatchGsfElectron* electron) {
  
  double eOverP = electron->eSuperClusterOverP();
  double pin  = electron->trackMomentumAtVtx().R(); 
  double pout = electron->trackMomentumOut().R(); 
  double fBrem = (pin-pout)/pin;
  
  int cat=6;
  if(eOverP > 0.9*(1-fBrem)) {
    if(fBrem > 0.06) {
      if(eOverP > 1.5) 
        cat=2;
      else if(eOverP > 1.2) 
        cat=1;
      else if(eOverP > 0.8) 
        cat=0;
      else 
        cat=4;
    }
    else if(eOverP > 1.5) 
      cat=5;
    else 
      cat=3;
  } 
  
  return cat;
}
