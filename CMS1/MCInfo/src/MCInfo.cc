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
// $Date: 2007/03/01 20:27:51 $
// $Revision: 1.1 $
//

#include "CMS1/MCInfo/interface/MCInfo.h"

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

std::vector<const reco::GenJet*> cms1::MCInfo::getJetInfo(const Cuts& cuts) {
 
  // this is the output list
  std::vector<const reco::GenJet*> output_list;

  for(std::vector<reco::GenJet>::const_iterator it=data_.jetInfo->begin(); it!=data_.jetInfo->end(); ++it) 
    if (cuts.testGenJet(*it))
      output_list.push_back(&*it);

  return output_list;
}

std::vector<const HepMC::GenParticle*> cms1::MCInfo::getMCInfo(const ParticleType particleType, const Cuts& cuts) {
  
  // this is the output list
  std::vector<const HepMC::GenParticle*> output_list;
   
  for(std::vector<HepMC::GenParticle>::const_iterator it=data_.mcInfo->begin(); it!=data_.mcInfo->end(); ++it) {
    if ((abs(it->pdg_id()) == particleType) && (it->status() != 3))
      if (cuts.testGenParticle(*it))
	output_list.push_back(&*it); 
  }
  return output_list;
}


