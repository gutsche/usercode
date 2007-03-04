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
// $Revision: 1.1 $
//

#include "CLHEP/HepMC/GenParticle.h"
#include "CMS1/Base/interface/Cuts.h"

namespace cms1 {
  class MCInfo  {
  public:
    struct MCInfoData {
      MCInfoData(): mcInfo(0), jetInfo(0)
      {}
      
      const std::vector<HepMC::GenParticle>* mcInfo;
      const std::vector<reco::GenJet>* jetInfo;
    };
     
    // These are "types of electrons" that we define.  We can add as amany as we want as
    // people invent new electron requirements
    enum ParticleType {All=0, Electrons=11, Muons=13, Taus=15}; 
    
    void dumpLeptons(Cuts cut_el=Cuts(), Cuts cut_mu=Cuts(), Cuts cut_tau=Cuts());
    void dumpJets(Cuts cut_jet=Cuts());
 
    // We need to have pointers from the event to the collections that we might want to use.
    // This is done by setting the data_ private member at the beginning of the analysis of 
    // each event.  
    void setData(MCInfoData& data) { data_ = data; }  // sets data_
    const MCInfoData& getData() const { return data_; } // gets data_ back
    MCInfoData& getData() { return data_; }             // gets data_ back
    
    // This is the function that does all the work
    std::vector<const HepMC::GenParticle*> getMCInfo (const ParticleType, const Cuts&);
    std::vector<const reco::GenJet*> getJetInfo (const Cuts&);
 
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
    
  private:
    MCInfoData data_;
  };


}

#endif
