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
// $Date: 2007/04/12 19:40:37 $
// $Revision: 1.7 $
//

#include "CMS1/Electrons/interface/Electrons.h"
#include "CMS1/Base/interface/Cuts.h"
#include "DataFormats/EgammaCandidates/interface/SiStripElectron.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"

std::vector<const reco::Candidate*> cms1::Electrons::getElectrons(const ElectronType electronType,
									const Cuts& userCuts,
									Cuts::IsolationType isolated)
{
   // define output collection
   std::vector<const reco::Candidate*> output_list;
   
   switch (electronType) {
    case LooseElectrons:
	{
	   // check inputs
	   if (! data_ ) {
	      std::cout << "ERROR: electron black box doesn't know where to find EvenData." << std::endl;
	      return output_list;
	   }
	   const std::vector<reco::SiStripElectron>* collection = 
	     data_->container_reco_SiStripElectron.getCollection(edm::InputTag("siStripElectrons",""));
	   if ( ! collection ) {
	      std::cout << "ERROR: electron muon collection is not found in the event. Return nothing." << std::endl;
	      return output_list;
	   }

	   if (isolated && ! data_->container_reco_Track.getCollection(edm::InputTag("ctfWithMaterialTracks","")) ) {
	      std::cout << "ERROR: track collection for electron isolation is not set" << std::endl;
	      return output_list;
	   }
	   
	   // set the default cuts for this type
	   Cuts defaultCuts;
	   defaultCuts.pt_min = 19;
	   defaultCuts.eta_min = -2.4;
	   defaultCuts.eta_max = +2.4;
	   defaultCuts.isolated = isolated;
	   defaultCuts.setEventData( data_ );
	   
	   for ( std::vector<reco::SiStripElectron>::const_iterator electron = collection->begin();
		 electron != collection->end();
		 ++electron ) 
	     {
		if ( ! defaultCuts.testCandidate(*electron) ) continue;
		if ( ! userCuts.testCandidate(*electron) ) continue;
		
		// cut on E/p
		double energy = electron->superCluster()->energy();
		if (electron->p() < 1 || energy/electron->p() <0.5) continue;
		
		output_list.push_back(&*electron);
	     }
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
	   
	   return getElectrons(LooseElectrons, cuts, isolated);
	}
      break;
    case TruthMatchedElectrons:
	{
	   // This type is based on LooseElectrons

	   // test that all inputs are set correctly
	   if (! data_ || ! data_->mcInfo) {
	      std::cout << "ERROR: truth match info is not provided to the electron black box" << std::endl;
	      return output_list;
	   }
	   
	   // set the default cuts for this type and merge them with user Cuts
	   Cuts cuts;
	   cuts.truthMatchingType = Cuts::Electron;
	   cuts.AND(userCuts);
	   cuts.setEventData(data_);
	   
	   return getElectrons(LooseElectrons, cuts, isolated);
	}
      break;
    default:
      std::cout << "Unkown or not implemented type" << std::endl;
   }
   return output_list;
}

void cms1::Electrons::dump(ostream& o, std::vector<const reco::Candidate*> el) {
	for ( std::vector<const reco::Candidate*>::iterator i = el.begin(), ie = el.end(); i != ie; ++i ) {
		const reco::Candidate* cp = *i;
		o << "Electron "; 
		o << "Pt = " << cp->pt(); 
		o << ", Eta = " << cp->eta(); 
		o << ", Phi = " << cp->phi();
	   const  std::vector<reco::Track>* tracks = data_->container_reco_Track.getCollection(edm::InputTag("ctfWithMaterialTracks",""));
	   if ( tracks) {
	      double isoRel = cms1::Cuts::trackRelIsolation(cp->momentum(), cp->vertex(), tracks, 0.3, 0.01, 0.1, 0.1, 0.2, 1.5);
	      o << ", isol = " << isoRel;
	   }
	   o << std::endl; 
	}
}
