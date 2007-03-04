//
// Package:         CMS1/Base
// Class:           Cuts
// 
// Description:     selectors for standard classes
//
// Original Author: Dmytro Kovalskyi
//
// $Author: gutsche $
// $Date: 2007/02/22 23:10:50 $
// $Revision: 1.2 $
//

#include "CMS1/Base/interface/Cuts.h"

bool cms1::Cuts::testTrack(const reco::Track& track) const
{
   if (track.pt() < pt_min || track.pt() > pt_max) return false;
   if (track.eta() < eta_min || track.eta() > eta_max) return false;
   if (track.phi() < phi_min || track.phi() > phi_max) return false;
   
   return true;
}

bool cms1::Cuts::testCandidate(const reco::Candidate& candidate) const
{
   if (candidate.pt() < pt_min || candidate.pt() > pt_max) return false;
   if (candidate.eta() < eta_min || candidate.eta() > eta_max) return false;
   if (candidate.phi() < phi_min || candidate.phi() > phi_max) return false;
   
   return true;
}

bool cms1::Cuts::testGenParticle(const HepMC::GenParticle& particle) const {

   if (particle.momentum().perp() < pt_min || particle.momentum().perp() > pt_max) return false;
   if (particle.momentum().eta() < eta_min || particle.momentum().eta() > eta_max) return false;
   if (particle.momentum().phi() < phi_min || particle.momentum().phi() > phi_max) return false;

   return true;
}

bool cms1::Cuts::testGenJet(const reco::GenJet& jet) const {

   if (jet.pt() < pt_min || jet.pt() > pt_max) return false;
   if (jet.eta() < eta_min || jet.eta() > eta_max) return false;
   if (jet.phi() < phi_min || jet.phi() > phi_max) return false;
   if (jet.et() < et_min || jet.et() > et_max) return false;

   return true;
}

