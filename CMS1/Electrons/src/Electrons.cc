//
// Package:         CMS1/Electrons
// Class:           Electrons
// 
// Description:     analysis functions for electrons
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Wed Feb 21 00:15:42 UTC 2007
//
// $Author: dmytro $
// $Date: 2007/05/11 04:12:17 $
// $Revision: 1.12 $
//

#include "CMS1/Electrons/interface/Electrons.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"

#include "DataFormats/EgammaReco/interface/BasicClusterShapeAssociation.h"
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"

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
      
    //for (std::vector<reco::PixelMatchGsfElectron>::const_iterator el = collection->begin(); el != collection->end(); ++el) {
    // Here we make simple cuts in a standard way
    //  if (cuts.testCandidate(*el)) 
    //    output_list.push_back(&*el);
    // }
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
      return temp;
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
      
      return temp;
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
      
      return temp;
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
      
      return temp;
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
      
      return temp;
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

bool cms1::Electrons::classify(ElectronDef def, const reco::PixelMatchGsfElectron* electron) {

  bool result = false;
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
  double ratio = seedShapeRef->eMax()/seedShapeRef->e3x3();
  
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
void cms1::Electrons::registerEventUserData()
{
   tracks.registerBlock( *data_, "electrons",     "cms1_electrons");
}

void cms1::Electrons::fillEventUserData()
{
   const std::vector<reco::PixelMatchGsfElectron>* collection = 
     data_->getData<std::vector<reco::PixelMatchGsfElectron> >("pixelMatchGsfElectrons");
   if ( ! collection ) {
      std::cout << "ERROR: pixelMatchGsfElectrons collection is not found in the event. Return nothing." << std::endl;
      return;
   }
   for ( std::vector<reco::PixelMatchGsfElectron>::const_iterator element = collection->begin();
	 element != collection->end(); ++element ) 
     tracks.fill(*(element->gsfTrack().get()));
}


