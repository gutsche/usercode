#include "CMS1/EventHyp/interface/DiLeptonUserBlock.h"
#include "CMS1/Muons/interface/Muons.h"
#include "CMS1/Electrons/interface/Electrons.h"
#include "CMS1/Jets/interface/Jets.h"
#include "CMS1/MET/interface/MET.h"
#include "CMS1/Base/interface/UserDataBlocks.h"
#include "CMS1/Base/interface/TrackStreamer.h"
#include "CMS1/Base/interface/JetStreamer.h"
#include "CMS1/CommonTools/interface/UserDataTools.h"

cms1::DiLeptonUserBlock::DiLeptonUserBlock(): met(0),mass(0),type(0)
{ 
   isCandidate_ = true;
   lTight.setCandidateFlag( true );
   lLoose.setCandidateFlag( true );
   elec1.setCandidateFlag( true );
   elec2.setCandidateFlag( true );
   muon1.setCandidateFlag( true );
   muon2.setCandidateFlag( true );
   jets.setCandidateFlag( true );
   otherJets.setCandidateFlag( true );
}

void cms1::DiLeptonUserBlock::registerBlock(EventData& event, const std::string& name_prefix, const std::string& alias_prefix)
{
   lTight.registerBlock( event, name_prefix+"_lt", alias_prefix );
   lLoose.registerBlock( event, name_prefix+"_ll", alias_prefix );
   elec1.registerBlock(  event, name_prefix+"_el1", alias_prefix );
   elec2.registerBlock(  event, name_prefix+"_el2", alias_prefix );
   muon1.registerBlock(  event, name_prefix+"_mu1", alias_prefix );
   muon2.registerBlock(  event, name_prefix+"_mu2", alias_prefix );
   jets.registerBlock(   event, name_prefix+"_jets", alias_prefix );
   otherJets.registerBlock(   event, name_prefix+"_other_jets", alias_prefix );
   
   addEntry(event.floatUserData, met,             "_met",        name_prefix, alias_prefix);
   addEntry(event.floatUserData, mass,            "_mass",       name_prefix, alias_prefix);
   addEntry(event.intUserData,   type,            "_type",       name_prefix, alias_prefix);
}

void cms1::DiLeptonUserBlock::fill(EventData& event, const DiLeptonCandidate& candidate,
				   std::vector<const reco::Candidate*>& allJets)
{
   if ( ! usable() ) return;
   const reco::Candidate* nothing = 0;
   lTight.fill( getStreamerArguments(&event, candidate.lTight) );
   lLoose.fill( getStreamerArguments(&event, candidate.lLoose) );
   switch (candidate.candidateType) {
    case DiLeptonCandidate::MuMu:
      muon1.fill( getStreamerArguments(&event, candidate.lTight) );
      muon2.fill( getStreamerArguments(&event, candidate.lLoose) );
      elec1.fill( nothing );
      elec2.fill( nothing );
      break;
    case DiLeptonCandidate::MuEl:
      muon1.fill( getStreamerArguments(&event, candidate.lTight) ); 
      elec2.fill( getStreamerArguments(&event, candidate.lLoose) ); 
      muon2.fill( nothing );
      elec1.fill( nothing );
      break;
    case DiLeptonCandidate::ElMu:
      elec1.fill( getStreamerArguments(&event, candidate.lTight) ); 
      muon2.fill( getStreamerArguments(&event, candidate.lLoose) ); 
      muon1.fill( nothing );
      elec2.fill( nothing );
      break;
    case DiLeptonCandidate::ElEl:
      elec1.fill( getStreamerArguments(&event, candidate.lTight) );
      elec2.fill( getStreamerArguments(&event, candidate.lLoose) );
      muon1.fill( nothing );
      muon2.fill( nothing );
      break;
   }
   jets.fill( getStreamerArguments(&event, candidate.jets) );
   type->addData( candidate.candidateType );
   met->addData( candidate.MET );
   mass->addData( (candidate.lTight->p4()+candidate.lLoose->p4()).M() );
   // fill a vector of jets that were not used
   std::vector<const reco::Candidate*> notUsedJets;
   for (std::vector<const reco::Candidate*>::const_iterator itr = allJets.begin(); itr != allJets.end(); ++itr)
     {
	bool notUsed = true;
	for(std::vector<const reco::Candidate*>::const_iterator itr2=candidate.jets.begin(); itr2 != candidate.jets.end(); ++itr2)
	  if ( (*itr)==(*itr2) ) notUsed = false;
	if ( notUsed ) notUsedJets.push_back(*itr);
     }
   otherJets.fill( getStreamerArguments(&event, notUsedJets) );
   
}

bool cms1::DiLeptonUserBlock::usable()
{
   if ( met==0||mass==0||type==0)
     {
	std::cout << "ERROR: attempt to fill data for non-registered JetUserBlock" << std::endl;
	return false;
     }
   return true;
}
