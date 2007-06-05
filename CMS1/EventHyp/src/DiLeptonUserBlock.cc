#include "CMS1/EventHyp/interface/DiLeptonUserBlock.h"
#include "CMS1/Muons/interface/Muons.h"
#include "CMS1/Electrons/interface/Electrons.h"
#include "CMS1/Jets/interface/Jets.h"
#include "CMS1/MET/interface/MET.h"
#include "CMS1/Base/interface/UserDataBlocks.h"
#include "CMS1/Base/interface/TrackStreamer.h"
#include "CMS1/Base/interface/JetStreamer.h"
#include "CMS1/CommonTools/interface/UserDataTools.h"

cms1::DiLeptonUserBlock::DiLeptonUserBlock(): met(0),type(0)
{ 
   isCandidate_ = true;
   lTight.setCandidateFlag( true );
   lLoose.setCandidateFlag( true );
   jets.setCandidateFlag( true );
}

void cms1::DiLeptonUserBlock::registerBlock(EventData& event, const std::string& name_prefix, const std::string& alias_prefix)
{
   lTight.registerBlock(    event, name_prefix+"hyp_lt_", alias_prefix );
   lLoose.registerBlock(    event, name_prefix+"hyp_ll_", alias_prefix );
   jets.registerBlock(      event, name_prefix+"hyp_jets_", alias_prefix );
   otherJets.registerBlock( event, name_prefix+"hyp_other_jets_", alias_prefix );
   
   addEntry(event.p4UserData,    p4Hyp,           "hyp_p4",         name_prefix, alias_prefix);
   addEntry(event.floatUserData, met,             "hyp_met",        name_prefix, alias_prefix);
   addEntry(event.floatUserData, metPhi,          "hyp_metPhi",     name_prefix, alias_prefix);
   addEntry(event.intUserData,   type,            "hyp_type",       name_prefix, alias_prefix);
   addEntry(event.intUserData,   nJets,           "hyp_njets",      name_prefix, alias_prefix);
   addEntry(event.intUserData,   nOtherJets,      "hyp_nojets",     name_prefix, alias_prefix);
   addEntry(event.intUserData,   ltIndex,         "hyp_lt_index",   name_prefix, alias_prefix);
   addEntry(event.intUserData,   llIndex,         "hyp_ll_index",   name_prefix, alias_prefix);
   addEntry(event.intUserData,   ltId,            "hyp_lt_id",      name_prefix, alias_prefix);  // PDG id
   addEntry(event.intUserData,   llId,            "hyp_ll_id",      name_prefix, alias_prefix);  // PDG id
}

void cms1::DiLeptonUserBlock::fill(EventData& event, const DiLeptonCandidate& candidate)
{
   if ( ! usable() ) return;
   const reco::PixelMatchGsfElectron* e1=0;
   const reco::PixelMatchGsfElectron* e2=0;
   e1 = dynamic_cast<const reco::PixelMatchGsfElectron*>(candidate.lTight);
   e2 = dynamic_cast<const reco::PixelMatchGsfElectron*>(candidate.lLoose);

   lTight.fill( getStreamerArguments(&event, candidate.lTight) );
   lLoose.fill( getStreamerArguments(&event, candidate.lLoose) );
   jets.fill( getStreamerArguments(&event, candidate.jets) );
   type->addData( candidate.candidateType );
   met->addData( candidate.MET );
   metPhi->addData( candidate.METphi );
   p4Hyp->addData( candidate.lTight->p4()+candidate.lLoose->p4() );
   // fill a vector of jets that were not used
   std::vector<const reco::Candidate*> notUsedJets;
   for (std::vector<const reco::Candidate*>::const_iterator itr = event.refJets.begin(); itr != event.refJets.end(); ++itr)
     {
	bool notUsed = true;
	bool electron = true;
	std::vector<const reco::Candidate*> vect_elecs;
	if(e1 !=0 || e2 !=0 ) {
	  if( e1!=0 ) vect_elecs.push_back(e1);
	  if( e2!=0 ) vect_elecs.push_back(e2);
	  //testJetForElectrons returns false if the Jet contains an electron
	  electron = cms1::Cuts::testJetForElectrons(**itr, vect_elecs) ;
	}
	
	for(std::vector<const reco::Candidate*>::const_iterator itr2=candidate.jets.begin(); itr2 != candidate.jets.end(); ++itr2) {
	  if ( (*itr)==(*itr2) ) notUsed = false;
	}
	
	if ( notUsed && electron ) notUsedJets.push_back(*itr);
     }




   otherJets.fill( getStreamerArguments(&event, notUsedJets) );
   nJets->addData( candidate.jets.size() );
   nOtherJets->addData( notUsedJets.size() );
   ltId->addData( candidate.lTight->pdgId() );
   llId->addData( candidate.lLoose->pdgId() );
   // fill index
   if ( candidate.candidateType == DiLeptonCandidate::MuMu || candidate.candidateType == DiLeptonCandidate::MuEl ){
      for (unsigned int i=0; i<event.refMuons.size(); ++i)
	if (candidate.lTight == event.refMuons[i]) { 
	   ltIndex->addData( i );
	   break;
	}
   }else{
      for (unsigned int i=0; i<event.refElectrons.size(); ++i)
	if (candidate.lTight == event.refElectrons[i]) { 
	   ltIndex->addData( i );
	   break;
	}
   }
   if ( candidate.candidateType == DiLeptonCandidate::MuEl || candidate.candidateType == DiLeptonCandidate::ElEl ){
      for (unsigned int i=0; i<event.refElectrons.size(); ++i)
	if (candidate.lLoose == event.refElectrons[i]) { 
	   llIndex->addData( i );
	   break;
	}
   }else{
      for (unsigned int i=0; i<event.refMuons.size(); ++i)
	if (candidate.lLoose == event.refMuons[i]) { 
	   llIndex->addData( i );
	   break;
	}
   }
}

bool cms1::DiLeptonUserBlock::usable()
{
   if ( met==0||type==0)
     {
	std::cout << "ERROR: attempt to fill data for non-registered JetUserBlock" << std::endl;
	return false;
     }
   return true;
}
