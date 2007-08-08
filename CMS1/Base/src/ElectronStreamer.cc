// Description:     Electron streamer class 
//
// Original Author: Matteo Sani
//
// $Author: sani $
// $Date: 2007/08/08 15:51:09 $
// $Revision: 1.1 $
//

#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"

#include "CMS1/Base/interface/Cuts.h"
#include "CMS1/Base/interface/ElectronUtil.h"
#include "CMS1/Base/interface/ElectronStreamer.h"

cms1::ElectronStreamer::ElectronStreamer() {

  nSeed = addInt("nSeed", " Number of basic clusters in the super cluster  ", -999);
  cms_class = addInt("class", " CMSSW electron classification", -999);
  looseId = addInt("looseId", " Loose identification result", -999);
  tightId = addInt("tightId", " Tight identification result", -999);
   
  hOverE = addFloat("hOverE", " Hadronic over Electromagnetic energy", -999);
  eOverPIn = addFloat("eOverPIn", " Super cluster energy over momentum at vertex", -999);
  eOverPOut = addFloat("eSeedOverPOut", " Super cluster seed energy over momentum at calo", -999);
  fBrem = addFloat("fBrem", " Fraction of brem energy according to tracker", -999); 
  dEtaIn = addFloat("dEtaIn", " Delta Eta between SC and momentum at vertex", -999);
  dEtaOut = addFloat("dEtaOut", " Delta Eta between SC and momentum at calo", -999);
  dPhiIn = addFloat("dPhiIn", " Delta Phi between SC and momentum at vertex", -999);
  dPhiOut = addFloat("dPhiOut", " Delta Phi between SC and momentum at calo", -999);
  varEsc = addFloat("ESc", " Super cluster corrected energy", -999);
  varRawEsc = addFloat("ESc_raw", " Super cluster raw energy", -999);
  vare3x3 = addFloat("e3x3", " Super cluster energy in a 3x3 crystal matrix", -999);
  vare5x5 = addFloat("e5x5", " Super cluster energy in a 5x5 crystal matrix", -999);
  eSeed = addFloat("ESeed", " Energy of the super cluster seed", -999);
  sPhiPhi = addFloat("sigmaPhiPhi", " Sigma Eta of the super cluster energy distribution", -999);
  sEtaEta = addFloat("sigmaEtaEta", " Sigma Phi of the super cluster energy distribution", -999);
  tkIso = addFloat("tkIso", " Tracker electron isolation", -999);
}

void cms1::ElectronStreamer::fill(const reco::Candidate* candidate, bool reset) {
  if (reset) 
    setDefaults();
  
  if (!candidate) 
    return;
  
  TrackStreamer::fill(candidate,false);
  
  if (const reco::PixelMatchGsfElectron* aElectron = dynamic_cast<const reco::PixelMatchGsfElectron*>(candidate)) {
    
    *nSeed = aElectron->numberOfClusters()-1;
    *cms_class = aElectron->classification();
    
    int id1=0, id2=0;
    if (identify(aElectron,barrelClShp,endcapClShp, 0))
      id1 = 1;
    if (identify(aElectron,barrelClShp,endcapClShp, 1))
      id2 = 1;
    *looseId = id1;
    *tightId = id2;
    
    *hOverE = aElectron->hadronicOverEm();
    *eOverPIn = aElectron->eSuperClusterOverP();
    *eOverPOut = aElectron->eSeedClusterOverPout();
    
    float pin  = aElectron->trackMomentumAtVtx().R(); 
    float pout = aElectron->trackMomentumOut().R();
    
    *fBrem = (pin-pout)/pin;
    *dEtaIn = aElectron->deltaEtaSuperClusterTrackAtVtx();
    *dEtaOut = aElectron->deltaEtaSeedClusterTrackAtCalo();
    *dPhiIn = aElectron->deltaPhiSuperClusterTrackAtVtx();
    *dPhiOut = aElectron->deltaPhiSeedClusterTrackAtCalo();
    
    float esc, e3x3, e5x5, see, spp;
    R9_25(barrelClShp, endcapClShp, aElectron, esc, e3x3, e5x5, spp, see);
    *varEsc = esc;
    *varRawEsc = aElectron->superCluster()->rawEnergy();
    *vare3x3 = e3x3;
    *vare5x5 = e5x5; 
    *eSeed = aElectron->superCluster()->seed()->energy();
    *sPhiPhi = spp;
    *sEtaEta = see;
    
    double iso = 0.;
    if (tracks) {
      iso = cms1::Cuts::trackRelIsolation(aElectron->trackMomentumAtVtx(), aElectron->vertex(), tracks, 0.3, 0.01, 0.1, 999.9, 0.5, 1.5, 7);
    } else 
      iso = 0.;
    *tkIso = iso;
  } 
}

void cms1::ElectronStreamer::fill(const StreamerArguments& args, bool reset) {
  if (reset) 
    setDefaults();
  TrackStreamer::fill(args,false);
  if (args.candidate) 
    fill(args.candidate, false);
}

void cms1::ElectronStreamer::addCollections(const reco::BasicClusterShapeAssociationCollection* a ,
                                            const reco::BasicClusterShapeAssociationCollection* b) {

  barrelClShp = a;
  endcapClShp = b;
}

void cms1::ElectronStreamer::addCollections(const std::vector<reco::Track>* c) {
  tracks = c;
}

