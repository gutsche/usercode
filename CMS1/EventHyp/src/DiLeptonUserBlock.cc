#include "CMS1/EventHyp/interface/DiLeptonUserBlock.h"
#include "CMS1/Muons/interface/Muons.h"
#include "CMS1/Electrons/interface/Electrons.h"
#include "CMS1/Jets/interface/Jets.h"
#include "CMS1/MET/interface/MET.h"
#include "CMS1/Base/interface/UserDataBlocks.h"
#include "CMS1/Base/interface/TrackStreamer.h"
#include "CMS1/Base/interface/JetStreamer.h"
#include "CMS1/CommonTools/interface/UserDataTools.h"
#include "TrackingTools/TrackAssociator/interface/TrackDetectorAssociator.h"
#include "TrackingTools/TrackAssociator/interface/TrackDetMatchInfo.h"


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
   
   addEntry(event.p4UserData,    p4Hyp,           "hyp_p4",             name_prefix, alias_prefix);
   addEntry(event.floatUserData, met,             "hyp_met",            name_prefix, alias_prefix);
   addEntry(event.floatUserData, metPhi,          "hyp_metPhi",         name_prefix, alias_prefix);
   addEntry(event.floatUserData, metCone,         "hyp_metCone",        name_prefix, alias_prefix);
   addEntry(event.floatUserData, metPhiCone,      "hyp_metPhiCone",     name_prefix, alias_prefix);
   addEntry(event.floatUserData, metNoCalo,       "hyp_metNoCalo",      name_prefix, alias_prefix);
   addEntry(event.floatUserData, metPhiNoCalo,    "hyp_metPhiNoCalo",   name_prefix, alias_prefix);
   addEntry(event.floatUserData, metJes5,         "hyp_metJes5",        name_prefix, alias_prefix);
   addEntry(event.floatUserData, metPhiJes5,      "hyp_metPhiJes5",     name_prefix, alias_prefix);
   addEntry(event.floatUserData, metJes15,        "hyp_metJes15",       name_prefix, alias_prefix);
   addEntry(event.floatUserData, metPhiJes15,     "hyp_metPhiJes15",    name_prefix, alias_prefix);
   addEntry(event.floatUserData, metJes30,        "hyp_metJes30",       name_prefix, alias_prefix);
   addEntry(event.floatUserData, metPhiJes30,     "hyp_metPhiJes30",    name_prefix, alias_prefix);
   addEntry(event.floatUserData, metJes50,        "hyp_metJes50",       name_prefix, alias_prefix);
   addEntry(event.floatUserData, metPhiJes50,     "hyp_metPhiJes50",    name_prefix, alias_prefix);
   // addEntry(event.floatUserData, metMuonCorr,     "hyp_metMuonCorr",    name_prefix, alias_prefix);
   // addEntry(event.floatUserData, metPhiMuonCorr,  "hyp_metPhiMuonCorr", name_prefix, alias_prefix);
   addEntry(event.floatUserData, metDPhiJet10,    "hyp_metDPhiJet10",   name_prefix, alias_prefix);
   addEntry(event.floatUserData, metDPhiJet15,    "hyp_metDPhiJet15",   name_prefix, alias_prefix);
   addEntry(event.floatUserData, metDPhiJet20,    "hyp_metDPhiJet20",   name_prefix, alias_prefix);
   addEntry(event.floatUserData, metDPhiTrk10,    "hyp_metDPhiTrk10",   name_prefix, alias_prefix);
   addEntry(event.floatUserData, metDPhiTrk25,    "hyp_metDPhiTrk25",   name_prefix, alias_prefix);
   addEntry(event.floatUserData, metDPhiTrk50,    "hyp_metDPhiTrk50",   name_prefix, alias_prefix);
   addEntry(event.floatUserData, ltIso,           "hyp_lt_iso",         name_prefix, alias_prefix);
   addEntry(event.floatUserData, llIso,           "hyp_ll_iso",         name_prefix, alias_prefix);
   
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
   ltIso->addData( candidate.lTightIso );
   llIso->addData( candidate.lLooseIso );
   jets.fill( getStreamerArguments(&event, candidate.jets) );
   type->addData( candidate.candidateType );
   met->addData( candidate.MET );
   metPhi->addData( candidate.METphi );
   
   // calculate simple muon correction (no calo MIP correction)
     {
	double tmpMet = candidate.MET_uncorr;
	double tmpMetPhi = candidate.METphi_uncorr;
	MET::correctMETmuons( &event, tmpMet, tmpMetPhi, false );
	metNoCalo->addData( tmpMet );
	metPhiNoCalo->addData( tmpMetPhi );
     }

      // calculate cone energy corrected MET
     {
	double tmpMet = candidate.MET_uncorr;
	double tmpMetPhi = candidate.METphi_uncorr;
	MET::correctMETmuons( &event, tmpMet, tmpMetPhi, true, false );
	metCone->addData( tmpMet );
	metPhiCone->addData( tmpMetPhi );
     }

   // metMuonCorr->addData( candidate.MET_muon_corr );
   // metPhiMuonCorr->addData( candidate.METphi_muon_corr );

   p4Hyp->addData( candidate.lTight->p4()+candidate.lLoose->p4() );
   // fill a vector of jets that were not used
   std::vector<const reco::Candidate*> notUsedJets;
   const std::vector<const reco::Candidate*>& refJets     = event.getBBCollection("refJets");
   for (std::vector<const reco::Candidate*>::const_iterator itr = refJets.begin(); itr != refJets.end(); ++itr)
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
   const std::vector<const reco::Candidate*>& refMuons     = event.getBBCollection("refMuons");
   const std::vector<const reco::Candidate*>& refElectrons = event.getBBCollection("refElectrons");
   if ( candidate.candidateType == DiLeptonCandidate::MuMu || candidate.candidateType == DiLeptonCandidate::MuEl ){
      for (unsigned int i=0; i<refMuons.size(); ++i)
	if (candidate.lTight == refMuons[i]) { 
	   ltIndex->addData( i );
	   break;
	}
   }else{
      for (unsigned int i=0; i<refElectrons.size(); ++i)
	if (candidate.lTight == refElectrons[i]) { 
	   ltIndex->addData( i );
	   break;
	}
   }
    if ( candidate.candidateType == DiLeptonCandidate::MuEl || candidate.candidateType == DiLeptonCandidate::ElEl ){
      for (unsigned int i=0; i<refElectrons.size(); ++i)
	if (candidate.lLoose == refElectrons[i]) { 
	   llIndex->addData( i );
	   break;
	}
   }else{
      for (unsigned int i=0; i<refMuons.size(); ++i)
	if (candidate.lLoose == refMuons[i]) { 
	   llIndex->addData( i );
	   break;
	}
   }
   
   
   // jet scale energy correction for MET
   // get jets without electrons
   std::vector<const reco::Candidate*> electrons;
   if ( candidate.candidateType == DiLeptonCandidate::MuEl || 
	candidate.candidateType == DiLeptonCandidate::ElEl ) electrons.push_back(candidate.lLoose);
   if ( candidate.candidateType == DiLeptonCandidate::ElMu || 
	candidate.candidateType == DiLeptonCandidate::ElEl ) electrons.push_back(candidate.lTight);
   
   const std::vector<const reco::Candidate*>& allJets = event.getBBCollection("AllJets");
   std::vector<const reco::Candidate*> jetsnoel;
   for ( std::vector<const reco::Candidate*>::const_iterator jet = allJets.begin();
	 jet != allJets.end(); ++jet )
     if ( Cuts::testJetForElectrons(**jet, electrons ) ) jetsnoel.push_back(*jet);

     {
	double tmpMet = candidate.MET;
	double tmpMetPhi = candidate.METphi;
	MET::correctedJetMET( &event, &jetsnoel, tmpMet, tmpMetPhi, 5 );
	metJes5->addData( tmpMet );
	metPhiJes5->addData( tmpMetPhi );
     }
     {
	double tmpMet = candidate.MET;
	double tmpMetPhi = candidate.METphi;
	MET::correctedJetMET( &event, &jetsnoel, tmpMet, tmpMetPhi, 15 );
	metJes15->addData( tmpMet );
	metPhiJes15->addData( tmpMetPhi );
     }
     {
	double tmpMet = candidate.MET;
	double tmpMetPhi = candidate.METphi;
	MET::correctedJetMET( &event, &jetsnoel, tmpMet, tmpMetPhi, 30 );
	metJes30->addData( tmpMet );
	metPhiJes30->addData( tmpMetPhi );
     }
     {
	double tmpMet = candidate.MET;
	double tmpMetPhi = candidate.METphi;
	MET::correctedJetMET( &event, &jetsnoel, tmpMet, tmpMetPhi, 50 );
	metJes50->addData( tmpMet );
	metPhiJes50->addData( tmpMetPhi );
     }

   // MET dPhi wrt various things
   // compute dPhi with respect to jets
     {
	float minDPhi10 = 9999;
	float minDPhi15 = 9999;
	float minDPhi20 = 9999;
	float dPhi10 = 9999;
	float dPhi15 = 9999;
	float dPhi20 = 9999;
	for (std::vector<const reco::Candidate*>::const_iterator cand = refJets.begin();
	     cand != refJets.end(); ++cand)
	  {
	     // dphi units are not radians in the search for a minimum,
	     // but they are radians for the output
	     double dphi = fabs(cos((*cand)->phi()-candidate.METphi));
	     if( (*cand)->pt()>10 && minDPhi10 > dphi ) {
		minDPhi10 = dphi;
		dPhi10 = (*cand)->phi()-candidate.METphi;
	     }
	     if( (*cand)->pt()>15 && minDPhi15 > dphi ) {
		minDPhi15 = dphi;
		dPhi15 = (*cand)->phi()-candidate.METphi;
	     }
	     if( (*cand)->pt()>20 && minDPhi20 > dphi ) {
		minDPhi20 = dphi;
		dPhi20 = (*cand)->phi()-candidate.METphi;
	     }
	  }
	metDPhiJet10->addData( dPhi10 );
	metDPhiJet15->addData( dPhi15 );
	metDPhiJet20->addData( dPhi20 );
     }
   
   // compute dPhi with respect to tracks
     {
	float minDPhi10 = 9999;
	float minDPhi25 = 9999;
	float minDPhi50 = 9999;
	float dPhi10 = 9999;
	float dPhi25 = 9999;
	float dPhi50 = 9999;
	for (std::vector<const reco::Track*>::const_iterator cand = event.refTracks.begin();
	     cand != event.refTracks.end(); ++cand)
	  {
	     // dphi units are not radians in the search for a minimum,
	     // but they are radians for the output
	     double dphi = fabs(cos((*cand)->phi()-candidate.METphi));
	     if( (*cand)->pt()>10 && minDPhi10 > dphi ) {
		minDPhi10 = dphi;
		dPhi10 = (*cand)->phi()-candidate.METphi;
	     }
	     if( (*cand)->pt()>25 && minDPhi25 > dphi ) {
		minDPhi25 = dphi;
		dPhi25 = (*cand)->phi()-candidate.METphi;
	     }
	     if( (*cand)->pt()>50 && minDPhi50 > dphi ) {
		minDPhi50 = dphi;
		dPhi50 = (*cand)->phi()-candidate.METphi;
	     }
	  }
	metDPhiTrk10->addData( dPhi10 );
	metDPhiTrk25->addData( dPhi25 );
	metDPhiTrk50->addData( dPhi50 );
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
