//
// Package:         CMS1/MCInfo
// Class:           MCInfo
// 
// Description:     info and matching for MC
//
// Original Author: Matteo Sani, matteo.sani@cern.ch
// Created:         Thu Mar 1 20:27:42 UTC 2007
//
// $Author: dmytro $
// $Date: 2007/06/14 05:59:58 $
// $Revision: 1.10 $
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
    std::cout << "ERROR: Monte Carlo black box doesn't know where to find EventData." << std::endl;
    return output_list;
  }
  
  if (data_->mcInfo.empty()) {
    std::cout << "Warning: generator particle list is empty. Not filled?" << std::endl;
    return output_list;
  }

  if (particleType == MCInfo::Summary) {
    for(std::vector<HepMC::GenParticle*>::const_iterator it=data_->mcInfo.begin(); it!=data_->mcInfo.end(); ++it) {
      if ((*it)->status() == 3)
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

math::XYZTLorentzVector cms1::MCInfo::getGenMET() 
{
  math::XYZTLorentzVector tempvect = math::XYZTLorentzVector(0,0,0,0);
  if(!data_) {
    std::cout << "ERROR: Monte Carlo black box doesn't know where to find EventData." << std::endl
	      << "Returning an empty Loretz Vector and therefore Gen MET = 0, and Gen METPHI=0" << std::endl;
    
    return tempvect;
  }
  
  if (data_->mcInfo.empty()) {
    std::cout << "Warning: generator particle list is empty. Not filled?" << std::endl
	      << "Returning an empty Loretz Vector and therefore Gen MET = 0, and Gen METPHI=0" << std::endl;;
    
    return tempvect;
  }
  
  for(std::vector<HepMC::GenParticle*>::const_iterator it=data_->mcInfo.begin(); it!=data_->mcInfo.end(); ++it) {
    int part_id = abs( (*it)->pdg_id() );
    //12 = nuE, 14=nuMu, 16=nuTau, 
    if( (*it)->status() != 3) {
       if( part_id == 12 || part_id == 14 || part_id == 16) {
	 tempvect = tempvect+math::XYZTLorentzVector( (*it)->momentum().x(),
						      (*it)->momentum().y(),
						      (*it)->momentum().z(),
						      (*it)->momentum().e() );
       }
    }
  }
  return tempvect;
  
}
  
   
   

void cms1::MCInfo::registerEventUserData()
{
   evtGenParticles.registerBlock( *data_, "genps_", "MC paticle block,");
   evtGenJets.registerBlock( *data_, "genjs_", "GenJet block,");
   data_->floatUserData.push_back( new UserData<float>("gen_met", "generated MET", false) );
   evtGenMET = data_->floatUserData.back();
   data_->floatUserData.push_back( new UserData<float>("gen_metPhi", "generated MET phi", false) );
   evtGenMETPhi = data_->floatUserData.back();
}

void cms1::MCInfo::fillEventUserData()
{
   std::vector<const HepMC::GenParticle*> summary = getMCInfo(Summary,Cuts());
   evtGenParticles.fill( summary );
   std::vector<const reco::GenJet*> sumJets = getJetInfo(Cuts());
   evtGenJets.fill( sumJets );
   math::XYZTLorentzVector met = getGenMET();
   evtGenMET->addData( met.Pt() );
   evtGenMETPhi->addData( std::atan2(met.Py(), met.Px()) );
}


