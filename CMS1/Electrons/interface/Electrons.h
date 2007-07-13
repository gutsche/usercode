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
// $Date: 2007/07/06 16:44:11 $
// $Revision: 1.18 $
//

#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"
#include "CLHEP/HepMC/GenParticle.h"

#include "CMS1/Base/interface/Cuts.h"
#include "CMS1/Base/interface/BlackBox.h"
#include "CMS1/Base/interface/UserDataBlocks.h"
#include "CMS1/Base/interface/TrackStreamer.h"

namespace cms1 {
  class Electrons: public BlackBox  {
  public:
    Electrons(): BlackBox() {}
    
    // These are "types of electrons" that we define. We can add as many as we want as
    // people invent new electron requirements
    enum ElectronType {AllElectrons, TightElectrons, LooseElectrons, TruthMatchedElectrons, Golden, BigBrem, Narrow, Showering};
    
    std::vector<const reco::Candidate*> getElectrons (const ElectronType, const Cuts&,
                                                      Cuts::IsolationType isolated = Cuts::NotIsolated);
    int classify(const reco::PixelMatchGsfElectron* electron);
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
