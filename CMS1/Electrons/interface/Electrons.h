#ifndef CMS1Electrons_h
#define CMS1Electrons_h

//
// Package:         CMS1/Electrons
// Class:           Electrons
// 
// Description:     analysis functions for electrons
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Wed Feb 21 00:15:42 UTC 2007
//
// $Author: burkett $
// $Date: 2007/03/03 13:05:11 $
// $Revision: 1.2 $
//

#include "DataFormats/EgammaCandidates/interface/Electron.h"
#include "DataFormats/EgammaCandidates/interface/SiStripElectron.h"
#include "CLHEP/HepMC/GenParticle.h"

#include "CMS1/Base/interface/Cuts.h"

namespace cms1 {
  class Electrons  {
  public:
    struct ElectronData
    {
      ElectronData(): globalElectronCollection(0)
      {}
      
      const std::vector<reco::SiStripElectron>*              globalElectronCollection;
      std::vector<HepMC::GenParticle>* mcInfo;
    };
     
    // These are "types of electrons" that we define.  We can add as amany as we want as
    // people invent new electron requirements
    enum ElectronType { AllGlobalElectrons, TightGlobalElectrons, LooseGlobalElectrons };
     
    // We need to have pointers from the event to the collections that we might want to use.
    // This is done by setting the data_ private member at the beginning of the analysis of 
    // each event.  
    void setData( ElectronData& data ) { data_ = data; }  // sets data_
    const ElectronData& getData() const { return data_; } // gets data_ back
    ElectronData& getData() { return data_; }            // gaets data_ back
    
    // This is the function that does all the work
    std::vector<const reco::SiStripElectron*> getElectrons (const ElectronType, 
						     const Cuts&,
						     bool isolated = true);
      
    // a trivial function that uses getElectrons to return number of 
    // electrons of a given type in the event
    int numberOfElectrons(const ElectronType type, const Cuts& cuts) {
      return getElectrons(type,cuts).size();
    }
    
  private:
    ElectronData data_;
  };
}

#endif
