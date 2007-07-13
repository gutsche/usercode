//
// Package:         CMS1/Base
// Class:           Cuts
// 
// Description:     selectors for standard classes
//
// Original Author: Dmytro Kovalskyi
//
// $Author: mangano $
// $Date: 2007/05/31 17:06:33 $
// $Revision: 1.10 $
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
      if ( trackRelIsolation(track.momentum(),track.vertex(), tracks ) > 5. ) return false;
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
      if ( trackRelIsolation(candidate.momentum(),candidate.vertex(), tracks ) > 5. ) return false;
   }
   
   return true;
}

bool cms1::Cuts::testGenParticle(const HepMC::GenParticle* particle) const {

   if (particle->momentum().perp() < pt_min || particle->momentum().perp() > pt_max) return false;
   if (particle->momentum().eta() < eta_min || particle->momentum().eta() > eta_max) return false;
   if (particle->momentum().phi() < phi_min || particle->momentum().phi() > phi_max) return false;

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
				     double vtxDiffDMax, double vtxDiffZMax, double ptMin, unsigned int nHits, bool debug)
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
			  <<", hHits: "<< iTk->found()
	       <<std::endl;
      if (dR < dRConeMin) continue;
      if ( dR < dRConeMax 
	   && dZ < vtxDiffZMax
	   && d0 < tkVtxDMax
	   && dD0 < vtxDiffDMax 
	   && iTk->pt() >= ptMin
	   && iTk->found() > nHits){
	sumPt += iTk->pt();
      }
    }

    isoResult = sumPt;
    if ( debug) std::cout << "isoResult: " << isoResult << std::endl;
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
   for(std::vector<HepMC::GenParticle*>::const_iterator genParticle = data_->mcInfo.begin(); 
       genParticle != data_->mcInfo.end(); ++genParticle)
     {
	math::XYZVector genP3((*genParticle)->momentum().x(),
			      (*genParticle)->momentum().y(),
			      (*genParticle)->momentum().z());
	//  && fabs((electron->pt()-genParticle->momentum().perp())/genParticle->momentum().perp()-1)<0.2)
	// matching cone radius can be different for different particle types
	switch ( truthMatchingType ) {
	 case Electron:
	   if ( abs( (*genParticle)->pdg_id() ) != 11 || (*genParticle)->status() == 3 ) continue;
	   if ( ROOT::Math::VectorUtil::DeltaR(testParticleP3, genP3) < 0.1 ) matched = true;
	   break;
	 case Muon:
	   if ( abs( (*genParticle)->pdg_id() ) != 13 || (*genParticle)->status() == 3 ) continue;
	   if ( ROOT::Math::VectorUtil::DeltaR(testParticleP3, genP3) < 0.1 ) matched = true;
	   break;
	 case Tau:
	   if ( abs( (*genParticle)->pdg_id() ) != 15 || (*genParticle)->status() == 3 ) continue;
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
     std::cout << "WARNING: Met cuts are not overlaping. No events will pass this cut." << std::endl;
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

/*
This function does not perform a logical AND, but instead replaces the previous cuts with the new cuts
Designed to allow cuts specified in the .cfg to supersede Black Box Defaults
This is still a rough draft, and relies heavily on shoddy code.  Should be improved.

Note: User-Defined cuts means the cuts passed to this object for purposes of this documentation

-mnorman

Functional Guide:

This function uses replacement cut values (passed in the form of a Cuts object and referred to as user-defined cuts), 
to replace the values currently
stored in this object.  The major weakness of this has to do with the fact that Cuts is intelligent enough to fill in 
default values, meaning that when the user Cuts object is defined, all possible cuts have default values.  To counter 
this, I have set the default values equal to Default_Parameter_Value (for maximums) and -1*Default_Parameter_Value for 
minimums, except in the case of pt where the minimum is zero.  Hence, for each possible cut, I check to see if the value 
passed by the user is within the boundaries established by Default_Parameter_Value.  If it is, I replace the previous 
value with the user defined value.  Otherwise, it is assumed the user made an error (Default_Parameter_Value is very 
far outside the physical range).  This, of course, depends on Default_Parameter_Value being functional, and is also a 
very non-object oriented way to program.

-mnorman

NOTE:
This function currently ALWAYS uses the user-defined isolation and truthMatching requirements

*/
void cms1::Cuts::REPLACE( const Cuts& cuts )
{

  //First check for overlapping cuts
  //Assume we haven't entered non-physical cuts.  Hence only check the user-defined cuts.
   if ( cuts.pt_min > cuts.pt_max ) 
     {
       std::cout << "WARNING: pt cuts are not overlaping. No events will pass this cut." << std::endl;
       std::cout << cuts.pt_min << " : " << cuts.pt_max << std::endl;
     }
   if ( cuts.eta_min > cuts.eta_max )
     std::cout << "WARNING: eta cuts are not overlaping. No events will pass this cut." << std::endl;
   if ( cuts.phi_min > cuts.phi_max ) 
     std::cout << "WARNING: phi cuts are not overlaping. No events will pass this cut." << std::endl;
   if ( cuts.et_min > cuts.et_max ) 
     std::cout << "WARNING: Et cuts are not overlaping. No events will pass this cut." << std::endl;
   if ( cuts.met_min > cuts.met_max )
     std::cout << "WARNING: Met cuts are not overlaping. No events will pass this cut." << std::endl;


   //Now prepare to assign values
   //First check and see if the value is the default value assigned by Cuts (if so ignore)
   //Then pass to the variable
   if (cuts.pt_min > 0.0) pt_min = cuts.pt_min;
   if (cuts.pt_max < Default_Parameter_Value) pt_max = cuts.pt_max;
   if (cuts.eta_min > -1*Default_Parameter_Value) eta_min = cuts.eta_min;
   if (cuts.eta_max < Default_Parameter_Value) eta_max = cuts.eta_max;
   if (cuts.phi_min > -1*Default_Parameter_Value) phi_min = cuts.phi_min;
   if (cuts.phi_max < Default_Parameter_Value) phi_max = cuts.phi_max;
   if (cuts.et_min > -1*Default_Parameter_Value) et_min = cuts.et_min;
   if (cuts.et_max < Default_Parameter_Value) et_max = cuts.et_max;
   if (cuts.met_min > -1*Default_Parameter_Value) met_min = cuts.met_min;
   if (cuts.met_max < Default_Parameter_Value) met_max = cuts.met_max;
   
   //Always pass the isolation cuts?
   isolated = cuts.isolated;


//    if (isolated != NotIsolated) {
//       if ( cuts.isolated != NotIsolated && isolated != cuts.isolated )
// 	{
// 	  std::cout << "WARNING: isolation requirements are not compatible for two set of cuts. Keep the new value." << std::endl;
// 	  isolated = cuts.isolated;
//    } 
//    else 
//      isolated = cuts.isolated;

   //Always pass the truthMatching?
   truthMatchingType = cuts.truthMatchingType;
   
//    if (truthMatchingType != NoMatching) {
//       if ( cuts.truthMatchingType != NoMatching && truthMatchingType != cuts.truthMatchingType )
// 	std::cout << "WARNING: truthMatchingType requirements are not compatible for two set of cuts. Keep the old value." << std::endl;
//    }
//    else
//      truthMatchingType = cuts.truthMatchingType;

   


} //END REPLACE function


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


//This is a simple print function created to print out the current cuts
//Unfortunately, I can't seem to find a good place to print them out that doesn't activate every run
//-mnorman
void cms1::Cuts::PrintCuts()
{
  std::cout << "Now Printing Cuts:" << std::endl;
  std::cout << "Pt_Min: " << pt_min << std::endl;
  std::cout << "Pt_Max: " << pt_max << std::endl;
  std::cout << "Et_Min: " << et_min << std::endl;
  std::cout << "Et_Min: " << et_max << std::endl;
  std::cout << "Eta_Min: " << eta_min << std::endl;
  std::cout << "Eta_Min: " << eta_max << std::endl;
  std::cout << "Phi_Min: " << phi_min << std::endl;
  std::cout << "Phi_Min: " << phi_max << std::endl;
  std::cout << "Met_Min: " << met_min << std::endl;
  std::cout << "Met_Min: " << met_max << std::endl;
}

