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
// $Date: 2007/08/08 15:44:21 $
// $Revision: 1.30 $
//

#include "CMS1/Electrons/interface/Electrons.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"

#include "DataFormats/EgammaReco/interface/BasicClusterShapeAssociation.h"
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"
#include "CMS1/CommonTools/interface/UserDataTools.h"
#include "CMS1/Base/interface/ElectronUtil.h"

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

std::vector<const reco::Candidate*> cms1::Electrons::getElectrons(const std::string& electronType,
                                                                  const Cuts& userCuts, Cuts::IsolationType isolated) {

   // define output collection
   std::vector<const reco::Candidate*> output_list, temp;
   if (!data_) {
      std::cout << "ERROR: electron black box doesn't know where to find EvenData." << std::endl;
      assert(0);
   }
  
   //////////////////////////////////////////////////////////////////////////////////
   
   if ( electronType.compare("AllElectrons")==0 )
     {
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
	   if (!defaultCuts.testCandidate(*electron)) continue;
	   if (!userCuts.testCandidate(*electron)) continue;
	   output_list.push_back(&*electron);
	} 
	return output_list;
     }
   
   //////////////////////////////////////////////////////////////////////////////////
   
   if ( electronType.compare("LooseElectrons")==0 )
     {
	// set the default cuts for this type
	Cuts cuts;
	cuts.pt_min = 19;
	cuts.eta_min = -2.5;
	cuts.eta_max = +2.5;
	cuts.AND(userCuts);
	cuts.setEventData(data_);
	return getElectrons("AllElectrons", cuts, isolated);
     }
   
   //////////////////////////////////////////////////////////////////////////////////

   if (electronType.compare("TightElectrons")==0 )
     {
	Cuts cuts;
	cuts.pt_min = 20;
	cuts.eta_min = -2.5;
	cuts.eta_max = +2.5;
	cuts.AND(userCuts);
	cuts.setEventData(data_);
	return getElectrons("AllElectrons", cuts, isolated);
     }

   //////////////////////////////////////////////////////////////////////////////////

   if ( electronType.compare("Golden")==0 )
     {
	temp = getElectrons("AllElectrons", userCuts, isolated); 

	for(std::vector<const reco::Candidate*>::const_iterator electron = temp.begin(); electron != temp.end(); ++electron) { 
	   const reco::PixelMatchGsfElectron* ele = ( const reco::PixelMatchGsfElectron*)*electron;
	   if((ele)->classification() == 0 || (ele)->classification() == 100) 
	     output_list.push_back(*electron);
	}
	return output_list;
     }
 
   //////////////////////////////////////////////////////////////////////////////////
   
   if ( electronType.compare("BigBrem")==0 )
     {
	temp = getElectrons("AllElectrons", userCuts, isolated);
	for(std::vector<const reco::Candidate*>::const_iterator electron = temp.begin(); electron != temp.end(); ++electron) { 
	   const reco::PixelMatchGsfElectron* ele = ( const reco::PixelMatchGsfElectron*)*electron;
	   if((ele)->classification() == 10 || (ele)->classification() == 110) 
	     output_list.push_back(*electron);
	}
	return output_list;
     }
 
   //////////////////////////////////////////////////////////////////////////////////
 
   if ( electronType.compare("Narrow")==0 )
     {
	temp = getElectrons("AllElectrons", userCuts, isolated);
	for(std::vector<const reco::Candidate*>::const_iterator electron = temp.begin(); electron != temp.end(); ++electron) { 
	   const reco::PixelMatchGsfElectron* ele = ( const reco::PixelMatchGsfElectron*)*electron;
	   if((ele)->classification() == 20 || (ele)->classification() == 120) 
	     output_list.push_back(*electron);
	}
	return output_list;
     }
   
   //////////////////////////////////////////////////////////////////////////////////
 
   if ( electronType.compare("Showering")==0 )
     {
	temp = getElectrons("AllElectrons", userCuts, isolated);
	for(std::vector<const reco::Candidate*>::const_iterator electron = temp.begin(); electron != temp.end(); ++electron) { 
	   const reco::PixelMatchGsfElectron* ele = ( const reco::PixelMatchGsfElectron*)*electron;
	   if(((ele)->classification() >= 30 && (ele)->classification()<= 34) ||
	      ((ele)->classification() >= 130 && (ele)->classification() <= 134)) 
	     output_list.push_back(*electron);
	}
	return output_list;
     }
   
   //////////////////////////////////////////////////////////////////////////////////
   
   std::cout << "Unkown or not implemented electron type: " << electronType << "\nAbort" <<std::endl;
   assert(0);
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
      double isoRel = cms1::Cuts::trackRelIsolation(cp->momentum(), cp->vertex(), tracks, 0.3, 0.01, 0.1, 999.9, 0.5, 1.5, 7);
      o << ", isol = " << isoRel;
    }
    o << std::endl; 
	}
}

void cms1::Electrons::registerEventUserData() {
  
  evtElectrons.registerBlock( *data_, "els_", "reference electrons,");
  data_->intUserData.push_back( new UserData<int>("evt_nels", "number of electrons in the reference collection", false) );
  nElectrons = data_->intUserData.back();
}

void cms1::Electrons::fillEventUserData() {

  const reco::BasicClusterShapeAssociationCollection* barrelClShp = data_->getData<reco::BasicClusterShapeAssociationCollection>("hybridSuperClusters", "hybridShapeAssoc");
  const reco::BasicClusterShapeAssociationCollection* endcapClShp = data_->getData<reco::BasicClusterShapeAssociationCollection>("islandBasicClusters", "islandEndcapShapeAssoc");
  const std::vector<reco::Track>* tracks = data_->getData<std::vector<reco::Track> >("ctfWithMaterialTracks");
  
  // std::vector<const reco::Candidate*> els = getElectrons("AllElectrons",Cuts());
  // data_->addBBCollection("refElectrons", els); 
  std::vector<const reco::Candidate*> els = data_->getBBCollection("refElectrons");
  evtElectrons.addCollections(&(*barrelClShp), &(*endcapClShp));
  evtElectrons.addCollections(&(*tracks));
  evtElectrons.fill( getStreamerArguments(data_, els) );
  nElectrons->addData(els.size());
}  




