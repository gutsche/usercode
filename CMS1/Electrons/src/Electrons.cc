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
// $Date: 2007/06/19 15:27:53 $
// $Revision: 1.21 $
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
                                                                  const Cuts& userCuts, Cuts::IsolationType isolated, ElectronDef def) {

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
      cuts.eta_min = -2.4;
      cuts.eta_max = +2.4;
      cuts.AND(userCuts);
      cuts.setEventData(data_);

      return getElectrons(AllElectrons, cuts, isolated);
    }
    break;
  case TightElectrons:
    {
      // This type is based on LooseElectrons
      // set the default cuts for this type and merge them with user Cuts
      Cuts cuts;
      cuts.pt_min = 20;
      cuts.eta_min = -2.4;
      cuts.eta_max = +2.4;
      cuts.AND(userCuts);
      cuts.setEventData(data_);
      
      return getElectrons(AllElectrons, cuts, isolated);
    }
    break;
  case TruthMatchedElectrons:
    {
      // This type is based on LooseElectrons
      // test that all inputs are set correctly
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
      // This type is based on LooseElectrons
      // test that all inputs are set correctly
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
      // This type is based on LooseElectrons
      // test that all inputs are set correctly
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
      // This type is based on LooseElectrons
      // test that all inputs are set correctly
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
      // This type is based on LooseElectrons
      // test that all inputs are set correctly
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
  case Custom:
    {
      // This type is based on LooseElectrons
      // test that all inputs are set correctly
      if (!data_) {
	      std::cout << "ERROR: event data is not provided to the electron black box" << std::endl;
	      return output_list;
      }
      
      temp = getElectrons(AllElectrons, userCuts, isolated);
      for(std::vector<const reco::Candidate*>::const_iterator electron = temp.begin(); electron != temp.end(); ++electron) { 
        const reco::PixelMatchGsfElectron* ele = ( const reco::PixelMatchGsfElectron*)*electron; 
        if (classify(def, ele))
            output_list.push_back(*electron);
      }
    }
    break;
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
  std::cout << spp << std::endl;
  see = seedShapeRef->covEtaEta();
}

bool cms1::Electrons::classify(ElectronDef def, const reco::PixelMatchGsfElectron* electron) {

  bool result = false;
  reco::SuperClusterRef sclRef = electron->superCluster();

  float eMax, e3x3, e5x5, spp, see;
  R9_25(electron, eMax, e3x3, e5x5, spp, see);
  float ratio = eMax/e3x3;
  
  // use supercluster energy including f(Ncry) correction
  float scEnergy=sclRef->energy();
  
  // then decide to which class it belongs
  float p0 = 7.20583e-04;
  float p1 = 9.20913e-02;
  float p2 = 8.97269e+00;
  
  float pin  = electron->trackMomentumAtVtx().R(); 
  float pout = electron->trackMomentumOut().R(); 
  float peak = p0 + p1/(pin-p2);
  int nbrem = electron->numberOfClusters()-1;
 
  if (nbrem < def.numberOfSCSeed) {
    if ((pin - scEnergy)/pin < def.eOverP) {
      if (fabs(electron->caloPosition().phi() -
               electron->gsfTrack()->outerMomentum().phi() - peak) < def.deltaPhi) {
        if ((pin - pout)/pin < def.fBrem) {
          if (electron->hadronicOverEm() < def.hOverE) {
            if (ratio < def.R9) {
              return true;
            }
          }
        }
      }
    }
  }

  return result;
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
    std::cout << spp << std::endl;

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




