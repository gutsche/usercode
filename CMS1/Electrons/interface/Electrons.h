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
// $Author: sani $
// $Date: 2007/07/13 20:30:59 $
// $Revision: 1.20 $
//

#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"
//#include "CLHEP/HepMC/GenParticle.h"
#include "CMS1/Base/interface/Cuts.h"
#include "CMS1/Base/interface/BlackBox.h"
#include "CMS1/Base/interface/UserDataBlocks.h"
#include "CMS1/Base/interface/ElectronStreamer.h"

namespace cms1 {
  class Electrons: public BlackBox  {
  public:
    Electrons(): BlackBox() {}
    
    // These are "types of electrons" that we define. We can add as many as we want as
    // people invent new electron requirements
    enum ElectronType {AllElectrons, TightElectrons, LooseElectrons, TruthMatchedElectrons, Golden, BigBrem, Narrow, Showering};
    
    std::vector<const reco::Candidate*> getElectrons (const ElectronType, const Cuts&,
                                                      Cuts::IsolationType isolated = Cuts::NotIsolated);
    void removeElectrons(const std::vector<reco::PixelMatchGsfElectron>*);
    
    void dump(std::ostream& o, std::vector<const reco::Candidate*> el);
    void registerEventUserData();
    void fillEventUserData();

  private:
    VectorUserBlock<ElectronStreamer> evtElectrons;
    UserDataInt *nElectrons;
  };
}

#endif
