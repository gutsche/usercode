//
// Package:         CMS1/Base
// Class:           Cuts
// 
// Description:     selectors for standard classes
//
// Original Author: Dmytro Kovalskyi
//
// $Author: dmytro $
// $Date: 2007/04/12 19:32:27 $
// $Revision: 1.7 $
//

#include "CMS1/Base/interface/Cuts.h"
#include "CMS1/Base/interface/EventData.h"

#include "DataFormats/Math/interface/LorentzVector.h"
#include "Math/VectorUtil.h"
#include <algorithm>
#include <string>

bool cms1::Cuts::testTrack(const reco::Track& track) const
{
   if (track.pt() < pt_min || track.pt() > pt_max) return false;
   if (track.eta() < eta_min || track.eta() > eta_max) return false;
   if (track.phi() < phi_min || track.phi() > phi_max) return false;
   if (! truthMatch(track.momentum()) ) return false;
   if ( isolated != NotIsolated ) {
      if ( ! data_ ) {
	 std::cout << "Configuration Error: isolation requested, but event data is not provided" << std::endl;
	 return false;
      }
      const  std::vector<reco::Track>* tracks = data_->getData<std::vector<reco::Track> >("ctfWithMaterialTracks");
      if (! tracks) {
	 std::cout << "Configuration Error: isolation requested, but tracks not found in the event" << std::endl;
	 return false;
      }
      if ( trackRelIsolation(track.momentum(),track.vertex(), tracks ) > 0.1 ) return false;
   }
   
   return true;
}

bool cms1::Cuts::testCandidate(const reco::Candidate& candidate) const
{
   if (candidate.pt() < pt_min || candidate.pt() > pt_max) return false;
   if (candidate.eta() < eta_min || candidate.eta() > eta_max) return false;
   if (candidate.phi() < phi_min || candidate.phi() > phi_max) return false;
   if (! truthMatch(candidate.momentum()) ) return false;
   if ( isolated != NotIsolated ) {
      if ( ! data_ ) {
	 std::cout << "Configuration Error: isolation requested, but event data is not provided" << std::endl;
	 return false;
      }
      const  std::vector<reco::Track>* tracks = data_->getData<std::vector<reco::Track> >("ctfWithMaterialTracks");
      if (! tracks) {
	 std::cout << "Configuration Error: isolation requested, but tracks not found in the event" << std::endl;
	 return false;
      }
      if ( trackRelIsolation(candidate.momentum(),candidate.vertex(), tracks ) > 0.1 ) return false;
   }
   
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

double cms1::Cuts::trackRelIsolation(const math::XYZVector momentum, 
				     const math::XYZPoint vertex,
				     const  std::vector<reco::Track>* tracks, 
				     double dRConeMax, double dRConeMin, 
				     double tkVtxDMax,
				     double vtxDiffDMax, double vtxDiffZMax, double ptMin, bool debug)
{
    double isoResult = -10.;
    if ( tracks == 0 ) {
       std::cout << "Configuration Error: track collection is not set!" <<std::endl;
       return isoResult;
    }
    
    double sumPt = 0;

    std::vector<reco::Track>::const_iterator iTk = tracks->begin();
    for (; iTk != tracks->end(); ++iTk){
      double dR = ROOT::Math::VectorUtil::DeltaR(momentum, iTk->momentum());
      //exclude tks in veto cone (set it to small number to 
      //exclude this track
      double dZ = fabs(vertex.z() - iTk->vz());
      double d0 = sqrt(iTk->vertex().perp2());
      double dD0 = sqrt((iTk->vertex() - vertex).perp2());
      if (debug) std::cout<<"Track: "
	       <<" pt: "<< iTk->pt()
	       <<", dZ: "<<dZ
	       <<", d0: "<<d0
	       <<", dD0: "<< dD0
	       <<std::endl;
      if (dR < dRConeMin) continue;
      if ( dR < dRConeMax 
	   && dZ < vtxDiffZMax
	   && d0 < tkVtxDMax
	   && dD0 < vtxDiffDMax ){
	sumPt += iTk->pt();
      }
    }

    isoResult = sumPt/sqrt(momentum.perp2());
    return isoResult;

}

bool cms1::Cuts::truthMatch( const math::XYZVector& testParticleP3 ) const
{
   if (truthMatchingType == NoMatching) return true;
   bool matched = false;
   if ( ! data_ ) {
      std::cout << "Configuration Error: generator info is not available for truth matching" <<std::endl;
      return matched;
   }
   if ( data_->mcInfo.empty() ) {
      std::cout << "Warning: generator particle list is empty. Not filled?" <<std::endl;
      return matched;
   }
   for(std::vector<HepMC::GenParticle>::const_iterator genParticle = data_->mcInfo.begin(); 
       genParticle != data_->mcInfo.end(); ++genParticle)
     {
	math::XYZVector genP3(genParticle->momentum().x(),genParticle->momentum().y(),genParticle->momentum().z());
	//  && fabs((electron->pt()-genParticle->momentum().perp())/genParticle->momentum().perp()-1)<0.2)
	// matching cone radius can be different for different particle types
	switch ( truthMatchingType ) {
	 case Electron:
	   if ( abs( genParticle->pdg_id() ) != 11 || genParticle->status() == 3 ) continue;
	   if ( ROOT::Math::VectorUtil::DeltaR(testParticleP3, genP3) < 0.1 ) matched = true;
	   break;
	 case Muon:
	   if ( abs( genParticle->pdg_id() ) != 13 || genParticle->status() == 3 ) continue;
	   if ( ROOT::Math::VectorUtil::DeltaR(testParticleP3, genP3) < 0.1 ) matched = true;
	   break;
	 case Tau:
	   if ( abs( genParticle->pdg_id() ) != 15 || genParticle->status() == 3 ) continue;
	   if ( ROOT::Math::VectorUtil::DeltaR(testParticleP3, genP3) < 0.1 ) matched = true;
	   break;
	 default:
	   std::cout << "Unkown or not supported truth type. Ignored" << std::endl;
	   return true;
	}
     }
   return matched;
}

void cms1::Cuts::AND( const Cuts& cuts )
{
   if ( pt_min > cuts.pt_max || pt_max < cuts.pt_min ) 
     std::cout << "WARNING: pt cuts are not overlaping. No events will pass this cut." << std::endl;
   pt_min = std::max( pt_min, cuts.pt_min );
   pt_max = std::min( pt_max, cuts.pt_max );
   
   if ( eta_min > cuts.eta_max || eta_max < cuts.eta_min )
     std::cout << "WARNING: eta cuts are not overlaping. No events will pass this cut." << std::endl;
   eta_min = std::max( eta_min, cuts.eta_min );
   eta_max = std::min( eta_max, cuts.eta_max );
   
   if ( phi_min > cuts.phi_max || phi_max < cuts.phi_min ) 
     std::cout << "WARNING: phi cuts are not overlaping. No events will pass this cut." << std::endl;
   phi_min = std::max( phi_min, cuts.phi_min );
   phi_max = std::min( phi_max, cuts.phi_max );
   
   if ( et_min > cuts.et_max || et_max < cuts.et_min ) 
     std::cout << "WARNING: Et cuts are not overlaping. No events will pass this cut." << std::endl;
   et_min = std::max( et_min, cuts.et_min );
   et_max = std::min( et_max, cuts.et_max );
   
   if ( met_min > cuts.met_max || met_max < cuts.met_min ) 
     std::cout << "WARNING: Et cuts are not overlaping. No events will pass this cut." << std::endl;
   met_min = std::min( met_min, cuts.met_min );
   met_max = std::max( met_max, cuts.met_max );

   if (isolated != NotIsolated) {
      if ( cuts.isolated != NotIsolated && isolated != cuts.isolated )
	std::cout << "WARNING: isolation requirements are not compatible for two set of cuts. Keep the old value." << std::endl;
   } 
   else 
     isolated = cuts.isolated;
   
   if (truthMatchingType != NoMatching) {
      if ( cuts.truthMatchingType != NoMatching && truthMatchingType != cuts.truthMatchingType )
	std::cout << "WARNING: truthMatchingType requirements are not compatible for two set of cuts. Keep the old value." << std::endl;
   }
   else
     truthMatchingType = cuts.truthMatchingType;
}

bool cms1::Cuts::testJetForElectrons(const reco::Candidate& jet,
				     const std::vector<const reco::Candidate*>& electrons)
{
   bool matched = false;
   for ( std::vector<const reco::Candidate*>::const_iterator electron = electrons.begin(); 
	 electron != electrons.end(); ++electron) {
      double dR = ROOT::Math::VectorUtil::DeltaR( (*electron)->p4(), jet.p4() );
      if (dR < 0.4) matched = true;
   }
   return ! matched;
}

