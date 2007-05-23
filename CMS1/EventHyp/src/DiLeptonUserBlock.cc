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
   jets.setCandidateFlag( true );
   otherJets.setCandidateFlag( true );
}

void cms1::DiLeptonUserBlock::registerBlock(EventData& event, const std::string& name_prefix, const std::string& alias_prefix)
{
   lTight.registerBlock( event, name_prefix+"lt_", alias_prefix );
   lLoose.registerBlock( event, name_prefix+"ll_", alias_prefix );
   jets.registerBlock(   event, name_prefix+"jets_", alias_prefix );
   otherJets.registerBlock(   event, name_prefix+"other_jets_", alias_prefix );
   
   addEntry(event.floatUserData, met,             "met",        name_prefix, alias_prefix);
   addEntry(event.floatUserData, mass,            "mass",       name_prefix, alias_prefix);
   addEntry(event.intUserData,   type,            "type",       name_prefix, alias_prefix);
   addEntry(event.intUserData,   nJets,           "jets_size",       name_prefix, alias_prefix);
   addEntry(event.intUserData,   nOtherJets,      "other_jets_size", name_prefix, alias_prefix);
}

void cms1::DiLeptonUserBlock::fill(EventData& event, const DiLeptonCandidate& candidate,
				   std::vector<const reco::Candidate*>& allJets)
{
   if ( ! usable() ) return;
   lTight.fill( getStreamerArguments(&event, candidate.lTight) );
   lLoose.fill( getStreamerArguments(&event, candidate.lLoose) );
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
   nJets->addData( candidate.jets.size() );
   nOtherJets->addData( notUsedJets.size() );
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
