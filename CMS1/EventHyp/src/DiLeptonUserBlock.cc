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

void cms1::DiLeptonUserBlock::registerBlock(EventData& event, const std::string& name_prefix, const std::string& title_prefix)
{
   lTight.registerBlock(    event, name_prefix + "lt_",         title_prefix + " tight lepton," );
   lLoose.registerBlock(    event, name_prefix + "ll_",         title_prefix + " loose lepton," );
   jets.registerBlock(      event, name_prefix + "jets_",       title_prefix + " accepted jets," );
   otherJets.registerBlock( event, name_prefix + "other_jets_", title_prefix + " other jets," );
   
   addEntry(event.p4UserData,    p4Hyp,           name_prefix + "p4",             title_prefix + " p4");
   addEntry(event.floatUserData, met,             name_prefix + "met",            title_prefix + " MET with full muon correction using only hypothesis muons");
   addEntry(event.floatUserData, metPhi,          name_prefix + "metPhi",         title_prefix + " phi of the MET with full muon correction using only hypothesis muons");
   addEntry(event.floatUserData, metCaloExp,      name_prefix + "metCaloExp",     title_prefix + " MET with full muon correction and expected MIP energy in calorimeter");
   addEntry(event.floatUserData, metPhiCaloExp,   name_prefix + "metPhiCaloExp",  title_prefix + " phi of the MET with full muon correction and expected MIP energy in calorimeter");
   addEntry(event.floatUserData, metCone,         name_prefix + "metCone",        title_prefix + " MET with muon calo correction in a large area using only hypothesis muons");
   addEntry(event.floatUserData, metPhiCone,      name_prefix + "metPhiCone",     title_prefix + " MET phi with muon calo correction in a large area using only hypothesis muons");
   addEntry(event.floatUserData, metNoCalo,       name_prefix + "metNoCalo",      title_prefix + " MET corrected only for muon momentum using only hypothesis muons");
   addEntry(event.floatUserData, metPhiNoCalo,    name_prefix + "metPhiNoCalo",   title_prefix + " phi of MET corrected only for muon momentum using only hypothesis muons");
   addEntry(event.floatUserData, metAll,          name_prefix + "metAll",         title_prefix + " MET with full muon correction using all muons");
   addEntry(event.floatUserData, metPhiAll,       name_prefix + "metPhiAll",      title_prefix + " phi of the MET with full muon correction using all muons");
   addEntry(event.floatUserData, metAllCaloExp,   name_prefix + "metAllCaloExp",  title_prefix + " MET with full muon correction using all muons and expected MIP energy");
   addEntry(event.floatUserData, metPhiAllCaloExp,name_prefix+"metPhiAllCaloExp", title_prefix + " phi of the MET with full muon correction using all muons and expected MIP energy");
   addEntry(event.floatUserData, metJes5,         name_prefix + "metJes5",        title_prefix + " MET, muon + jet energy scale correction (jet pt>5) using only hypothesis muons");
   addEntry(event.floatUserData, metPhiJes5,      name_prefix + "metPhiJes5",     title_prefix + " MET phi, muon + jet energy scale correction (jet pt>5) using only hypothesis muons");
   addEntry(event.floatUserData, metJes10,        name_prefix + "metJes10",       title_prefix + " MET, muon + jet energy scale correction (jet pt>10) using only hypothesis muons");
   addEntry(event.floatUserData, metPhiJes10,     name_prefix + "metPhiJes10",    title_prefix + " MET phi, muon + jet energy scale correction (jet pt>10) using only hypothesis muons");
   addEntry(event.floatUserData, metJes15,        name_prefix + "metJes15",       title_prefix + " MET, muon + jet energy scale correction (jet pt>15) using only hypothesis muons");
   addEntry(event.floatUserData, metPhiJes15,     name_prefix + "metPhiJes15",    title_prefix + " MET phi, muon + jet energy scale correction (jet pt>15) using only hypothesis muons");
   addEntry(event.floatUserData, metJes30,        name_prefix + "metJes30",       title_prefix + " MET, muon + jet energy scale correction (jet pt>30) using only hypothesis muons");
   addEntry(event.floatUserData, metPhiJes30,     name_prefix + "metPhiJes30",    title_prefix + " MET phi, muon + jet energy scale correction (jet pt>30) using only hypothesis muons");
   addEntry(event.floatUserData, metJes50,        name_prefix + "metJes50",       title_prefix + " MET, muon + jet energy scale correction (jet pt>50) using only hypothesis muons");
   addEntry(event.floatUserData, metPhiJes50,     name_prefix + "metPhiJes50",    title_prefix + " MET phi, muon + jet energy scale correction (jet pt>50) using only hypothesis muons");
   // addEntry(event.floatUserData, metMuonCorr,     name_prefix + "metMuonCorr",    title_prefix + " ");
   // addEntry(event.floatUserData, metPhiMuonCorr,  name_prefix + "metPhiMuonCorr", title_prefix + " ");
   addEntry(event.floatUserData, metDPhiJet10,    name_prefix + "metDPhiJet10",   title_prefix + " MET dPhi wrt closest jet with pt>10 (muon corrected)");
   addEntry(event.floatUserData, metDPhiJet15,    name_prefix + "metDPhiJet15",   title_prefix + " MET dPhi wrt closest jet with pt>15 (muon corrected)");
   addEntry(event.floatUserData, metDPhiJet20,    name_prefix + "metDPhiJet20",   title_prefix + " MET dPhi wrt closest jet with pt>20 (muon corrected)");
   addEntry(event.floatUserData, metDPhiTrk10,    name_prefix + "metDPhiTrk10",   title_prefix + " MET dPhi wrt closest track with pt>10 (muon corrected)");
   addEntry(event.floatUserData, metDPhiTrk25,    name_prefix + "metDPhiTrk25",   title_prefix + " MET dPhi wrt closest track with pt>25 (muon corrected)");
   addEntry(event.floatUserData, metDPhiTrk50,    name_prefix + "metDPhiTrk50",   title_prefix + " MET dPhi wrt closest track with pt>50 (muon corrected)");
   addEntry(event.floatUserData, ltIso,           name_prefix + "lt_iso",         title_prefix + " tight lepton track isolation (sumPt)");
   addEntry(event.floatUserData, llIso,           name_prefix + "ll_iso",         title_prefix + " loose lepton track isolation (sumPt)");
   
   addEntry(event.intUserData,   type,            name_prefix + "type",       title_prefix + " dilepton type");
   addEntry(event.intUserData,   nJets,           name_prefix + "njets",      title_prefix + " number of accepted jets");
   addEntry(event.intUserData,   nOtherJets,      name_prefix + "nojets",     title_prefix + " number of other jets in the event");
   addEntry(event.intUserData,   ltIndex,         name_prefix + "lt_index",   title_prefix + " tight lepton index in the reference collection");
   addEntry(event.intUserData,   llIndex,         name_prefix + "ll_index",   title_prefix + " loose lepton index in the reference collection");
   addEntry(event.intUserData,   ltId,            name_prefix + "lt_id",      title_prefix + " tight lepton PDG id");
   addEntry(event.intUserData,   llId,            name_prefix + "ll_id",      title_prefix + " loose lepton PDG id");
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
   std::vector<const reco::Candidate*> hypMuons;
   if ( candidate.candidateType == DiLeptonCandidate::MuEl || candidate.candidateType == DiLeptonCandidate::MuMu ) hypMuons.push_back(candidate.lTight);
   if ( candidate.candidateType == DiLeptonCandidate::ElMu || candidate.candidateType == DiLeptonCandidate::MuMu ) hypMuons.push_back(candidate.lLoose);
   const std::vector<const reco::Candidate*>& metMuons = event.getBBCollection("MuonsForMETCorrection");
   
   // calculate full muon correction using all muons
     {
	double tmpMet = candidate.MET_uncorr;
	double tmpMetPhi = candidate.METphi_uncorr;
	MET::correctMETmuons( event, metMuons, tmpMet, tmpMetPhi);
	metAll->addData( tmpMet );
	metPhiAll->addData( tmpMetPhi );
     }
   
   // calculate full muon correction using all muons and expected MIP energy deposition
     {
	double tmpMet = candidate.MET_uncorr;
	double tmpMetPhi = candidate.METphi_uncorr;
	MET::correctMETmuons( event, metMuons, tmpMet, tmpMetPhi, MET::ExpectedMipEnergyCorrection);
	metAllCaloExp->addData( tmpMet );
	metPhiAllCaloExp->addData( tmpMetPhi );
     }
   
   // calculate full muon correction using only hypothesis muons and expected MIP energy deposition
     {
	double tmpMet = candidate.MET_uncorr;
	double tmpMetPhi = candidate.METphi_uncorr;
	MET::correctMETmuons( event, hypMuons, tmpMet, tmpMetPhi, MET::ExpectedMipEnergyCorrection);
	metCaloExp->addData( tmpMet );
	metPhiCaloExp->addData( tmpMetPhi );
     }
   
   // calculate simple muon correction (no calo MIP correction)
     {
	double tmpMet = candidate.MET_uncorr;
	double tmpMetPhi = candidate.METphi_uncorr;
	MET::correctMETmuons( event, hypMuons, tmpMet, tmpMetPhi, MET::NoCaloCorrection );
	metNoCalo->addData( tmpMet );
	metPhiNoCalo->addData( tmpMetPhi );
     }

      // calculate cone energy corrected MET
     {
	double tmpMet = candidate.MET_uncorr;
	double tmpMetPhi = candidate.METphi_uncorr;
	MET::correctMETmuons( event, hypMuons, tmpMet, tmpMetPhi, MET::S9EnergyCorrection );
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
      int index = -1;
      for (unsigned int i=0; i<refMuons.size(); ++i)
	if (candidate.lTight == refMuons[i]) {
	   index = i;
	   break;
	}
      ltIndex->addData( index );
   }else{
      int index = -1;
      for (unsigned int i=0; i<refElectrons.size(); ++i)
	if (candidate.lTight == refElectrons[i]) { 
	   index = i;
	   break;
	}
      ltIndex->addData( index );
   }
    if ( candidate.candidateType == DiLeptonCandidate::MuEl || candidate.candidateType == DiLeptonCandidate::ElEl ){
      int index = -1;
      for (unsigned int i=0; i<refElectrons.size(); ++i)
	if (candidate.lLoose == refElectrons[i]) { 
	   index = i;
	   break;
	}
       llIndex->addData( index );
   }else{
      int index = -1;
      for (unsigned int i=0; i<refMuons.size(); ++i)
	if (candidate.lLoose == refMuons[i]) { 
	   index = i;
	   break;
	}
      llIndex->addData( index );
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
	MET::correctedJetMET( event, jetsnoel, tmpMet, tmpMetPhi, 5 );
	metJes5->addData( tmpMet );
	metPhiJes5->addData( tmpMetPhi );
     }
     {
	double tmpMet = candidate.MET;
	double tmpMetPhi = candidate.METphi;
	MET::correctedJetMET( event, jetsnoel, tmpMet, tmpMetPhi, 10 );
	metJes10->addData( tmpMet );
	metPhiJes10->addData( tmpMetPhi );
     }
     {
	double tmpMet = candidate.MET;
	double tmpMetPhi = candidate.METphi;
	MET::correctedJetMET( event, jetsnoel, tmpMet, tmpMetPhi, 15 );
	metJes15->addData( tmpMet );
	metPhiJes15->addData( tmpMetPhi );
     }
     {
	double tmpMet = candidate.MET;
	double tmpMetPhi = candidate.METphi;
	MET::correctedJetMET( event, jetsnoel, tmpMet, tmpMetPhi, 30 );
	metJes30->addData( tmpMet );
	metPhiJes30->addData( tmpMetPhi );
     }
     {
	double tmpMet = candidate.MET;
	double tmpMetPhi = candidate.METphi;
	MET::correctedJetMET( event, jetsnoel, tmpMet, tmpMetPhi, 50 );
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
