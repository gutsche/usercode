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
// $Author: dmytro $
// $Date: 2007/03/16 07:15:17 $
// $Revision: 1.4 $
//

#include "DataFormats/EgammaCandidates/interface/Electron.h"
#include "DataFormats/EgammaCandidates/interface/SiStripElectron.h"
#include "CLHEP/HepMC/GenParticle.h"

#include "CMS1/Base/interface/Cuts.h"
#include "CMS1/Base/interface/BlackBox.h"

namespace cms1 {
   class Electrons: public BlackBox  {
  public:
     Electrons(): BlackBox(){}
      
    // These are "types of electrons" that we define.  We can add as many as we want as
    // people invent new electron requirements
    enum ElectronType { TightElectrons, LooseElectrons, TruthMatchedElectrons };
     
    // FIXME: return type corresponds to a single algoritm
    std::vector<const reco::Candidate*> getElectrons (const ElectronType, 
							    const Cuts&,
							    Cuts::IsolationType isolated = Cuts::NotIsolated);
	void dump(ostream& o, std::vector<const reco::Candidate*>);
  };
}

#endif
