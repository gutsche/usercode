//
// Package:         CMS1/MCInfo
// Class:           MCInfo
// 
// Description:     info and matching for MC
//
// Original Author: Matteo Sani, matteo.sani@cern.ch
// Created:         Thu Mar 1 20:27:42 UTC 2007
//
// $Author: sani $
// $Date: 2007/05/23 16:32:27 $
// $Revision: 1.6 $
//

#include "CMS1/MCInfo/interface/MCInfo.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include <Math/VectorUtil.h>

#include <iostream>

// write a simple dump of the MC event
void cms1::MCInfo::dumpLeptons(Cuts cut_el, Cuts cut_mu, Cuts cut_tau) {

  std::cout << "----------------------------------------------------------" << std::endl;
  std::cout << "                    MC Lepton Dump" << std::endl;

  std::vector<const HepMC::GenParticle*> electrons = getMCInfo(Electrons, cut_el);
  std::vector<const HepMC::GenParticle*> muons = getMCInfo(Muons, cut_mu);
  std::vector<const HepMC::GenParticle*> taus = getMCInfo(Taus, cut_tau);

  std::cout << "This event has: " << electrons.size() << " electrons, " << muons.size() << " muons, " << taus.size() << "taus." << std::endl;

  for(std::vector<const HepMC::GenParticle*>::const_iterator it=electrons.begin(); it!=electrons.end(); ++it)
    std::cout << "Electron, pT: " << (*it)->momentum().perp() << ", eta: " << (*it)->momentum().eta() << std::endl;

  for(std::vector<const HepMC::GenParticle*>::const_iterator it=muons.begin(); it!=muons.end(); ++it)
    std::cout << "Muon, pT: " << (*it)->momentum().perp() << ", eta: " << (*it)->momentum().eta() << std::endl;

  for(std::vector<const HepMC::GenParticle*>::const_iterator it=taus.begin(); it!=taus.end(); ++it)
    std::cout << "Tau, pT: " << (*it)->momentum().perp() << ", eta: " << (*it)->momentum().eta() << std::endl;

  std::cout << "----------------------------------------------------------" << std::endl;
}

void cms1::MCInfo::dumpJets(Cuts cut_jet) {

  std::cout << "----------------------------------------------------------" << std::endl;
  std::cout << "                    MC Jet Dump" << std::endl;
  std::vector<const reco::GenJet*> jets =  getJetInfo(cut_jet);

  std::cout << "This event has: " << jets.size() << " jets." << std::endl;

  for(std::vector<const reco::GenJet*>::const_iterator it=jets.begin(); it!=jets.end(); ++it)
    std::cout << "Jet, ET: " << (*it)->et() << ", eta: " << (*it)->eta() << std::endl;

  std::cout << "----------------------------------------------------------" << std::endl;
}

std::vector<const reco::GenJet*> cms1::MCInfo::getJetInfo(Cuts cuts) {
 
  // this is the output list
  std::vector<const reco::GenJet*> output_list;

  if (!data_) {
    std::cout << "ERROR: Monte Carlo black box doesn't know where to find EvenData." << std::endl;
    return output_list;
  }
  
  if (data_->jetInfo.empty()) {
    std::cout << "Warning: generator jet list is empty. Not filled?" << std::endl;
    return output_list;
  }

  for(std::vector<reco::GenJet>::const_iterator it=data_->jetInfo.begin(); it!=data_->jetInfo.end(); ++it) 
    if (cuts.testGenJet(*it))
      output_list.push_back(&*it);

  return output_list;
}

std::vector<const HepMC::GenParticle*> cms1::MCInfo::getMCInfo(ParticleType particleType, Cuts cuts) {

  // this is the output list
  std::vector<const HepMC::GenParticle*> output_list;

  if (!data_) {
    std::cout << "ERROR: Monte Carlo black box doesn't know where to find EvenData." << std::endl;
    return output_list;
  }
  
  if (data_->mcInfo.empty()) {
    std::cout << "Warning: generator particle list is empty. Not filled?" << std::endl;
    return output_list;
  }

  if (particleType == MCInfo::Summary) {
    for(std::vector<HepMC::GenParticle*>::const_iterator it=data_->mcInfo.begin(); it!=data_->mcInfo.end(); ++it) {
      if ((*it)->status() == 3)
        if (cuts.testGenParticle(*it))
          output_list.push_back(*it); 
    }
    return  output_list;
  }
  
  if (particleType == MCInfo::All) {
    for(std::vector<HepMC::GenParticle*>::const_iterator it=data_->mcInfo.begin(); it!=data_->mcInfo.end(); ++it) {
      if ((*it)->status() != 3)
        if (cuts.testGenParticle(*it))
          output_list.push_back(*it); 
    }
    return  output_list;
  }
   
   for(std::vector<HepMC::GenParticle*>::const_iterator it=data_->mcInfo.begin(); it!=data_->mcInfo.end(); ++it) {
      if ((abs((*it)->pdg_id()) == particleType) && ((*it)->status() != 3))
        if (cuts.testGenParticle(*it)) {
          output_list.push_back(*it); 
        }
    }
  return output_list;
}

/*
const HepMC::GenParticle* cms1::MCInfo::match(const reco::Candidate& candidate, const ParticleType particleType, Cuts cuts) {
  
  const HepMC::GenParticle* output = 0;
  double dRmin = 0.1;
  
  std::vector<const HepMC::GenParticle*> genParticles = getMCInfo(particleType, cuts);
  std::vector<const HepMC::GenParticle*>::const_iterator itPart;

  for(itPart=genParticles.begin(); itPart!=genParticles.end(); ++itPart) {

    const math::XYZVector v1((*itPart)->momentum().x(), (*itPart)->momentum().y(), (*itPart)->momentum().z());
    const math::XYZVector v2(candidate.px(), candidate.py(), candidate.pz());

    double dR = ROOT::Math::VectorUtil::DeltaR(v1, v2);
    if (dR < dRmin) {
      dRmin = dR;
      output = *itPart;
    }
  }

  return output;
}
*/
