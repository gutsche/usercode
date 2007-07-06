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
// $Date: 2007/06/19 15:27:45 $
// $Revision: 1.16 $
//

#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"
#include "CLHEP/HepMC/GenParticle.h"

#include "CMS1/Base/interface/Cuts.h"
#include "CMS1/Base/interface/BlackBox.h"
#include "CMS1/Base/interface/UserDataBlocks.h"
#include "CMS1/Base/interface/TrackStreamer.h"

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
    enum ElectronType {AllElectrons, TightElectrons, LooseElectrons, TruthMatchedElectrons, Golden, BigBrem, Narrow, Showering, Custom};
    
    std::vector<const reco::Candidate*> getElectrons (const ElectronType, const Cuts&,
                                                      Cuts::IsolationType isolated = Cuts::NotIsolated,
                                                      ElectronDef def = ElectronDef());
    bool classify(ElectronDef def, const reco::PixelMatchGsfElectron* electron);
    void removeElectrons(const std::vector<reco::PixelMatchGsfElectron>*);
    
    void dump(std::ostream& o, std::vector<const reco::Candidate*> el);
    void registerEventUserData();
    void fillEventUserData();

  private:
    void R9_25(const reco::PixelMatchGsfElectron*, float&, float&, float&, float&, float&);
    bool identify(const reco::PixelMatchGsfElectron*, int);

    VectorUserBlock<TrackStreamer> evtElectrons;
    UserDataInt *nElectrons;

    UserDataInt1D *nSeed, *cms_class;
    UserDataFloat1D *hOverE, *fBrem, *dEtaIn, *dEtaOut, *dPhiIn, *dPhiOut, *varEsc, *varRawEsc, *vare3x3, *vare5x5;
    UserDataFloat1D *eOverPIn, *eOverPOut, *eSeed, *sPhiPhi, *sEtaEta, *tkIso;
  };
}

#endif
