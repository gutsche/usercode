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
  
  // type = 0 robust (looser and safe), 1 loose, 2 tight, 3 simple (old robust), 4 old loose (6 categories), 5 old tight (6 categories) 
  
  float dummy, e3x3, e5x5, i, l; 
  R9_25(barrelClShp, endcapClShp, electron, dummy, e3x3, e5x5, l, i);
  int eb;
  
  double eOverP = electron->eSuperClusterOverP();
  double eSeedOverPOut = electron->eSeedClusterOverPout();
  double eSeed = electron->superCluster()->seed()->energy();
  double pin  = electron->trackMomentumAtVtx().R();   
  double eSeedOverPin = eSeed/pin; 
  double pout = electron->trackMomentumOut().R(); 
  double fBrem = (pin-pout)/pin;
  double hOverE = electron->hadronicOverEm();
  double sigmaee = i;
  double sigmaeecor = i;
  double deltaPhiIn = electron->deltaPhiSuperClusterTrackAtVtx();
  double deltaEtaIn = electron->deltaEtaSuperClusterTrackAtVtx();
  double eta = fabs(electron->p4().Eta());

  if (eta < 1.479) 
    eb = 0;
  else {
    eb = 1; 
    sigmaeecor = sigmaee - 0.02*(fabs(eta) - 2.3);   //correct sigmaetaeta dependence on eta in endcap
  }
  
  if (type == 2) { 
    
    float cuthoe[]  = {0.05,    0.042,  0.045,  0.,  
                       0.055,   0.037,  0.050,  0.};
    float cutsee[]  = {0.0125,  0.011,  0.01,   0.,
                       0.0265,  0.0252, 0.026,  0.};
    float cutdphi[] = {0.032,   0.016,  0.0525, 0.09, 
                       0.025,   0.035,  0.065,  0.092};
    float cutdeta[] = {0.0055,  0.0030, 0.0065, 0.,
                       0.0060,  0.0055, 0.0075, 0.}; 
    float cuteop2[] = {0.24,    0.94,   0.11,   0.,   
                       0.32,    0.83,   0.,     0.}; 
    
    int cat = classify(electron);
        
    // TIGHT Selection
    if ((eOverP < 0.8) && (fBrem < 0.2)) 
      return false;
    
    if (eOverP < 0.9*(1-fBrem))
      return false;
    
    if (hOverE > cuthoe[cat+eb*4]) 
      return false;    
    
    if (sigmaeecor > cutsee[cat+4*eb]) 
      return false;    
    
    if (eOverP < 1.5) {
      if (fabs(deltaPhiIn) > cutdphi[cat+4*eb]) 
        return false;    
    } else {
      if (fabs(deltaPhiIn) > cutdphi[3+4*eb])
        return false;
    }
    
    if (fabs(deltaEtaIn) > cutdeta[cat+4*eb]) 
      return false;    
    
    if (eSeedOverPin < cuteop2[cat+4*eb]) 
      return false;  
    
    return true; 
  }

  if (type == 1) { 
    
    float cutsee[]  = {0.014,  0.012,  0.0115, 0.,
                       0.0275, 0.0265, 0.0265, 0.};  // first row barrel, second endcap
    float cutdeta[] = {0.009,  0.0045, 0.0085, 0.,
                       0.0105, 0.0068, 0.010,  0.};
    float cuteop2[] = {0.11,   0.91,    0.11,  0.,
                       0.0,    0.85,    0.0,   0.};
    float cutdphi[] = {0.05,   0.025,  0.053,  0.09,
                       0.07,   0.03,   0.092,  0.092};
    float cuthoe[]  = {0.115,  0.10,   0.055,  0.,
                       0.145,  0.12,   0.15,   0.};
    
    int cat = classify(electron);
    
    // const reco::ClusterShapeRef& shapeRef = getClusterShape(electron, e);
    
    // LOOSE Selection
    if ((eOverP < 0.8) && (fBrem < 0.2)) 
      return false;
    
    if (hOverE > cuthoe[cat+eb*4]) 
      return false;    
    
    if (sigmaeecor > cutsee[cat+4*eb]) 
      return false;    
    
    if (eOverP < 1.5) {
      if (fabs(deltaPhiIn) > cutdphi[cat+4*eb]) 
	return false;    
    } else {
      if (fabs(deltaPhiIn) > cutdphi[3+4*eb])
	return false;
    }
    
    if (fabs(deltaEtaIn) > cutdeta[cat+4*eb]) 
      return false;    
    
    if (eSeedOverPin < cuteop2[cat+4*eb]) 
      return false;  
    
    return true; 
  }

  if (type == 0) {
    
    double cut[] = {0.115, 0.0140, 0.090, 0.0090,
                    0.150, 0.0275, 0.092, 0.0105};
    
    if (hOverE > cut[0+eb*4]) 
      return false;    
    
    if (sigmaeecor > cut[1+eb*4]) 
      return false;    
    
    if (fabs(deltaPhiIn) > cut[2+eb*4]) 
      return false;    
    
    if (fabs(deltaEtaIn) > cut[3+eb*4]) 
      return false;    
    
    return true;
  }
  
  if (type == 3) {
    double dRIn = sqrt(pow(deltaEtaIn,2)+pow(deltaPhiIn,2));
    if ((eta > 1.479 || sigmaee < 0.012) && sigmaee < 0.03 && 
        fabs(deltaEtaIn) < 0.012 && hOverE < 0.1 && eOverP > 0.9 && dRIn < 0.1)
      return true;
    
    return false;
  }
  
  if (type == 4) {
    
    int cat_old = classify_old(electron);
    
    float cutsee[] =  {0.014,  0.0125, 0.0125, 0.0125, 0.0125, 0.0125, 0.0125, 0.0125, 
                       0.031,  0.028,  0.028,  0.028,  0.028,  0.028,  0.028,  0.028}; // first row barrel, second endcap
    float cutdeta[] = {0.009,  0.0085, 0.0085, 0.0035, 0.0085, 0.0035, 0.0085, 0.0085,
                       0.009,  0.009,  0.009,  0.009,  0.009,  0.009,  0.009,  0.009};
    float cutdphi[] = {0.06,   0.06,   0.10,   0.02,   0.06,   0.06,   0.06,   0.06,
                       0.06,   0.10,   0.10,   0.02,   0.10,   0.10,   0.10,   0.10};
    float cuthoe[] =  {0.125,  0.055,  0.055,  0.10,   0.055,  0.055,  0.055,  0.055,
                       0.125,  0.10,   0.10,   0.10,   0.10,   0.10,   0.10,   0.10};
    float cuteop2[] = {0.55,   0.88,   0.88,   0.88,   0.88,   0.88,   0.88,   0.88,
                       0.85,   0.85,   0.85,   0.85,   0.85,   0.85,   0.85,   0.85};
    
    if(sigmaeecor < cutsee[cat_old+eb*8]) {
      if(fabs(deltaEtaIn) < cutdeta[cat_old+eb*8]) {
        if(fabs(deltaPhiIn) < cutdphi[cat_old+eb*8]) {
          if(eSeedOverPOut > cuteop2[cat_old+eb*8]) {
            if(hOverE < cuthoe[cat_old+eb*8]) {
              return true;
            }
          }
        }
      }
    }
  
    return false;
  }

  if (type == 5) {
    
    int cat_old = classify_old(electron);
    
    float cutsee[] =  {0.014,  0.012,  0.012,  0.012,  0.012,  0.012,  0.012,  0.012, 
                       0.027,  0.026,  0.026,  0.026,  0.026,  0.026,  0.026,  0.026}; // first row barrel, second endcap
    float cutdeta[] = {0.009,  0.0055, 0.0055, 0.004,  0.0055, 0.004,  0.0055, 0.0055,
                       0.008,  0.008,  0.008,  0.008,  0.008,  0.008,  0.008,  0.008};
    float cutdphi[] = {0.04,   0.04,   0.07,   0.02,   0.04,   0.04,   0.04,   0.04,
                       0.055,  0.085,  0.085,  0.02,   0.085,  0.085,  0.085,  0.085};
    float cuthoe[] =  {0.12,   0.055,  0.055,  0.095,  0.055,  0.055,  0.055,  0.055,
                       0.05,   0.03,   0.03,   0.03,   0.03,   0.03,   0.03,   0.03};
    float cuteop2[] = {0.55,   0.91,   0.91,   0.91,   0.91,   0.91,   0.91,   0.91,
                       0.85,   0.85,   0.85,   0.85,   0.85,   0.85,   0.85,   0.85};
    
    if(sigmaeecor < cutsee[cat_old+eb*8]) {
      if(fabs(deltaEtaIn) < cutdeta[cat_old+eb*8]) {
        if(fabs(deltaPhiIn) < cutdphi[cat_old+eb*8]) {
          if(eSeedOverPOut > cuteop2[cat_old+eb*8]) {
            if(hOverE < cuthoe[cat_old+eb*8]) {
              return true;
            }
          }
        }
      }
    }
    
    return false;
  }
  
  return false;
}
  
int cms1::classify_old(const reco::PixelMatchGsfElectron* electron) {
   
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

int cms1::classify(const reco::PixelMatchGsfElectron* electron) {
  
  double eta = electron->p4().Eta();
  double eOverP = electron->eSuperClusterOverP();
  double pin  = electron->trackMomentumAtVtx().R(); 
  double pout = electron->trackMomentumOut().R(); 
  double fBrem = (pin-pout)/pin;
  
  int cat;
  
  if((fabs(eta)<1.479 && fBrem<0.06) || (fabs(eta)>1.479 && fBrem<0.1)) 
    cat=1;
  else if (eOverP < 1.2 && eOverP > 0.8) 
    cat=0;
  else 
    cat=2;
  
  return cat;
}

