#include "CMS1/ElectronAnalyzer/interface/ElIdUserBlock.h"
#include "CMS1/Electrons/interface/Electrons.h"
#include "CMS1/Base/interface/UserDataBlocks.h"
#include "CMS1/Base/interface/TrackStreamer.h"
#include "CMS1/CommonTools/interface/UserDataTools.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"

#include "DataFormats/EgammaReco/interface/BasicClusterShapeAssociation.h"
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"

cms1::ElIdUserBlock::ElIdUserBlock() { 

  isCandidate_ = true;
  el.setCandidateFlag(true);
}

void cms1::ElIdUserBlock::registerBlock(EventData& event, const std::string& name_prefix, const std::string& alias_prefix) {

  el.registerBlock(event, name_prefix+"elid_", alias_prefix);
  gp.registerBlock(event, name_prefix+"other_mc_el_", alias_prefix);
  
  event.intUserData1D.push_back( new UserDataInt1D("nSeed", "elid_", "cms1_els_", false) );
  nSeed = event.intUserData1D.back(); 
  event.intUserData1D.push_back( new UserDataInt1D("class", "elid_", "cms1_els_", false) );
  cms_class = event.intUserData1D.back(); 

  event.floatUserData1D.push_back( new UserDataFloat1D("hOverE", "elid_", "cms1_els_", false) );
  hOverE = event.floatUserData1D.back(); 
  event.floatUserData1D.push_back( new UserDataFloat1D("eOverPIn", "elid_", "cms1_els_", false) );
  eOverPIn = event.floatUserData1D.back();
  event.floatUserData1D.push_back( new UserDataFloat1D("eSeedOverPOut", "elid_", "cms1_els_", false) );
  eOverPOut = event.floatUserData1D.back();
  event.floatUserData1D.push_back( new UserDataFloat1D("fBrem", "elid_", "cms1_els_", false) );
  fBrem = event.floatUserData1D.back(); 

  event.floatUserData1D.push_back(new UserDataFloat1D("dEtaIn", "elid_", "cms1_els_", false));
  dEtaIn = event.floatUserData1D.back();
  event.floatUserData1D.push_back(new UserDataFloat1D("dEtaOut", "elid_", "cms1_els_", false) );
  dEtaOut = event.floatUserData1D.back();
  event.floatUserData1D.push_back(new UserDataFloat1D("dPhiIn", "elid_", "cms1_els_", false) );
  dPhiIn = event.floatUserData1D.back();
  event.floatUserData1D.push_back(new UserDataFloat1D("dPhiOut", "elid_", "cms1_els_", false) );
  dPhiOut = event.floatUserData1D.back();
  event.floatUserData1D.push_back(new UserDataFloat1D("ESc", "elid_", "cms1_els_", false));
  vareMax = event.floatUserData1D.back(); 
  event.floatUserData1D.push_back(new UserDataFloat1D("e3x3", "elid_", "cms1_els_", false));
  vare3x3 = event.floatUserData1D.back();
  event.floatUserData1D.push_back(new UserDataFloat1D("e5x5", "elid_", "cms1_els_", false));
  vare5x5 = event.floatUserData1D.back(); 
  event.floatUserData1D.push_back(new UserDataFloat1D("ESeed", "elid_", "cms1_els_", false));
  eSeed = event.floatUserData1D.back();  
  event.floatUserData1D.push_back(new UserDataFloat1D("sigmaPhiPhi", "elid_", "cms1_els_", false));
  sPhiPhi = event.floatUserData1D.back();  
  event.floatUserData1D.push_back(new UserDataFloat1D("sigmaEtaEta", "elid_", "cms1_els_", false));
  sEtaEta = event.floatUserData1D.back(); 
  event.floatUserData1D.push_back(new UserDataFloat1D("tkIso", "elid_", "cms1_els_", false));
  tkIso = event.floatUserData1D.back(); 
}

void cms1::ElIdUserBlock::fill(EventData& event, std::vector<const reco::Candidate*> candidate, std::vector<const HepMC::GenParticle*> mcel) {
  el.fill(getStreamerArguments(&event, candidate));
  gp.fill(mcel);

  float eMax=0., e3x3=0., e5x5=0., see, spp;
  double iso;

  std::vector<int> vint0, vint1;
  std::vector<float> vfloat0,vfloat1,vfloat2,vfloat3;
  std::vector<float> vfloat4,vfloat5,vfloat6,vfloat7,vfloat8;
  std::vector<float> vfloat9,vfloat10, vfloat11, vfloat12, vfloat13, vfloat14;
  std::vector<const reco::Candidate*>::const_iterator it;

  // add isolation info
  const  std::vector<reco::Track>* tracks = event.getData<std::vector<reco::Track> >("ctfWithMaterialTracks");

  for(it = candidate.begin(); it != candidate.end(); ++it) {

    const reco::PixelMatchGsfElectron* electron = dynamic_cast<const reco::PixelMatchGsfElectron*>(*it);
    
    if (tracks) {
      iso = cms1::Cuts::trackRelIsolation(electron->trackMomentumAtVtx(), electron->vertex(), tracks, 0.3, 0.01, 0.1, 0.1, 0.2, 1.5);
    } else 
      iso = 0.;

    float pin  = electron->trackMomentumAtVtx().R(); 
    float pout = electron->trackMomentumOut().R();

    R9_25(event, electron, eMax, e3x3, e5x5, spp, see);

    vint0.push_back(electron->numberOfClusters()-1);
    vint1.push_back(electron->classification());
    
    vfloat0.push_back(electron->hadronicOverEm());                  
    vfloat1.push_back(electron->eSuperClusterOverP());                  
    vfloat2.push_back(electron->eSeedClusterOverPout());                
    vfloat3.push_back((pin-pout)/pin);                        
    
    vfloat4.push_back(electron->deltaEtaSuperClusterTrackAtVtx());  
    vfloat5.push_back(electron->deltaEtaSeedClusterTrackAtCalo());
    vfloat6.push_back(electron->deltaPhiSuperClusterTrackAtVtx());
    vfloat7.push_back(electron->deltaPhiSeedClusterTrackAtCalo());  
    vfloat8.push_back(eMax);
    vfloat9.push_back(e3x3);
    vfloat10.push_back(e5x5);
    vfloat11.push_back(electron->superCluster()->seed()->energy());
    vfloat12.push_back(spp);
    vfloat13.push_back(see);
    vfloat14.push_back(iso);
  }

  nSeed->addData(vint0);
  cms_class->addData(vint1);
  
  hOverE->addData(vfloat0);
  eOverPIn->addData(vfloat1);
  eOverPOut->addData(vfloat2);
  fBrem->addData(vfloat3);
  
  dEtaIn->addData(vfloat4);
  dEtaOut->addData(vfloat5);
  dPhiIn->addData(vfloat6);
  dPhiOut->addData(vfloat7);
  vareMax->addData(vfloat8);
  vare3x3->addData(vfloat9);
  vare5x5->addData(vfloat10);
  eSeed->addData(vfloat11);
  sPhiPhi->addData(vfloat12);
  sEtaEta->addData(vfloat13);
  tkIso->addData(vfloat14);
}

void cms1::ElIdUserBlock::R9_25(EventData& event, const reco::PixelMatchGsfElectron* electron,
			   float& eMax, float& e3x3, float& e5x5, float& spp, float& see) { 

  reco::SuperClusterRef sclRef=electron->superCluster();
  
  const reco::BasicClusterShapeAssociationCollection* barrelClShp = event.getData<reco::BasicClusterShapeAssociationCollection>("hybridSuperClusters", "hybridShapeAssoc");
  const reco::BasicClusterShapeAssociationCollection* endcapClShp = event.getData<reco::BasicClusterShapeAssociationCollection>("islandBasicClusters", "islandEndcapShapeAssoc");
  
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
  spp = seedShapeRef->covPhiPhi();
  see = seedShapeRef->covEtaEta();
}
