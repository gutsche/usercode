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
// $Date: 2007/04/18 17:20:30 $
// $Revision: 1.7 $
//

//#include "DataFormats/EgammaCandidates/interface/Electron.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"
#include "CLHEP/HepMC/GenParticle.h"

#include "CMS1/Base/interface/Cuts.h"
#include "CMS1/Base/interface/BlackBox.h"

namespace cms1 {
  struct ElectronDef {
    ElectronDef(int a=0, double b=0.1, double c=0.15, double d=0.2, double e=100., double f=100.) {
      numberOfSCSeed = a;
      eOverP = b;
      deltaPhi = c;
      fBrem = d;
      hOverE = e;
      R9 = f;
    }
    int numberOfSCSeed;
    double eOverP;
    double deltaPhi;
    double fBrem;
    double hOverE;
    double R9;
  };
  
   class Electrons: public BlackBox  {
  public:
     Electrons(): BlackBox() {}
      
    // These are "types of electrons" that we define. We can add as many as we want as
    // people invent new electron requirements
    enum ElectronType {TightElectrons, LooseElectrons, TruthMatchedElectrons, Golden, BigBrem, Narrow, Showering, Custom};
     
    // FIXME: return type corresponds to a single algoritm
    std::vector<const reco::Candidate*> getElectrons (const ElectronType, const Cuts&,
                                                      Cuts::IsolationType isolated = Cuts::NotIsolated,
                                                                  ElectronDef def = ElectronDef());
    bool classify(ElectronDef def, const reco::PixelMatchGsfElectron* electron);
    void dump(std::ostream& o, std::vector<const reco::Candidate*> el);
  };
}

#endif
