/*
 *  DiLeptonCandidate.cc
 *  CMS1
 *
 *  Created by LATBauerdick on 3/22/07.
 *
 */

#include "CMS1/EventHyp/interface/DiLeptonCandidate.h"
#include "CMS1/MET/interface/MET.h"

cms1::DiLeptonCandidate::DiLeptonCandidate( EventData* event,
					    const reco::Candidate* lt, 
					    const reco::Candidate* ll,  
					    std::vector<const reco::Candidate*> iJets,
					    double met, double metPhi, DiLeptonType t )
{
   lTight = lt;
   lLoose = ll;
   jets = iJets;
   
   // assuming that we get uncorrected MET, let's make corrections
   
   MET_uncorr = met;
   METphi_uncorr = metPhi;
   
   // MET correction for muons ( full correction ) 
   MET::correctMETmuons( event, met, metPhi );
 /*  
   dl->MET_muon_corr = met;
   dl->METphi_muon_corr = metPhi;
   
   // jet scale energy correction
   // get jets without electrons
   std::vector<const reco::Candidate*> electrons;
   if ( t == MuEl || t == ElEl ) electrons.push_back(ll);
   if ( t == ElMu || t == ElEl ) electrons.push_back(lt);
   
   const std::vector<const reco::Candidate*>& allJets = event->getBBCollection("AllJets");
   std::vector<const reco::Candidate*> jetsnoel;
   for ( std::vector<const reco::Candidate*>::const_iterator jet = allJets.begin();
	 jet != allJets.end(); ++jet )
     if ( Cuts::testJetForElectrons(**jet, electrons ) ) jetsnoel.push_back(*jet);
   
   // do the correction
   MET::correctedJetMET( event, &jetsnoel, met, metPhi );
   */
   MET = met;
   METphi = metPhi;
   
   candidateType = t;
}

std::string cms1::DiLeptonCandidate::candidateTypeString() const {
	switch (candidateType) {
		case MuMu: return "MuMu";
		case MuEl: return "MuEl";
		case ElMu: return "ElMu";
		case ElEl: return "ElEl";
	}
	return "ERR!"; // this can never happen...
}

std::string cms1::DiLeptonCandidate::tightLeptonTypeString() const {
	switch (candidateType) {
		case MuMu: return "Mu";
		case MuEl: return "Mu";
		case ElMu: return "El";
		case ElEl: return "El";
	}
	return "ERR!"; // this can never happen...
}

std::string cms1::DiLeptonCandidate::looseLeptonTypeString() const {
	switch (candidateType) {
		case MuMu: return "Mu";
		case MuEl: return "El";
		case ElMu: return "Mu";
		case ElEl: return "El";
	}
	return "ERR!"; // this can never happen...
}

