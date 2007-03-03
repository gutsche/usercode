//
// Package:         CMS1/TableMaker
// Class:           TableMaker

// 
// Description:     EDAnalyzer filling table for di-lepton analysis dependent on number of jets
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Tue Feb 20 23:00:01 UTC 2007
//
// $Author: dmytro $
// $Date: 2007/03/01 21:25:08 $
// $Revision: 1.5 $
//

#include <vector>
#include <cmath>
#include <utility>
#include <sstream>
#include <iomanip>

#include "CMS1/TableMaker/interface/TableMaker.h"
#include "CMS1/Base/interface/Dump.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/EgammaCandidates/interface/Electron.h"
#include "DataFormats/EgammaCandidates/interface/ElectronFwd.h"

#include "DataFormats/JetReco/interface/CaloJet.h"

#include "DataFormats/METReco/interface/CaloMETCollection.h"

#include "DataFormats/Math/interface/LorentzVector.h"

// correct MET energies for Muons
void correctMETmuons(std::vector<const reco::Muon*> *m, double& et, double& phi) {
//		std::cout << "We're correctiong METs for Muons here" << std::endl;
// ACHTUNG: we should also correct for the energy MIP

   	double metx =  et*std::cos(phi);
		double mety =  et*std::sin(phi);
		for ( std::vector<const reco::Muon*>::iterator i = m->begin(), ie = m->end();
			i != ie;
			++i ) {
				const reco::Muon* cp = *i;
				double pt0 = cp->pt(); 
				double phi0 = cp->phi(); 
				metx -= pt0*std::cos(phi0);
				mety -= pt0*std::sin(phi0);
		}
		et = std::sqrt(metx*metx+mety*mety);
		phi = std::atan2(mety, metx);
}

void
cms1::TableMaker::analyze()
{

  ++events_;
   
  // get vector of muons
  std::vector<const reco::Muon*> tightMuons = muons_.getMuons(Muons::TightGlobalMuons,tightMuon_);
  std::vector<const reco::Muon*> looseMuons = muons_.getMuons(Muons::LooseGlobalMuons,looseMuon_);
  std::vector<const reco::Muon*> allMuons = muons_.getMuons(Muons::AllGlobalMuons,allMuon_);

  // get vector of electrons
  std::vector<const reco::SiStripElectron*> tightElectrons = electrons_.getElectrons(Electrons::TightGlobalElectrons,tightElectron_);
  std::vector<const reco::SiStripElectron*> looseElectrons = electrons_.getElectrons(Electrons::LooseGlobalElectrons,looseElectron_);

  // get vector of jets
  std::vector<const reco::CaloJet*> jets = jets_.getJets(Jets::GlobalJets,jet_);

  // get vector of MET
  std::vector<const reco::CaloMET*> metVector = MET_.getMET(MET::GlobalMET,metCut_);

  //    edm::LogVerbatim("CMS1TableMaker") << tightMuons.size() << " tight global muon(s) found!";
  //    edm::LogVerbatim("CMS1TableMaker") << looseMuons.size() << " loose global muon(s) found!";
  //    edm::LogVerbatim("CMS1TableMaker") << tightElectrons.size() << " tight global electron(s) found!";
  //    edm::LogVerbatim("CMS1TableMaker") << looseElectrons.size() << " loose global electron(s) found!";
  //    edm::LogVerbatim("CMS1TableMaker") << jets.size() << " global jet(s) found!";
  //    edm::LogVerbatim("CMS1TableMaker") << metVector.size() << " global MET found!";


//	cms1::Dump::event();
//	cms1::Dump::candidates("AllGlobalMuons", allMuons );
//	cms1::Dump::candidates("LooseGlobalElectrons", looseElectrons );
//	cms1::Dump::candidates("GlobalJets", jets );

#ifndef NOTDEF

// Dump Event contents
	std::cout << "------------------------------------------------------------------------" << std::endl; 

  // get vector of MET without cut
  std::vector<const reco::CaloMET*> metVector0 = MET_.getMET(MET::GlobalMET,noCut_);
	double met = (*(metVector0.begin()))->pt();
	double mephi = (*(metVector0.begin()))->phi();

// ACHTUNG ACHTUNG, here we change met and mephi in situ!!
	std::cout << "MET = " << met << ", ME Phi = " << mephi << std::endl; 
	correctMETmuons(&allMuons, met, mephi);
	std::cout << "MET = " << met << ", ME Phi = " << mephi << std::endl; 

	std::cout << "Dump of Event, " 
		<< " Ne = " << looseElectrons.size() 
		<< " Nmu = " << allMuons.size() 
		<< " Nj = " << jets.size()		 
		<< " MET = " << metVector0.size()		 
	<< std::endl;	

  for ( std::vector<const reco::SiStripElectron*>::iterator i = looseElectrons.begin(), ie = looseElectrons.end();
		i != ie;
		++i ) {
			const reco::Candidate* cp = *i;
			std::cout 
				<< "Electron " 
			<< "Pt = " << cp->pt() 
			<< ", Eta = " << cp->eta() 
			<< ", Phi = " << cp->phi() 
			<< std::endl; 
	}
		
	for ( std::vector<const reco::Muon*>::iterator i = allMuons.begin(), ie = allMuons.end();
		i != ie;
		++i ) {
			const reco::Candidate* cp = *i;
			std::cout 
			<< "Muon "  
			<< "Pt = " << cp->pt() 
			<< ", Eta = " << cp->eta() 
			<< ", Phi = " << cp->phi() 
			<< std::endl; 
	}
	for ( std::vector<const reco::CaloJet*>::iterator i = jets.begin(), ie = jets.end();
		i != ie;
		++i ) {
			const reco::Candidate* cp = *i;
			std::cout 
					<< "Jet " 
					<< "Pt = " << cp->pt() 
					<< ", Eta = " << cp->eta() 
					<< ", Phi = " << cp->phi() 
					<< std::endl; 
	}
	for ( std::vector<const reco::CaloMET*>::iterator i = metVector0.begin(), ie = metVector0.end();
			i != ie;
			++i ) {
		const reco::Candidate* cp = *i;
		std::cout 
	   		<< "MET "  
				<< "Pt = " << cp->pt() 
				<< ", Eta = " << cp->eta() 
				<< ", Phi = " << cp->phi() 
				<< std::endl; 
	}

#endif

  // logic
  std::vector<std::pair<const reco::SiStripElectron*, const reco::SiStripElectron*> > takenEE;
  std::vector<std::pair<const reco::SiStripElectron*, const reco::Muon*> >     takenEMu;
  std::vector<std::pair<const reco::Muon*, const reco::SiStripElectron*> >     takenMuE;
  std::vector<std::pair<const reco::Muon*, const reco::Muon*> >         takenMuMu;


  // loop over tight electrons
  for ( std::vector<const reco::SiStripElectron*>::iterator tightElectron = tightElectrons.begin(),
	  electronEnd = tightElectrons.end();
	tightElectron != electronEnd;
	++tightElectron ) {
    // loop over loose electrons
    for ( std::vector<const reco::SiStripElectron*>::iterator looseElectron = looseElectrons.begin(),
	    electronEnd = looseElectrons.end();
	  looseElectron != electronEnd;
	  ++looseElectron ) {
      // check if the same electron has been selected
      if ( *tightElectron != *looseElectron ) {
	// check if pair already passed cuts
	bool passed = false;
	for ( std::vector<std::pair<const reco::SiStripElectron*, const reco::SiStripElectron*> >::iterator passedPair = takenEE.begin(),
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
	  if ( metVector.size() > 0 ) {
	    // calculate invariant mass and check for special MET cut within Z window
	    // take first MET cancicate
	    math::XYZTLorentzVector inv = (*tightElectron)->p4() + (*looseElectron)->p4();
	    if ( inv.mass() >= ZRangeMinMass_ && inv.mass() <= ZRangeMaxMass_ ) {
	      if ( metCutAroundZ_.testCandidate(**(metVector.begin())) ) {
		// passed
		takenEE.push_back(std::make_pair(*tightElectron,*looseElectron));
	      }
	    } else {
	      takenEE.push_back(std::make_pair(*tightElectron,*looseElectron));
	    }
	  }
	}
      }
    }
    // loop over loose muons
    for ( std::vector<const reco::Muon*>::iterator looseMuon = looseMuons.begin(),
	    MuonEnd = looseMuons.end();
	  looseMuon != MuonEnd;
	  ++looseMuon ) {
      // check if candidate passes MET cut
      if ( metVector.size() > 0 ) {
	takenEMu.push_back(std::make_pair(*tightElectron,*looseMuon));
      }
    }
  }

  // loop over tight muons
  for ( std::vector<const reco::Muon*>::iterator tightMuon = tightMuons.begin(),
	  muonEnd = tightMuons.end();
	tightMuon != muonEnd;
	++tightMuon ) {
    // loop over loose electrons
    for ( std::vector<const reco::SiStripElectron*>::iterator looseElectron = looseElectrons.begin(),
	    electronEnd = looseElectrons.end();
	  looseElectron != electronEnd;
	  ++looseElectron ) {
      // check if candidate passes MET cut
      if ( metVector.size() > 0 ) {
	takenMuE.push_back(std::make_pair(*tightMuon,*looseElectron));
      }
    }
    // loop over loose muons
    for ( std::vector<const reco::Muon*>::iterator looseMuon = looseMuons.begin(),
	    MuonEnd = looseMuons.end();
	  looseMuon != MuonEnd;
	  ++looseMuon ) {

      // check if the same muon has been selected
      if ( *tightMuon != *looseMuon ) {
	// check if pair already passed cuts
	bool passed = false;
	for ( std::vector<std::pair<const reco::Muon*, const reco::Muon*> >::iterator passedPair = takenMuMu.begin(),
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
	  if ( metVector.size() > 0 ) {
	    // calculate invariant mass and check for special MET cut within Z window
	    // take first MET cancicate
	    math::XYZTLorentzVector inv = (*tightMuon)->p4() + (*looseMuon)->p4();
	    if ( inv.mass() >= ZRangeMinMass_ && inv.mass() <= ZRangeMaxMass_ ) {
	      if ( metCutAroundZ_.testCandidate(**(metVector.begin())) ) {
		// passed
		takenMuMu.push_back(std::make_pair(*tightMuon,*looseMuon));
	      }
	    } else {
	      takenMuMu.push_back(std::make_pair(*tightMuon,*looseMuon));
	    }
	  }
	}
      }
    }
  }

  // fill counter
  if ( jets.size() == 0 ) {
    countedEE0Jets_   += takenEE.size();
    countedEMu0Jets_  += takenEMu.size();
    countedMuE0Jets_  += takenMuE.size();
    countedMuMu0Jets_ += takenMuMu.size();
  } else if ( jets.size() == 1 ) {
    countedEE1Jets_   += takenEE.size();
    countedEMu1Jets_  += takenEMu.size();
    countedMuE1Jets_  += takenMuE.size();
    countedMuMu1Jets_ += takenMuMu.size();
  } else if ( jets.size() == 2 ) {
    countedEE2Jets_   += takenEE.size();
    countedEMu2Jets_  += takenEMu.size();
    countedMuE2Jets_  += takenMuE.size();
    countedMuMu2Jets_ += takenMuMu.size();
  } else if ( jets.size() == 3 ) {
    countedEE3Jets_   += takenEE.size();
    countedEMu3Jets_  += takenEMu.size();
    countedMuE3Jets_  += takenMuE.size();
    countedMuMu3Jets_ += takenMuMu.size();
  } else if ( jets.size() >= 4 ) {
    countedEE4Jets_   += takenEE.size();
    countedEMu4Jets_  += takenEMu.size();
    countedMuE4Jets_  += takenMuE.size();
    countedMuMu4Jets_ += takenMuMu.size();
  }
     
}


void 
cms1::TableMaker::beginJob()
{

  countedEE0Jets_ = 0;
  countedEMu0Jets_ = 0;
  countedMuE0Jets_ = 0;
  countedMuMu0Jets_ = 0;
  countedEE1Jets_ = 0;
  countedEMu1Jets_ = 0;
  countedMuE1Jets_ = 0;
  countedMuMu1Jets_ = 0;
  countedEE2Jets_ = 0;
  countedEMu2Jets_ = 0;
  countedMuE2Jets_ = 0;
  countedMuMu2Jets_ = 0;
  countedEE3Jets_ = 0;
  countedEMu3Jets_ = 0;
  countedMuE3Jets_ = 0;
  countedMuMu3Jets_ = 0;
  countedEE4Jets_ = 0;
  countedEMu4Jets_ = 0;
  countedMuE4Jets_ = 0;
  countedMuMu4Jets_ = 0;

  events_ = 0;

}

void 
cms1::TableMaker::endJob() {

  std::cout << "Muons" << countedMuMu1Jets_ << ", "  << countedMuMu2Jets_ << ", "  << countedMuMu3Jets_ << ", "  << std::endl;

  ostringstream output;

  output << "\n-------------------------------------------------------------------------\n";
  output <<   "| Events: " << setw(9) << events_ << "                                                     |\n";
  output << "-------------------------------------------------------------------------\n";
  output << "| Leptons   |    0 Jets |    1 Jet  |    2 Jets |    3 Jets | >= 4 Jets |\n";
  output << "-------------------------------------------------------------------------\n";
  output << "| EE        | " << std::setw(9) << countedEE0Jets_ << " | " 
	 << std::setw(9) << countedEE1Jets_ << " | " 
	 << std::setw(9) << countedEE2Jets_ << " | " 
	 << std::setw(9) << countedEE3Jets_ << " | " 
	 << std::setw(9) << countedEE4Jets_ << " |\n"; 
  output << "-------------------------------------------------------------------------\n";
  output << "| EMu       | " << std::setw(9) << countedEMu0Jets_ << " | " 
	 << std::setw(9) << countedEMu1Jets_ << " | " 
	 << std::setw(9) << countedEMu2Jets_ << " | " 
	 << std::setw(9) << countedEMu3Jets_ << " | " 
	 << std::setw(9) << countedEMu4Jets_ << " |\n" ;
  output << "-------------------------------------------------------------------------\n";
  output << "| MuE       | " << std::setw(9) << countedMuE0Jets_ << " | " 
	 << std::setw(9) << countedMuE1Jets_ << " | " 
	 << std::setw(9) << countedMuE2Jets_ << " | " 
	 << std::setw(9) << countedMuE3Jets_ << " | " 
	 << std::setw(9) << countedMuE4Jets_ << " |\n" ;
  output << "-------------------------------------------------------------------------\n";
  output << "| MuMu      | " << std::setw(9) << countedMuMu0Jets_ << " | " 
			     << std::setw(9) << countedMuMu1Jets_ << " | " 
	 << std::setw(9) << countedMuMu2Jets_ << " | " 
	 << std::setw(9) << countedMuMu3Jets_ << " | " 
	 << std::setw(9) << countedMuMu4Jets_ << " |\n" ;
  output << "-------------------------------------------------------------------------\n";

   std::cout << output.str() << std::endl; 

}
