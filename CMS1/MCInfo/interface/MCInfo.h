#ifndef CMS1MCInfo_h
#define CMS1MCInfo_h

//
// Package:         CMS1/MCInfo
// Class:           MCInfo
// 
// Description:     MC info and matching function
//
// Original Author: Matteo Sani, matteo.sani@cern.ch
// Created:         Thu Mar 1 20:27:02 UTC 2007
//
// $Author: sani $
// $Revision: 1.4 $
//

#include "CMS1/Base/interface/BlackBox.h"
#include "CLHEP/HepMC/GenParticle.h"
#include "CMS1/Base/interface/Cuts.h"
#include "DataFormats/Candidate/interface/Candidate.h"

namespace cms1 {
  class MCInfo : public BlackBox {
  public:

    MCInfo():BlackBox() {}
    
    // These are "types of electrons" that we define.  We can add as amany as we want as
    // people invent new electron requirements
    enum ParticleType {Summary = -1, All=0, Electrons=11, Muons=13, Taus=15}; 
    
    void dumpLeptons(Cuts cut_el=Cuts(), Cuts cut_mu=Cuts(), Cuts cut_tau=Cuts());
    void dumpJets(Cuts cut_jet=Cuts());
 
    //const HepMC::GenParticle* match(const reco::Candidate& candidate, const ParticleType particleType, Cuts cuts=Cuts());
    // This is the function that does all the work
    std::vector<const HepMC::GenParticle*> getMCInfo (ParticleType=MCInfo::All, Cuts=Cuts());
    std::vector<const reco::GenJet*> getJetInfo (Cuts=Cuts());
 
    // a trivial function that uses getJetInfo to return the number of
    // jets in the event
    int numberOfJets(const Cuts& cuts) {
      return getJetInfo(cuts).size();
    }
     
    // a trivial function that uses getElectrons to return number of 
    // electrons of a given type in the event
    int numberOfParticles(const ParticleType type, const Cuts& cuts) {
      return getMCInfo(type,cuts).size();
    }
  };


}

#endif
