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
// $Date: 2007/03/10 01:34:47 $
// $Revision: 1.3 $
//

#include "CMS1/Electrons/interface/Electrons.h"
#include "DataFormats/EgammaCandidates/interface/SiStripElectron.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"

std::vector<const reco::SiStripElectron*> cms1::Electrons::getElectrons(const ElectronType electronType,
								 const Cuts& cuts,
								 const bool isolated )
{
  // this is the output list
  std::vector<const reco::SiStripElectron*> output_list;
   
  // Only TightGlobalElectrons are implemented for now
  switch (electronType) {
  case TightGlobalElectrons:
    {
      if (! data_.globalElectronCollection) {
	std::cout << "ERROR: global electron collection is not set" << std::endl;
	return output_list;
      }
      for ( std::vector<reco::SiStripElectron>::const_iterator electron = data_.globalElectronCollection->begin();
	    electron != data_.globalElectronCollection->end();
	    ++electron ) 
	{
	  // Here we make simple cuts in a standard way
	  if ( ! cuts.testCandidate(*electron) ) continue;
	  // get cluster energy
	   double energy = electron->superCluster()->energy();
	   std::cout << "Electron (p,E): " << electron->p() << ", " << energy <<std::endl;
	   if (electron->p() < 1 || energy/electron->p() <0.5) continue;

	   // truth cut
	   bool matched = false;
	   for(std::vector<HepMC::GenParticle>::const_iterator mc = data_.mcInfo->begin(); mc != data_.mcInfo->end(); ++mc)
	     if ((abs(mc->pdg_id())==11) && (mc->status() != 3) && 
		 fabs(electron->eta()-mc->momentum().eta())<0.1 &&
		 fabs(electron->phi()-mc->momentum().phi())<0.1 )
//		 && fabs((electron->pt()-mc->momentum().perp())/mc->momentum().perp()-1)<0.2)
	       matched = true;
	   if (matched) output_list.push_back(&*electron);
	}
      
      // At this point the output_list has been filled with electrons passing the simple cuts
      // In general, we will want to add more cuts.  This could go here.  It can be
      // done several ways, for example:
      //     Make another empty vector "final_output_list"
      //     loop over the vector "output_list" that we just made and add to final_output_list
      //     those electrons that pass more complicated cuts
      // For now, none of this is implemented    
      
    }
    break;
  case LooseGlobalElectrons:
    {
      if (! data_.globalElectronCollection) {
	std::cout << "ERROR: global electron collection is not set" << std::endl;
	return output_list;
      }
      for ( std::vector<reco::SiStripElectron>::const_iterator electron = data_.globalElectronCollection->begin();
	    electron != data_.globalElectronCollection->end();
	    ++electron ) 
	{
	  // Here we make simple cuts in a standard way
	  if ( ! cuts.testCandidate(*electron) ) continue;
	  // get cluster energy
	   double energy = electron->superCluster()->energy();
	   std::cout << "Electron (p,E): " << electron->p() << ", " << energy <<std::endl;
	   if (electron->p() < 1 || energy/electron->p() <0.5) continue;
	   // truth cut
	   bool matched = false;
	   for(std::vector<HepMC::GenParticle>::const_iterator mc = data_.mcInfo->begin(); mc != data_.mcInfo->end(); ++mc)
	     if ((abs(mc->pdg_id())==11) && (mc->status() != 3) && 
		 fabs(electron->eta()-mc->momentum().eta())<0.1 &&
		 fabs(electron->phi()-mc->momentum().phi())<0.1)
	       matched = true;
	   if (matched) output_list.push_back(&*electron);
	}
      
    }
    break;
    // You get here if you have requested a "electronType" that is not implemented
  default:
    std::cout << "Unkown or not implemented type" << std::endl;
  }
  return output_list;
}
