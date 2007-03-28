/*
 *  DiLeptonCandidate.cc
 *  CMS1
 *
 *  Created by LATBauerdick on 3/22/07.
 *
 */

#include "CMS1/EventHyp/interface/DiLeptonCandidate.h"

cms1::DiLeptonCandidate* cms1::DiLeptonCandidate::returnDiLeptonCandidate(
	const reco::Candidate* lt, const reco::Candidate* ll,  std::vector<const reco::Candidate*> jets, double met, DiLeptonType t
	) {
	
	cms1::DiLeptonCandidate*
	dl=nextStore();
	dl->lTight = lt;
	dl->lLoose = ll;
	dl->jets = jets;
	dl->MET = met;
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
