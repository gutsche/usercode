//
// Package:         CMS1/Base
// Class:           Cuts
// 
// Description:     selectors for standard classes
//
// Original Author: Dmytro Kovalskyi
//
// $Author: dmytro $
// $Date: 2007/02/16 11:49:22 $
// $Revision: 1.1 $
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

