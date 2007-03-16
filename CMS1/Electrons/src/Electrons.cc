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
// $Date: 2007/03/10 02:25:40 $
// $Revision: 1.4 $
//

#include "CMS1/Electrons/interface/Electrons.h"
#include "DataFormats/EgammaCandidates/interface/SiStripElectron.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"

std::vector<const reco::SiStripElectron*> cms1::Electrons::getElectrons(const ElectronType electronType,
									const Cuts& userCuts,
									Cuts::IsolationType isolated)
{
   // define output collection
   std::vector<const reco::SiStripElectron*> output_list;
   
   switch (electronType) {
    case LooseElectrons:
	{
	   // check inputs
	   if (! data_ || ! data_->siStripElectrons) {
	      std::cout << "ERROR: electron collection is not set" << std::endl;
	      return output_list;
	   }
	   if (isolated && ! data_->tracks) {
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
	   
	   for ( std::vector<reco::SiStripElectron>::const_iterator electron = data_->siStripElectrons->begin();
		 electron != data_->siStripElectrons->end();
		 ++electron ) 
	     {
		if ( ! defaultCuts.testCandidate(*electron) ) continue;
		if ( ! userCuts.testCandidate(*electron) ) continue;
		
		// cut on E/p
		double energy = electron->superCluster()->energy();
		std::cout << "Electron (p,E): " << electron->p() << ", " << energy <<std::endl;
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
