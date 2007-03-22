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
// $Date: 2007/03/16 07:27:07 $
// $Revision: 1.17 $
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

#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "CMS1/TableMaker/src/UtilFunctions_isolation.h"


//Number of bins in histograms
#define BINS 20

// correct the number of Jets according to electron selection
// this is a very simple algorithm which will be improved, 
// by the way the cone threshold can be passed as parameter to TableMaker.
unsigned int nJetsWithoutEl(std::vector<const reco::Candidate*> jets, const Candidate* e1=0, const Candidate* e2=0) {

  unsigned int nJets = jets.size();

  for(std::vector<const reco::Candidate*>::const_iterator itJet = jets.begin(); itJet != jets.end(); ++itJet) {

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
   // associate black boxes with the EventData
   muons_.setEventData(&data_);
   electrons_.setEventData(&data_);
   jets_.setEventData(&data_);
   MET_.setEventData(&data_);

   
	ostringstream temp; 
	hTable = new TH2I("Table","Table;NJets;DiLepton",5,0,5,4,0,4);
	hNJets = new TH1I("NJets","NJets;NJets;Events",5,0,5);
	hNJets->Sumw2();
	for(int i=0;i<4;++i)
	{
		temp << "PTJet_" << i;
		TH1F* histo= new TH1F(temp.str().c_str(),"PTJet;PT(GeV);Events",BINS,0,500);
		histo->Sumw2();
		hPTJet.push_back(histo);
		temp.clear(); temp.str("");
	}
	for(int i=0;i<5;++i)
	{
		temp << "Mll_J" << i;
		TH1F* histo1=new TH1F(temp.str().c_str(),"Invariant Mass;Mll(GeV);Events",BINS,0,500);
		histo1->Sumw2();
		hMll.push_back(histo1);
		temp.clear(); temp.str("");

		temp << "PTTight_J" << i;
		TH1F* histo2=new TH1F(temp.str().c_str(),"PT Tight Lepton;PT(GeV);Events",BINS,0,500);
                histo2->Sumw2();
		hPTTight.push_back(histo2);
		temp.clear(); temp.str("");

		temp << "PTLoose_J" << i;
		TH1F* histo3=new TH1F(temp.str().c_str(),"PT Loose Lepton;PT(GeV);Events",BINS,0,500);
                histo3->Sumw2();
		hPTLoose.push_back(histo3);
		temp.clear(); temp.str("");

		temp << "PTLeading_J" << i;
		TH1F* histo4=new TH1F(temp.str().c_str(),"PT Leading Lepton;PT(GeV);Events",BINS,0,500);
                histo4->Sumw2();
		hPTLeading.push_back(histo4);
		temp.clear(); temp.str("");

		temp << "PTTrailing_J" << i;
		TH1F* histo5=new TH1F(temp.str().c_str(),"PT Second Leading Lepton;PT(GeV);Events",BINS,0,500);
		histo5->Sumw2();
		hPTTrailing.push_back(histo5);
		temp.clear(); temp.str("");

		temp << "HT_J" << i;
		TH1F* histo6=new TH1F(temp.str().c_str(),"Temperature;Temperature(GeV);Events",BINS,0,1000);
		histo6->Sumw2();
		hHT.push_back(histo6);
		temp.clear(); temp.str("");

		temp << "MET_J" << i;
		TH1F* histo7=new TH1F(temp.str().c_str(),"Missing ET;MET(GeV);Events",BINS,0,500); 
		histo7->Sumw2();
		hMET.push_back(histo7);
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
  std::vector<const reco::Candidate*> tightMuons = muons_.getMuons(Muons::TightGlobalMuons,tightMuon_);
  std::vector<const reco::Candidate*> looseMuons = muons_.getMuons(Muons::LooseGlobalMuons,looseMuon_);
  std::vector<const reco::Candidate*> allMuons = muons_.getMuons(Muons::AllGlobalMuons,allMuon_);

  // get vector of electrons
   tightElectron_.truthMatchingType = Cuts::Electron ; // require truth matching
   tightElectron_.setEventData(&data_);                // let the Cuts know where to get event info (mcInfo in this case)
   looseElectron_.truthMatchingType = Cuts::Electron;  // require truth matching
   looseElectron_.setEventData(&data_);                // let the Cuts know where to get event info (mcInfo in this case)
  std::vector<const reco::Candidate*> tightElectrons = electrons_.getElectrons(Electrons::TightElectrons,tightElectron_);
  std::vector<const reco::Candidate*> looseElectrons = electrons_.getElectrons(Electrons::LooseElectrons,looseElectron_);

  // get vector of jets

  std::vector<const reco::Candidate*> jets = jets_.getJets(Jets::DefaultJets,jet_);

// get MET without cut and correct
   const reco::Candidate* metObj = MET_.getMET(MET::DefaultMET);
   double met = metObj->pt();
   double mephi = metObj->phi();
// ACHTUNG, Baby -- here we change met and mephi in situ!!
// but metObj is not changed 
   MET::correctMETmuons(&allMuons, met, mephi);


#ifndef NOTDEF

// Dump Event contents

	std::cout << "------------------------------------------------------------------------" << std::endl; 
	std::cout << "Dump of Event, " 
		<< " Ne = " << looseElectrons.size() 
		<< " Nmu = " << allMuons.size() 
		<< " Nj = " << jets.size()		 
		<< " Nj (def.Jets) = " << ( jets_.getEventData()->defaultJets == 0 ? -1 :  int( jets_.getEventData()->defaultJets->size() ) )
	<< std::endl;	
	electrons_.dump(std::cout, looseElectrons);
 	muons_.dump(std::cout, allMuons);
 	jets_.dump(std::cout, jets);
 	MET_.dump(std::cout, metObj);
	std::cout << "------------------------------------------------------------------------" << std::endl; 

#endif

  // logic
  std::vector<std::pair<const reco::Candidate*, const reco::Candidate*> > takenEE;
  std::vector<std::pair<const reco::Candidate*, const reco::Candidate*> >     takenEMu;
  std::vector<std::pair<const reco::Candidate*, const reco::Candidate*> >     takenMuE;
  std::vector<std::pair<const reco::Candidate*, const reco::Candidate*> >         takenMuMu;


  // loop over tight electrons
  for ( std::vector<const reco::Candidate*>::iterator tightElectron = tightElectrons.begin(),
	  electronEnd = tightElectrons.end();
	tightElectron != electronEnd;
	++tightElectron ) {
    double isoRelTight = trackRelIsolation((*tightElectron)->momentum(), (*tightElectron)->vertex(), data_.tracks,
				      0.3, 0.01, 0.1, 0.1, 0.2, 1.5);
    if (isoRelTight > 0.1) continue;

    // loop over loose electrons
    for ( std::vector<const reco::Candidate*>::iterator looseElectron = looseElectrons.begin(),
	    electronEnd = looseElectrons.end();
	  looseElectron != electronEnd;
	  ++looseElectron ) {
      double isoRelLoose = trackRelIsolation((*looseElectron)->momentum(), (*looseElectron)->vertex(), data_.tracks,
					0.3, 0.01, 0.1, 0.1, 0.2, 1.5);
      if (isoRelLoose > 0.1) continue;

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
	  if ( met > metCut_.pt_min) {
	    // calculate invariant mass and check for special MET cut within Z window
	    // take first MET cancicate
	    math::XYZTLorentzVector inv = (*tightElectron)->p4() + (*looseElectron)->p4();
	    if ( inv.mass() >= ZRangeMinMass_ && inv.mass() <= ZRangeMaxMass_ ) {
          // passed
	      if ( met > metCutAroundZ_.pt_min) {
		  takenEE.push_back(std::make_pair(*tightElectron,*looseElectron));
		  std::cout<<"Found EE"<<std::endl;
		  std::vector<const reco::Candidate*> el;
		  el.push_back(*tightElectron);
		  el.push_back(*looseElectron);
		  std::vector<const reco::Candidate*> jets = jets_.getJets(Jets::DefaultJets,jet_);
		  std::vector<const reco::Candidate*> jetsnoel;
		 for ( std::vector<const reco::Candidate*>::const_iterator jet = jets.begin(); jet != jets.end(); ++ jet )
		   if ( Cuts::testJetForElectrons(**jet, el ) ) jetsnoel.push_back(*jet);
		  
		  int njet = jetsnoel.size();
		  if (njet > 4)
		    {
		      njet = 4;
		    }
		  countedEEJets_[njet]++;

		  FillHistograms(jetsnoel,*tightElectron,*looseElectron,met);
	      }
	    } else {
	      takenEE.push_back(std::make_pair(*tightElectron,*looseElectron));
	       
	       std::vector<const reco::Candidate*> el;
	       el.push_back(*tightElectron);
	       el.push_back(*looseElectron);
	       std::vector<const reco::Candidate*> jets = jets_.getJets(Jets::DefaultJets,jet_);
	       std::vector<const reco::Candidate*> jetsnoel;
	       for ( std::vector<const reco::Candidate*>::const_iterator jet = jets.begin(); jet != jets.end(); ++ jet )
		 if ( Cuts::testJetForElectrons(**jet, el ) ) jetsnoel.push_back(*jet);
	      
	      int njet = jetsnoel.size();
	      if (njet > 4)
		{
		  njet = 4;
		}
	      countedEEJets_[njet]++;
	      
	      FillHistograms(jetsnoel,*tightElectron,*looseElectron,met);

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
      double isoRelLoose = trackRelIsolation((*looseMuon)->momentum(), (*looseMuon)->vertex(), data_.tracks,
					0.3, 0.01, 0.1, 0.1, 0.2, 1.5);
      if (isoRelLoose > 0.1) continue;
      // check if candidate passes MET cut
	  if ( met > metCut_.pt_min) {
	    std::cout<<"Found EMu"<<std::endl;
        takenEMu.push_back(std::make_pair(*tightElectron,*looseMuon));
	     
	     std::vector<const reco::Candidate*> el;
	     el.push_back(*tightElectron);
	     std::vector<const reco::Candidate*> jets = jets_.getJets(Jets::DefaultJets,jet_);
	     std::vector<const reco::Candidate*> jetsnoel;
	     for ( std::vector<const reco::Candidate*>::const_iterator jet = jets.begin(); jet != jets.end(); ++ jet )
	       if ( Cuts::testJetForElectrons(**jet, el ) ) jetsnoel.push_back(*jet);
	     
	      int njet = jetsnoel.size();
	      if (njet > 4)
		{
		  njet = 4;
		}
	    std::cout<<"Found EMu"<<std::endl;
	      countedEMuJets_[njet]++;       
	      std::cout<<"Found EMu"<<jetsnoel.size()<<std::endl;
   
	FillHistograms(jetsnoel,*tightElectron,*looseMuon,met);
   std::cout<<"Found EMu"<<std::endl;      
	  }
    }  //End loop over loose muons
  }  //End tight electrons loop

  // loop over tight muons
  for ( std::vector<const reco::Candidate*>::iterator tightMuon = tightMuons.begin(),
	  muonEnd = tightMuons.end();
	tightMuon != muonEnd;
	++tightMuon ) {
      double isoRelTight = trackRelIsolation((*tightMuon)->momentum(), (*tightMuon)->vertex(), data_.tracks,
					0.3, 0.01, 0.1, 0.1, 0.2, 1.5);
      if (isoRelTight > 0.1) continue;

    // loop over loose electrons
    for ( std::vector<const reco::Candidate*>::iterator looseElectron = looseElectrons.begin(),
	    electronEnd = looseElectrons.end();
	  looseElectron != electronEnd;
	  ++looseElectron ) {
      double isoRelLoose = trackRelIsolation((*looseElectron)->momentum(), (*looseElectron)->vertex(), data_.tracks,
					0.3, 0.01, 0.1, 0.1, 0.2, 1.5);
      if (isoRelLoose > 0.1) continue;

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
	  if ( met > metCut_.pt_min) {
        takenMuE.push_back(std::make_pair(*tightMuon,*looseElectron));
	     std::cout<<"Found MuE"<<std::endl;
	     std::vector<const reco::Candidate*> el;
	     el.push_back(*looseElectron);
	     std::vector<const reco::Candidate*> jets = jets_.getJets(Jets::DefaultJets,jet_);
	     std::vector<const reco::Candidate*> jetsnoel;
	     for ( std::vector<const reco::Candidate*>::const_iterator jet = jets.begin(); jet != jets.end(); ++ jet )
	       if ( Cuts::testJetForElectrons(**jet, el ) ) jetsnoel.push_back(*jet);
	      
	      int njet = jetsnoel.size();
	      if (njet > 4)
		{
		  njet = 4;
		}
	      countedMuEJets_[njet]++;       

	      FillHistograms(jetsnoel,*tightMuon,*looseElectron,met);
	  }
      }
    }
    // loop over loose muons
    for ( std::vector<const reco::Candidate*>::iterator looseMuon = looseMuons.begin(),
	    MuonEnd = looseMuons.end();
	  looseMuon != MuonEnd;
	  ++looseMuon ) {

      double isoRelLoose = trackRelIsolation((*looseMuon)->momentum(), (*looseMuon)->vertex(), data_.tracks,
					0.3, 0.01, 0.1, 0.1, 0.2, 1.5);
      if (isoRelLoose > 0.1) continue;
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
	  if ( met > metCut_.pt_min) {
	    // calculate invariant mass and check for special MET cut within Z window
	    // take first MET cancicate
	    math::XYZTLorentzVector inv = (*tightMuon)->p4() + (*looseMuon)->p4();
	    if ( inv.mass() >= ZRangeMinMass_ && inv.mass() <= ZRangeMaxMass_ ) {
	      if ( met > metCutAroundZ_.pt_min) {
		// passed
		std::cout<<"Found MuMu"<<std::endl;
		takenMuMu.push_back(std::make_pair(*tightMuon,*looseMuon));
    


	      std::vector<const reco::Candidate*> jetsnoel = jets_.getJets(Jets::DefaultJets,jet_);          
	      
	      int njet = jetsnoel.size();
	      if (njet > 4)
		{
		  njet = 4;
		}
	      countedMuMuJets_[njet]++;       

		FillHistograms(jetsnoel,*tightMuon,*looseMuon,met);

	      }
	    } else {
	      takenMuMu.push_back(std::make_pair(*tightMuon,*looseMuon));
        countedMuMuJets_[nJetsWithoutEl(jets)]++;

		FillHistograms(jets,*tightMuon,*looseMuon,met);

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

	for (int table_index = 1; table_index < 6; ++table_index)
	  {

	    hTable->SetBinContent(table_index,1,countedEEJets_[table_index-1]);
	    hTable->SetBinContent(table_index,2,countedEMuJets_[table_index-1]);
	    hTable->SetBinContent(table_index,3,countedMuEJets_[table_index-1]);
	    hTable->SetBinContent(table_index,4,countedMuMuJets_[table_index-1]);
	    
	  }//END table index

	hTable->Write();

	
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


void cms1::TableMaker::FillHistograms(std::vector<const reco::Candidate*> jets,const reco::Candidate *one,const reco::Candidate *two, double met)     

{
  std::cout<<"Jets size "<<jets.size()<<std::endl;
  int jetnum = jets.size();
  if (jetnum > 4) jetnum = 4;
	// Fill Number of Jets Hist
	hNJets->Fill(jetnum);
	std::vector<double> jetpt;
	//PTJetH
	//PTJetL
	double energy=0;  // highest and second highest Jet PT
	
//	if(numJets>0) hPTJetH->Fill(high);
//	if(numJets>1) hPTJetL->
	for(unsigned int i = 0;i<jets.size();++i)
	  {
	    jetpt.push_back(jets[i]->pt());
		energy+=jets[i]->et();
	  }

	std::sort(jetpt.begin(),jetpt.end());

	for(int i = 0;i<jetnum;++i)
	  {
	    hPTJet[i]->Fill(jetpt[i]);
	  }

	//Tight and loose
	hPTTight[jetnum]->Fill(one->pt());
	hPTLoose[jetnum]->Fill(two->pt());

	//PTLeading[i]
	//PTTrailing[i]
	if(one->pt() > two->pt())
	{
		hPTLeading[jetnum]->Fill(one->pt());
		hPTTrailing[jetnum]->Fill(two->pt());
	}
	else
	{
		hPTLeading[jetnum]->Fill(two->pt());
		hPTTrailing[jetnum]->Fill(one->pt());
	}
	
	//Mll[i]
	hMll[jetnum]->Fill((one->p4()+two->p4()).M());
	
	//missET[i]

	hMET[jetnum]->Fill(met);

	
	//HT[i]

	hHT[jetnum]->Fill(energy + met + one->et() + two->et());

}


