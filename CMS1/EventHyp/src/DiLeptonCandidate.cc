/*
 *  DiLeptonCandidate.cc
 *  CMS1
 *
 *  Created by LATBauerdick on 3/22/07.
 *
 */

#include "CMS1/EventHyp/interface/DiLeptonCandidate.h"
#include "CMS1/MET/interface/MET.h"

cms1::DiLeptonCandidate* cms1::DiLeptonCandidate::returnDiLeptonCandidate( EventData* event,
									   const reco::Candidate* lt, 
									   const reco::Candidate* ll,  
									   std::vector<const reco::Candidate*> jets, 
									   double met, double metPhi, DiLeptonType t )
{
   cms1::DiLeptonCandidate*  dl = nextStore();
   dl->lTight = lt;
   dl->lLoose = ll;
   dl->jets = jets;
   
   // assuming that we get uncorrected MET, let's make corrections
   
   // MET correction for muons ( full correction ) 
   MET::correctMETmuons( event, met, metPhi );
   
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
   
   dl->MET = met;
   dl->METphi = metPhi;
   
   dl->candidateType = t;
   return dl;
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

//////////////////////////////////////////
// ACHTUNG Baby, storage management for DiLeptonCandidates:
// need to call resetStore before each new event
// currently call this in EventHyp
/////////////////////////////////////////

int cms1::DiLeptonCandidate::iStore = -1;
std::vector<cms1::DiLeptonCandidate> cms1::DiLeptonCandidate::dlStore(4); 
void cms1::DiLeptonCandidate::resetStore() {
	iStore = -1; 
}

cms1::DiLeptonCandidate* cms1::DiLeptonCandidate::nextStore() {
	
	iStore++; 
	if (iStore > int(dlStore.size())-1) {
		std::cout << "Ooops: ran out of storage for DiLeptonCandidates, extending DiLeptonCandidate Store" << std::endl;
		dlStore.push_back(DiLeptonCandidate());
	}
	return &dlStore[iStore];
}
