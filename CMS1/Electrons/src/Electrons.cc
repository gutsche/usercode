//
// Package:         CMS1/Electrons
// Class:           Electrons
// 
// Description:     analysis functions for electrons
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Wed Feb 21 00:15:42 UTC 2007
//
// $Author: sani $
// $Date: 2007/07/06 16:43:59 $
// $Revision: 1.24 $
//

#include "CMS1/Electrons/interface/Electrons.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"

#include "DataFormats/EgammaReco/interface/BasicClusterShapeAssociation.h"
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"
#include "CMS1/CommonTools/interface/UserDataTools.h"

void cms1::Electrons::removeElectrons(const std::vector<reco::PixelMatchGsfElectron>* collection) {
  
  std::vector<reco::PixelMatchGsfElectron>* newEl = const_cast<std::vector<reco::PixelMatchGsfElectron>* >(collection);
  std::vector<reco::PixelMatchGsfElectron> copy = *newEl;

  newEl->clear();
  
  std::vector<reco::PixelMatchGsfElectron>::iterator it1, it2;

  for(it1=copy.begin(); it1!=copy.end(); ++it1) {

    bool isRemoved = false;
    for(it2=copy.begin(); it2!=copy.end(); ++it2) {
      if (it1 == it2)
        continue;
      if (((*it1).superCluster().id() == (*it2).superCluster().id()) &&
          ((*it1).superCluster().index() == (*it2).superCluster().index())) {

        float deltaEp1 = fabs((*it1).eSuperClusterOverP() - 1.);
        float deltaEp2 = fabs((*it2).eSuperClusterOverP() - 1.);
        if (deltaEp1 > deltaEp2) {
          isRemoved = true;
          break;
        }
      }
    }
    
    if (!isRemoved)
      newEl->push_back(*it1);

  }
}

std::vector<const reco::Candidate*> cms1::Electrons::getElectrons(const ElectronType electronType,
                                                                  const Cuts& userCuts, Cuts::IsolationType isolated) {

  // define output collection
  std::vector<const reco::Candidate*> output_list, temp;
  
  switch (electronType) {
  case AllElectrons:
    {
      if (!data_) {
        std::cout << "ERROR: electron black box doesn't know where to find EvenData." << std::endl;
        return output_list;
      }
      const std::vector<reco::PixelMatchGsfElectron>* collection = data_->getData<std::vector<reco::PixelMatchGsfElectron> >("pixelMatchGsfElectrons");
      if (!collection) {
        std::cout << "ERROR: electron collection is not found in the event. Return nothing." << std::endl;
        return output_list;
      }

      // remove electrons reconstructed twice
      if (collection->size() > 1)
        removeElectrons(collection);

      if (isolated && !data_->getData<std::vector<reco::Track> >("ctfWithMaterialTracks") ) {
        std::cout << "ERROR: track collection for electron isolation is not set" << std::endl;
        return output_list;
      }

      // set the default cuts for this type
      Cuts defaultCuts;
      defaultCuts.isolated = isolated;
      defaultCuts.setEventData(data_);
      defaultCuts.AND(userCuts);

      for (std::vector<reco::PixelMatchGsfElectron>::const_iterator electron = collection->begin(); electron != collection->end(); ++electron) {
        if (!defaultCuts.testCandidate(*electron)) 
          continue;
        if (!userCuts.testCandidate(*electron)) 
          continue;
        output_list.push_back(&*electron);
      } 
    }
    break;
  case LooseElectrons:
    {
      // set the default cuts for this type
      Cuts cuts;
      cuts.pt_min = 19;
      cuts.eta_min = -2.5;
      cuts.eta_max = +2.5;
      cuts.AND(userCuts);
      cuts.setEventData(data_);

      temp = getElectrons(AllElectrons, userCuts, isolated); 

      for(std::vector<const reco::Candidate*>::const_iterator electron = temp.begin(); electron != temp.end(); ++electron) { 
        const reco::PixelMatchGsfElectron* ele = ( const reco::PixelMatchGsfElectron*)*electron;
        if (identify(ele, 0))
          output_list.push_back(*electron);
      }
    }
    break;
  case TightElectrons:
    {
      Cuts cuts;
      cuts.pt_min = 20;
      cuts.eta_min = -2.5;
      cuts.eta_max = +2.5;
      cuts.AND(userCuts);
      cuts.setEventData(data_);
      temp = getElectrons(AllElectrons, userCuts, isolated); 
      
      for(std::vector<const reco::Candidate*>::const_iterator electron = temp.begin(); electron != temp.end(); ++electron) { 
        const reco::PixelMatchGsfElectron* ele = ( const reco::PixelMatchGsfElectron*)*electron;
        if (identify(ele, 1))
          output_list.push_back(*electron);
      }
    }

    break;
  case TruthMatchedElectrons:
    {
      if (!data_) {
	      std::cout << "ERROR: event data is not provided to the electron black box" << std::endl;
	      return output_list;
      }
	   
      // set the default cuts for this type and merge them with user Cuts
      Cuts cuts;
      cuts.truthMatchingType = Cuts::Electron;
      cuts.AND(userCuts);
      cuts.setEventData(data_);
	   
      return getElectrons(AllElectrons, cuts, isolated);
    }
    break;
  case Golden:
    {
      if (!data_) {
	      std::cout << "ERROR: event data is not provided to the electron black box" << std::endl;
	      return output_list;
      }
      temp = getElectrons(AllElectrons, userCuts, isolated); 

      for(std::vector<const reco::Candidate*>::const_iterator electron = temp.begin(); electron != temp.end(); ++electron) { 
        const reco::PixelMatchGsfElectron* ele = ( const reco::PixelMatchGsfElectron*)*electron;
        if((ele)->classification() == 0 || (ele)->classification() == 100) 
          output_list.push_back(*electron);
      }
    }
    break;
  case BigBrem:
    {
      if (!data_) {
	      std::cout << "ERROR: event data is not provided to the electron black box" << std::endl;
	      return output_list;
      }
      
      temp = getElectrons(AllElectrons, userCuts, isolated);
      for(std::vector<const reco::Candidate*>::const_iterator electron = temp.begin(); electron != temp.end(); ++electron) { 
        const reco::PixelMatchGsfElectron* ele = ( const reco::PixelMatchGsfElectron*)*electron;
        if((ele)->classification() == 10 || (ele)->classification() == 110) 
          output_list.push_back(*electron);
      }
    }
    break;
  case Narrow:
    {
      if (!data_) {
	      std::cout << "ERROR: event data is not provided to the electron black box" << std::endl;
	      return output_list;
      }
      
      temp = getElectrons(AllElectrons, userCuts, isolated);
      for(std::vector<const reco::Candidate*>::const_iterator electron = temp.begin(); electron != temp.end(); ++electron) { 
        const reco::PixelMatchGsfElectron* ele = ( const reco::PixelMatchGsfElectron*)*electron;
        if((ele)->classification() == 20 || (ele)->classification() == 120) 
          output_list.push_back(*electron);
      }
    }
    break;
  case Showering:
    {
      if (!data_) {
	      std::cout << "ERROR: event data is not provided to the electron black box" << std::endl;
	      return output_list;
      }
      
      temp = getElectrons(AllElectrons, userCuts, isolated);
      for(std::vector<const reco::Candidate*>::const_iterator electron = temp.begin(); electron != temp.end(); ++electron) { 
        const reco::PixelMatchGsfElectron* ele = ( const reco::PixelMatchGsfElectron*)*electron;
        if(((ele)->classification() >= 30 && (ele)->classification()<= 34) ||
           ((ele)->classification() >= 130 && (ele)->classification() <= 134)) 
          output_list.push_back(*electron);
      }
    }
  default:
    std::cout << "Unkown or not implemented type" << std::endl;
  }
  return output_list;
}

void cms1::Electrons::dump(std::ostream& o, std::vector<const reco::Candidate*> el) {
	for (std::vector<const reco::Candidate*>::iterator i = el.begin(), ie = el.end(); i != ie; ++i) {
		const reco::Candidate* cp = *i;
		o << "Electron "; 
		o << "Pt = " << cp->pt(); 
		o << ", Eta = " << cp->eta(); 
		o << ", Phi = " << cp->phi();
    const  std::vector<reco::Track>* tracks = data_->getData<std::vector<reco::Track> >("ctfWithMaterialTracks");
    if (tracks) {
      double isoRel = cms1::Cuts::trackRelIsolation(cp->momentum(), cp->vertex(), tracks, 0.3, 0.01, 0.1, 0.1, 0.2, 1.5);
      o << ", isol = " << isoRel;
    }
    o << std::endl; 
	}
}

void cms1::Electrons::R9_25(const reco::PixelMatchGsfElectron* electron,
			   float& eMax, float& e3x3, float& e5x5, float& spp, float& see) { 

  reco::SuperClusterRef sclRef=electron->superCluster();

  const reco::BasicClusterShapeAssociationCollection* barrelClShp = data_->getData<reco::BasicClusterShapeAssociationCollection>("hybridSuperClusters", "hybridShapeAssoc");
  const reco::BasicClusterShapeAssociationCollection* endcapClShp = data_->getData<reco::BasicClusterShapeAssociationCollection>("islandBasicClusters", "islandEndcapShapeAssoc");
  
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

bool cms1::Electrons::identify(const reco::PixelMatchGsfElectron* electron, int type) {

  float dummy, e3x3, e5x5, i, l;
  R9_25(electron, dummy, e3x3, e5x5, l, i);
 
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

int cms1::Electrons::classify(const reco::PixelMatchGsfElectron* electron) {

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

void cms1::Electrons::registerEventUserData() {
  
  evtElectrons.registerBlock( *data_, "els_", "cms1_els_");
  data_->intUserData.push_back( new UserData<int>("nels", "evt_", "cms1_evt_", false) );
  nElectrons = data_->intUserData.back();
  
  data_->intUserData1D.push_back( new UserDataInt1D("nSeed", "elid_", "cms1_elid_", false) );
  nSeed = data_->intUserData1D.back(); 
  data_->intUserData1D.push_back( new UserDataInt1D("class", "elid_", "cms1_elid_", false) );
  cms_class = data_->intUserData1D.back(); 
  
  data_->floatUserData1D.push_back( new UserDataFloat1D("hOverE", "elid_", "cms1_elid_", false) );
  hOverE = data_->floatUserData1D.back(); 
  data_->floatUserData1D.push_back( new UserDataFloat1D("eOverPIn", "elid_", "cms1_elid_", false) );
  eOverPIn = data_->floatUserData1D.back();
  data_->floatUserData1D.push_back( new UserDataFloat1D("eSeedOverPOut", "elid_", "cms1_elid_", false) );
  eOverPOut = data_->floatUserData1D.back();
  data_->floatUserData1D.push_back( new UserDataFloat1D("fBrem", "elid_", "cms1_elid_", false) );
  fBrem = data_->floatUserData1D.back(); 
  
  data_->floatUserData1D.push_back(new UserDataFloat1D("dEtaIn", "elid_", "cms1_elid_", false));
  dEtaIn = data_->floatUserData1D.back();
  data_->floatUserData1D.push_back(new UserDataFloat1D("dEtaOut", "elid_", "cms1_elid_", false) );
  dEtaOut = data_->floatUserData1D.back();
  data_->floatUserData1D.push_back(new UserDataFloat1D("dPhiIn", "elid_", "cms1_elid_", false) );
  dPhiIn = data_->floatUserData1D.back();
  data_->floatUserData1D.push_back(new UserDataFloat1D("dPhiOut", "elid_", "cms1_elid_", false) );
  dPhiOut = data_->floatUserData1D.back();
  data_->floatUserData1D.push_back(new UserDataFloat1D("ESc", "elid_", "cms1_elid_", false));
  varEsc = data_->floatUserData1D.back(); 
  data_->floatUserData1D.push_back(new UserDataFloat1D("ESc_raw", "elid_", "cms1_elid_", false));
  varRawEsc = data_->floatUserData1D.back(); 
  data_->floatUserData1D.push_back(new UserDataFloat1D("e3x3", "elid_", "cms1_elid_", false));
  vare3x3 = data_->floatUserData1D.back();
  data_->floatUserData1D.push_back(new UserDataFloat1D("e5x5", "elid_", "cms1_elid_", false));
  vare5x5 = data_->floatUserData1D.back(); 
  data_->floatUserData1D.push_back(new UserDataFloat1D("ESeed", "elid_", "cms1_elid_", false));
  eSeed = data_->floatUserData1D.back();  
  data_->floatUserData1D.push_back(new UserDataFloat1D("sigmaPhiPhi", "elid_", "cms1_elid_", false));
  sPhiPhi = data_->floatUserData1D.back();  
  data_->floatUserData1D.push_back(new UserDataFloat1D("sigmaEtaEta", "elid_", "cms1_elid_", false));
  sEtaEta = data_->floatUserData1D.back(); 
  data_->floatUserData1D.push_back(new UserDataFloat1D("tkIso", "elid_", "cms1_elid_", false));
  tkIso = data_->floatUserData1D.back(); 
}

void cms1::Electrons::fillEventUserData() {

  float eMax=0., e3x3=0., e5x5=0., see, spp;
  double iso;

  std::vector<const reco::Candidate*> els = getElectrons(AllElectrons,Cuts());
  data_->addBBCollection("refElectrons",els);
  //data_->refElectrons = els;

  evtElectrons.fill(getStreamerArguments(data_, els));
  nElectrons->addData(els.size());
  
  std::vector<int> vint0, vint1;
  std::vector<float> vfloat0,vfloat1,vfloat2,vfloat3;
  std::vector<float> vfloat4,vfloat5,vfloat6,vfloat7,vfloat8;
  std::vector<float> vfloat9,vfloat10, vfloat11, vfloat12, vfloat13, vfloat14, vfloat15;
  std::vector<const reco::Candidate*>::const_iterator it;

  // add isolation info
  const  std::vector<reco::Track>* tracks = data_->getData<std::vector<reco::Track> >("ctfWithMaterialTracks");

  for(it = els.begin(); it != els.end(); ++it) {

    const reco::PixelMatchGsfElectron* el = dynamic_cast<const reco::PixelMatchGsfElectron*>(*it);
    
    if (tracks) {
      iso = cms1::Cuts::trackRelIsolation(el->trackMomentumAtVtx(), el->vertex(), tracks, 0.3, 0.01, 0.1, 0.1, 0.2, 1.5);
    } else 
      iso = 0.;

    float pin  = el->trackMomentumAtVtx().R(); 
    float pout = el->trackMomentumOut().R();

    R9_25(el, eMax, e3x3, e5x5, spp, see);

    vint0.push_back(el->numberOfClusters()-1);
    vint1.push_back(el->classification());
    
    vfloat0.push_back(el->hadronicOverEm());                  
    vfloat1.push_back(el->eSuperClusterOverP());                  
    vfloat2.push_back(el->eSeedClusterOverPout());                
    vfloat3.push_back((pin-pout)/pin);                        
    
    vfloat4.push_back(el->deltaEtaSuperClusterTrackAtVtx());  
    vfloat5.push_back(el->deltaEtaSeedClusterTrackAtCalo());
    vfloat6.push_back(el->deltaPhiSuperClusterTrackAtVtx());
    vfloat7.push_back(el->deltaPhiSeedClusterTrackAtCalo());  
    vfloat8.push_back(eMax);
    vfloat15.push_back(el->superCluster()->rawEnergy());
    vfloat9.push_back(e3x3);
    vfloat10.push_back(e5x5);
    vfloat11.push_back(el->superCluster()->seed()->energy());
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
  varEsc->addData(vfloat8);
  varRawEsc->addData(vfloat15);
  vare3x3->addData(vfloat9);
  vare5x5->addData(vfloat10);
  eSeed->addData(vfloat11);
  sPhiPhi->addData(vfloat12);
  sEtaEta->addData(vfloat13);
  tkIso->addData(vfloat14);
}  




