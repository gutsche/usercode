/*
 *  EventHyp.cpp
 *  CMS1
 *
 *  Description:     EventHyp Black Box to return a list of Event Hypotheses
 *  Created by LATBauerdick on 3/22/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */
#include <iostream>

#include "CMS1/Base/interface/Cuts.h"
#include "CMS1/EventHyp/interface/EventHyp.h"




std::vector<const cms1::DiLeptonCandidate*> cms1::EventHyp::getEventHyp (
		std::vector<const reco::Candidate*> tightElectrons,
		std::vector<const reco::Candidate*> looseElectrons,
		std::vector<const reco::Candidate*> tightMuons,
		std::vector<const reco::Candidate*> looseMuons,
		std::vector<const reco::Candidate*> jets,
		double met,
		double metPhi,
		Cuts metCut,
		Cuts metCutAroundZ		
	) {


/////////////////////////////////////////////////////////
	double ZRangeMinMass_ = 80.;
	double ZRangeMaxMass_ = 100.;
	double isoCut_ = 0.1;
	candidateStore.clear(); // resets all previously allocated DiLeptonCandidates
////////////////////////////////////////////////////////

   const  std::vector<reco::Track>* tracks = data_->getData< std::vector<reco::Track> >("ctfWithMaterialTracks");
	
// logic
	std::vector<std::pair<const reco::Candidate*, const reco::Candidate*> > takenEE;
	std::vector<std::pair<const reco::Candidate*, const reco::Candidate*> > takenEMu;
	std::vector<std::pair<const reco::Candidate*, const reco::Candidate*> > takenMuE;
	std::vector<std::pair<const reco::Candidate*, const reco::Candidate*> > takenMuMu;


// loop over tight electrons
	for ( std::vector<const reco::Candidate*>::iterator tightElectron = tightElectrons.begin(),
		electronEnd = tightElectrons.end();
	tightElectron != electronEnd;
	++tightElectron ) {
		double isoRelTight = cms1::Cuts::trackRelIsolation((*tightElectron)->momentum(), (*tightElectron)->vertex(), tracks,
			0.3, 0.01, 0.1, 0.1, 0.2, 1.5);
		if (isoRelTight > isoCut_) continue;

			// loop over loose electrons
		for ( std::vector<const reco::Candidate*>::iterator looseElectron = looseElectrons.begin(),
			electronEnd = looseElectrons.end();
		looseElectron != electronEnd;
		++looseElectron ) {
			double isoRelLoose = cms1::Cuts::trackRelIsolation((*looseElectron)->momentum(), (*looseElectron)->vertex(), tracks,
				0.3, 0.01, 0.1, 0.1, 0.2, 1.5);
			if (isoRelLoose > isoCut_) continue;

				// check if the same electron has been selected
			if ( *tightElectron != *looseElectron ) {
			// check if pair already passed cuts
				bool passed = false;
				for ( std::vector<std::pair<const reco::Candidate*, const reco::Candidate*> >::iterator passedPair = takenEE.begin(),
					passedPairEnd = takenEE.end();
				passedPair != passedPairEnd;
				++passedPair ) {
					if ( (passedPair->second == *tightElectron) &&
					(passedPair->first == *looseElectron) ) {
						passed = true;
						break;
					}
				}
			// continue if pair hasn't passed cuts yet
				if ( !passed ) {
			// check if candidate passes MET cut
					if ( met > metCut.pt_min) {
				// calculate invariant mass and check for special MET cut within Z window
				// take first MET cancicate
						math::XYZTLorentzVector inv = (*tightElectron)->p4() + (*looseElectron)->p4();
						if ( inv.mass() >= ZRangeMinMass_ && inv.mass() <= ZRangeMaxMass_ ) {
					// passed
							if ( met > metCutAroundZ.pt_min) {
								takenEE.push_back(std::make_pair(*tightElectron,*looseElectron));
								std::vector<const reco::Candidate*> el;
								el.push_back(*tightElectron);
								el.push_back(*looseElectron);
								std::vector<const reco::Candidate*> jetsnoel;
								for ( std::vector<const reco::Candidate*>::const_iterator jet = jets.begin(); jet != jets.end(); ++ jet )
									if ( Cuts::testJetForElectrons(**jet, el ) ) jetsnoel.push_back(*jet);
									
// do it: jetsnoel, tightElectron, looseElectron, met
							   candidateStore.push_back(DiLeptonCandidate(data_, *tightElectron, *looseElectron, jetsnoel, met, metPhi, cms1::DiLeptonCandidate::ElEl));
							}
						} else {
							takenEE.push_back(std::make_pair(*tightElectron,*looseElectron));

							std::vector<const reco::Candidate*> el;
							el.push_back(*tightElectron);
							el.push_back(*looseElectron);
							std::vector<const reco::Candidate*> jetsnoel;
							for ( std::vector<const reco::Candidate*>::const_iterator jet = jets.begin(); jet != jets.end(); ++ jet )
								if ( Cuts::testJetForElectrons(**jet, el ) ) jetsnoel.push_back(*jet);
																	
// do it: jetsnoel, tightElectron, looseElectron, met
						   candidateStore.push_back(DiLeptonCandidate(data_, *tightElectron, *looseElectron, jetsnoel, met, metPhi, cms1::DiLeptonCandidate::ElEl));
						}
					}
				}
			}
		}
			// loop over loose muons
		for ( std::vector<const reco::Candidate*>::iterator looseMuon = looseMuons.begin(),
			MuonEnd = looseMuons.end();
		looseMuon != MuonEnd;
		++looseMuon ) {
			double isoRelLoose = cms1::Cuts::trackRelIsolation((*looseMuon)->momentum(), (*looseMuon)->vertex(), tracks,
				0.3, 0.01, 0.1, 0.1, 0.2, 1.5);
			if (isoRelLoose > isoCut_) continue;
				// check if candidate passes MET cut
			if ( met > metCut.pt_min) {
				takenEMu.push_back(std::make_pair(*tightElectron,*looseMuon));

				std::vector<const reco::Candidate*> el;
				el.push_back(*tightElectron);
				std::vector<const reco::Candidate*> jetsnoel;
				for ( std::vector<const reco::Candidate*>::const_iterator jet = jets.begin(); jet != jets.end(); ++ jet )
					if ( Cuts::testJetForElectrons(**jet, el ) ) jetsnoel.push_back(*jet);
					
// do it: jetsnoel, tightElectron, looseMuon, met
			   candidateStore.push_back(DiLeptonCandidate(data_, *tightElectron, *looseMuon, jetsnoel, met, metPhi, cms1::DiLeptonCandidate::ElMu));
			}
		}  //End loop over loose muons
	}  //End tight electrons loop

		// loop over tight muons
	for ( std::vector<const reco::Candidate*>::iterator tightMuon = tightMuons.begin(),
		muonEnd = tightMuons.end();
	tightMuon != muonEnd;
	++tightMuon ) {
		double isoRelTight = cms1::Cuts::trackRelIsolation((*tightMuon)->momentum(), (*tightMuon)->vertex(), tracks,
			0.3, 0.01, 0.1, 0.1, 0.2, 1.5);
		if (isoRelTight > isoCut_) continue;

			// loop over loose electrons
		for ( std::vector<const reco::Candidate*>::iterator looseElectron = looseElectrons.begin(),
			electronEnd = looseElectrons.end();
		looseElectron != electronEnd;
		++looseElectron ) {
			double isoRelLoose = cms1::Cuts::trackRelIsolation((*looseElectron)->momentum(), (*looseElectron)->vertex(), tracks,
				0.3, 0.01, 0.1, 0.1, 0.2, 1.5);
			if (isoRelLoose > isoCut_) continue;

				// exclude combinations from both tight electron and muon which have already been counted in EMu
			bool passed = false;
			for ( std::vector<std::pair<const reco::Candidate*, const reco::Candidate*> >::iterator passedPair = takenEMu.begin(),
				passedPairEnd = takenEMu.end();
			passedPair != passedPairEnd;
			++passedPair ) {
				if ( (passedPair->second == *tightMuon) &&
				(passedPair->first == *looseElectron) ) {
					passed = true;
					break;
				}
			}
				// continue if pair hasn't passed cuts yet
			if ( !passed ) {

				// check if candidate passes MET cut
				if ( met > metCut.pt_min) {
					takenMuE.push_back(std::make_pair(*tightMuon,*looseElectron));
					std::vector<const reco::Candidate*> el;
					el.push_back(*looseElectron);
					std::vector<const reco::Candidate*> jetsnoel;
					for ( std::vector<const reco::Candidate*>::const_iterator jet = jets.begin(); jet != jets.end(); ++ jet )
						if ( Cuts::testJetForElectrons(**jet, el ) ) jetsnoel.push_back(*jet);

																	
// do it: jetsnoel, tightMuon, looseElectron, met
				   candidateStore.push_back(DiLeptonCandidate(data_, *tightMuon, *looseElectron, jetsnoel, met, metPhi, cms1::DiLeptonCandidate::MuEl));
				}
			}
		}
			// loop over loose muons
		for ( std::vector<const reco::Candidate*>::iterator looseMuon = looseMuons.begin(),
			MuonEnd = looseMuons.end();
		looseMuon != MuonEnd;
		++looseMuon ) {

			double isoRelLoose = cms1::Cuts::trackRelIsolation((*looseMuon)->momentum(), (*looseMuon)->vertex(), tracks,
				0.3, 0.01, 0.1, 0.1, 0.2, 1.5);
			if (isoRelLoose > isoCut_) continue;
				// check if the same muon has been selected
			if ( *tightMuon != *looseMuon ) {
			// check if pair already passed cuts
				bool passed = false;
				for ( std::vector<std::pair<const reco::Candidate*, const reco::Candidate*> >::iterator passedPair = takenMuMu.begin(),
					passedPairEnd = takenMuMu.end();
				passedPair != passedPairEnd;
				++passedPair ) {
					if ( (passedPair->second == *tightMuon) &&
					(passedPair->first == *looseMuon) ) {
						passed = true;
						break;
					}
				}
			// continue if pair hasn't passed cuts yet
				if ( !passed ) {
			// check if candidate passes MET cut
					if ( met > metCut.pt_min) {
				// calculate invariant mass and check for special MET cut within Z window
				// take first MET cancicate
						math::XYZTLorentzVector inv = (*tightMuon)->p4() + (*looseMuon)->p4();
						if ( inv.mass() >= ZRangeMinMass_ && inv.mass() <= ZRangeMaxMass_ ) {
							if ( met > metCutAroundZ.pt_min) {
				// passed
								takenMuMu.push_back(std::make_pair(*tightMuon,*looseMuon));

// do it: jets, tightMuon, looseMuon, met
							   candidateStore.push_back(DiLeptonCandidate(data_, *tightMuon, *looseMuon, jets, met, metPhi, cms1::DiLeptonCandidate::MuMu));
							}
						} else {
							takenMuMu.push_back(std::make_pair(*tightMuon,*looseMuon));
																	
// do it: jets, tightMuon, looseMuon, met
						   candidateStore.push_back(DiLeptonCandidate(data_, *tightMuon, *looseMuon, jets, met, metPhi, cms1::DiLeptonCandidate::MuMu));
						}
					}
				}
			}
		}
	}
   std::vector<const cms1::DiLeptonCandidate*> output_list;
   for(std::vector<DiLeptonCandidate>::const_iterator cand = candidateStore.begin(); cand != candidateStore.end(); ++cand)
     output_list.push_back(&(*cand));
   return output_list;
}

void cms1::EventHyp::dump(std::ostream& o, std::vector<const cms1::DiLeptonCandidate*> cl) {
   const  std::vector<reco::Track>* tracks = data_->getData< std::vector<reco::Track> >("ctfWithMaterialTracks");
	for ( std::vector<const cms1::DiLeptonCandidate*>::iterator i = cl.begin(), ie = cl.end(); i != ie; ++i ) {
		const cms1::DiLeptonCandidate* dl = *i;
		const reco::Candidate* cp = dl->lTight; 
		o << "DiLepton Candidates: ";
		o << dl->candidateTypeString(); 
		int nj = dl->nJets();
		o << " + " << nj << " Jets ";  
		o << std::endl; 
// -- tight lepton
		o << "Tight " << dl->tightLeptonTypeString() << " "; 
		o << "Pt = " << cp->pt(); 
		o << ", Eta = " << cp->eta(); 
		o << ", Phi = " << cp->phi(); 
		if ( tracks != 0 ) {
			double isoRel = cms1::Cuts::trackRelIsolation(cp->momentum(), cp->vertex(), tracks, 0.3, 0.01, 0.1, 0.1, 0.2, 1.5);
			o << ", isol = " << isoRel;
		}
		o << std::endl; 
		
// -- loose lepton
		cp = dl->lLoose;
		o << "Loose " << dl->looseLeptonTypeString() << " "; 
		o << "Pt = " << cp->pt(); 
		o << ", Eta = " << cp->eta(); 
		o << ", Phi = " << cp->phi(); 
		if ( tracks != 0 ) {
			double isoRel = cms1::Cuts::trackRelIsolation(cp->momentum(), cp->vertex(), tracks, 0.3, 0.01, 0.1, 0.1, 0.2, 1.5);
			o << ", isol = " << isoRel;
		}
		o << std::endl; 

// -- Jets
		std::vector<const reco::Candidate*> jc = dl->jets;
		for ( std::vector<const reco::Candidate*>::iterator j = jc.begin(), je = jc.end(); j != je; ++j ) {
			const reco::Candidate* cp = *j;
			o << "Jet      "; 
			o << "Pt = " << cp->pt(); 
			o << ", Eta = " << cp->eta(); 
			o << ", Phi = " << cp->phi(); 
			o << std::endl; 
		}

// -- MET
		o << "MET      "; 
		o << "Et = " << dl->MET; 
		o << std::endl; 
		
	}
}
