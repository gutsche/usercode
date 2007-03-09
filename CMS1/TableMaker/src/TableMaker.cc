//
// Package:         CMS1/TableMaker
// Class:           TableMaker

// 
// Description:     EDAnalyzer filling table for di-lepton analysis dependent on number of jets
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Tue Feb 20 23:00:01 UTC 2007
//
// $Author: sani $
// $Date: 2007/03/06 13:17:25 $
// $Revision: 1.10 $
//

#include <vector>
#include <algorithm> 
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
#include <Math/VectorUtil.h>

//Number of bins in histograms
#define BINS 20

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

// correct the number of Jets according to electron selection
// this is a very simple algorithm which will be improved, 
// by the way the cone threshold can be passed as parameter to TableMaker.
unsigned int nJetsWithoutEl(std::vector<const reco::CaloJet*> jets, const SiStripElectron* e1=0, const SiStripElectron* e2=0) {

  unsigned int nJets = jets.size();

  for(std::vector<const reco::CaloJet*>::const_iterator itJet = jets.begin(); itJet != jets.end(); ++itJet) {

    if (e1 != 0) {
      double dR = ROOT::Math::VectorUtil::DeltaR(e1->p4(), (*itJet)->p4());
      if (dR < 0.4) {
        nJets--;
        continue;
      }
    }
  
    if (e2 != 0) {
      double dR = ROOT::Math::VectorUtil::DeltaR(e2->p4(), (*itJet)->p4());
      if (dR < 0.4)
        nJets--;
    } 
  }

  if (nJets > 4) 
    nJets = 4;

  return nJets;
} 

cms1::TableMaker::TableMaker()
{
	ostringstream temp; 
	hNJets = new TH1I("NJets","NJets;NJets;Events",5,0,5);
	for(int i=0;i<4;++i)
	{
		temp << "PTJet_" << i;
		hPTJet.push_back(new TH1F(temp.str().c_str(),"PTJet;PT(GeV);Events",BINS,0,500));
		temp.clear(); temp.str("");
	}
	for(int i=0;i<5;++i)
	{
		temp << "Mll_J" << i; 
		hMll.push_back(new TH1F(temp.str().c_str(),"Invariant Mass;Mll(GeV);Events",BINS,0,500));
		temp.clear(); temp.str("");

		temp << "PTTight_J" << i;
		hPTTight.push_back(new TH1F(temp.str().c_str(),"PT Tight Lepton;PT(GeV);Events",BINS,0,500));
		temp.clear(); temp.str("");

		temp << "PTLoose_J" << i;
		hPTLoose.push_back(new TH1F(temp.str().c_str(),"PT Loose Lepton;PT(GeV);Events",BINS,0,500));
		temp.clear(); temp.str("");

		temp << "PTLeading_J" << i;
		hPTLeading.push_back(new TH1F(temp.str().c_str(),"PT Leading Lepton;PT(GeV);Events",BINS,0,500));
		temp.clear(); temp.str("");

		temp << "PTTrailing_J" << i;
		hPTTrailing.push_back(new TH1F(temp.str().c_str(),"PT Second Leading Lepton;PT(GeV);Events",BINS,0,500));
		temp.clear(); temp.str("");

		temp << "HT_J" << i;
		hHT.push_back(new TH1F(temp.str().c_str(),"Temperature;Temperature(GeV);Events",BINS,0,1000));
		temp.clear(); temp.str("");

		temp << "MET_J" << i;
		hMET.push_back(new TH1F(temp.str().c_str(),"Missing ET;MET(GeV);Events",BINS,0,500));
		temp.clear(); temp.str("");

	}
	
}

cms1::TableMaker::~TableMaker()
{
	//delete  NJets;
	//delete  PTJetH;
	//delete  PTJetL;
	//for(int i=0;i<5;++i)
	//{
		//delete Mll[i];
		//delete PTLeading[i];
		//delete PTTrailing[i];
		//delete HT[i];
		//delete missET[i];
	//}
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
          countedEEJets_[nJetsWithoutEl(jets, *tightElectron,*looseElectron)]++;
		  FillHistograms(jets,nJetsWithoutEl(jets, *tightElectron,*looseElectron),*tightElectron,*looseElectron,*metVector.begin());
	      }
	    } else {
	      takenEE.push_back(std::make_pair(*tightElectron,*looseElectron));
        countedEEJets_[nJetsWithoutEl(jets, *tightElectron,*looseElectron)]++;
		FillHistograms(jets,nJetsWithoutEl(jets, *tightElectron,*looseElectron),*tightElectron,*looseElectron,*metVector.begin());
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
        countedEMuJets_[nJetsWithoutEl(jets, *tightElectron)]++;
		FillHistograms(jets,nJetsWithoutEl(jets, *tightElectron),*tightElectron,*looseMuon,*metVector.begin());
      }
    }  //End loop over loose muons
  }  //End tight electrons loop

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
        countedMuEJets_[nJetsWithoutEl(jets, *looseElectron)]++;
		FillHistograms(jets,nJetsWithoutEl(jets, *looseElectron),*tightMuon,*looseElectron,*metVector.begin());
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
    countedMuMuJets_[nJetsWithoutEl(jets)]++;
		FillHistograms(jets,nJetsWithoutEl(jets),*tightMuon,*looseMuon,*metVector.begin());
	      }
	    } else {
	      takenMuMu.push_back(std::make_pair(*tightMuon,*looseMuon));
        countedMuMuJets_[nJetsWithoutEl(jets)]++;
		FillHistograms(jets,nJetsWithoutEl(jets),*tightMuon,*looseMuon,*metVector.begin());
	    }
	  }
	}
      }
    }
  }
}


void 
cms1::TableMaker::beginJob()
{
  for(int i=0; i<5; ++i) {
    countedEEJets_[i] = 0;
    countedEMuJets_[i] = 0;
    countedMuEJets_[i] = 0;
    countedMuMuJets_[i] = 0;
  }

  events_ = 0;

}

void 
cms1::TableMaker::endJob() {
	
  ostringstream output;
  output << "pass0_" << fileTag << ".root";
	
	TFile file(output.str().c_str(),"RECREATE");
	hNJets->Write();
	//PTJetH->Write();
	//PTJetL->Write();
	for(int i=0;i<5;++i)
	{
		hMll[i]->Write();
		hPTLoose[i]->Write();
		hPTTight[i]->Write();
		hPTLeading[i]->Write();
		hPTTrailing[i]->Write();
		hHT[i]->Write();
		hMET[i]->Write();
		if(i<4)
		  {
		   hPTJet[i]->Write();
		  }
	}

	
	file.Close();

	output.clear();
	output.str("");

  std::cout << "Muons" << countedMuMuJets_[1] << ", "  << countedMuMuJets_[2] << ", "  << countedMuMuJets_[3] << ", "  << std::endl;


  output << "\n-------------------------------------------------------------------------\n";
  output <<   "| Events: " << setw(9) << events_ << "                                                     |\n";
  output << "-------------------------------------------------------------------------\n";
  output << "| Leptons   |    0 Jets |    1 Jet  |    2 Jets |    3 Jets | >= 4 Jets |\n";
  output << "-------------------------------------------------------------------------\n";
  output << "| EE        | " << std::setw(9) << countedEEJets_[0] << " | " 
	 << std::setw(9) << countedEEJets_[1] << " | " 
	 << std::setw(9) << countedEEJets_[2] << " | " 
	 << std::setw(9) << countedEEJets_[3] << " | " 
	 << std::setw(9) << countedEEJets_[4] << " |\n"; 
  output << "-------------------------------------------------------------------------\n";
  output << "| EMu       | " << std::setw(9) << countedEMuJets_[0] << " | " 
	 << std::setw(9) << countedEMuJets_[1] << " | " 
	 << std::setw(9) << countedEMuJets_[2] << " | " 
	 << std::setw(9) << countedEMuJets_[3] << " | " 
	 << std::setw(9) << countedEMuJets_[4] << " |\n" ;
  output << "-------------------------------------------------------------------------\n";
  output << "| MuE       | " << std::setw(9) << countedMuEJets_[0] << " | " 
	 << std::setw(9) << countedMuEJets_[1] << " | " 
	 << std::setw(9) << countedMuEJets_[2] << " | " 
	 << std::setw(9) << countedMuEJets_[3] << " | " 
	 << std::setw(9) << countedMuEJets_[4] << " |\n" ;
  output << "-------------------------------------------------------------------------\n";
  output << "| MuMu      | " << std::setw(9) << countedMuMuJets_[0] << " | " 
   << std::setw(9) << countedMuMuJets_[1] << " | " 
	 << std::setw(9) << countedMuMuJets_[2] << " | " 
	 << std::setw(9) << countedMuMuJets_[3] << " | " 
	 << std::setw(9) << countedMuMuJets_[4] << " |\n" ;
  output << "-------------------------------------------------------------------------\n";

   std::cout << output.str() << std::endl; 

}


//Function that will fill all of UCSD Grad Histograms
void cms1::TableMaker::FillHistograms(std::vector<const reco::CaloJet*> jets, unsigned int numJets,const RecoCandidate *one,const RecoCandidate *two, 
		const RecoCandidate *MET_)
{
	// Fill Number of Jets Hist
	hNJets->Fill(numJets);
	std::vector<double> jetpt;
	//PTJetH
	//PTJetL
	double energy=0;  // highest and second highest Jet PT
	
//	if(numJets>0) hPTJetH->Fill(high);
//	if(numJets>1) hPTJetL->
	for(int i = 0;i<jets.size();++i)
	  {
	    jetpt.push_back(jets[i]->pt());
		energy+=jets[i]->et();
	  }

	std::sort(jetpt.begin(),jetpt.end());

	for(int i = 0;i<numJets;++i)
	  {
	    hPTJet[i]->Fill(jetpt[i]);
	  }

	//Tight and loose
	hPTTight[numJets]->Fill(one->pt());
	hPTLoose[numJets]->Fill(two->pt());

	//PTLeading[i]
	//PTTrailing[i]
	if(one->pt() > two->pt())
	{
		hPTLeading[numJets]->Fill(one->pt());
		hPTTrailing[numJets]->Fill(two->pt());
	}
	else
	{
		hPTLeading[numJets]->Fill(two->pt());
		hPTTrailing[numJets]->Fill(one->pt());
	}
	
	//Mll[i]
	hMll[numJets]->Fill((one->p4()+two->p4()).M());
	
	//missET[i]
	hMET[numJets]->Fill(MET_->energy());
	
	//HT[i]
	hHT[numJets]->Fill(energy + MET_->energy() + one->et() + two->et());
}


