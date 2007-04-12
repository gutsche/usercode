//
// Package:         CMS1/TableMaker
// Class:           TableMaker

// 
// Description:     EDAnalyzer filling table for di-lepton analysis dependent on number of jets
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Tue Feb 20 23:00:01 UTC 2007
//
// $Author: latb $
// $Date: 2007/03/28 22:45:07 $
// $Revision: 1.19 $
//

#include <vector>
#include <algorithm> 
#include <cmath>
#include <utility>
#include <sstream>
#include <iomanip>

#include "CMS1/TableMaker/interface/TableMaker.h"
#include "CMS1/Base/interface/Dump.h"
#include "CMS1/EventHyp/interface/EventHyp.h"

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
  ostringstream output;
/*  output << "pass0_" << fileTag << ".root";
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
*/
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

void
cms1::TableMaker::processEvent()
{

  ++events_;
   
  // get vector of muons
  std::vector<const reco::Candidate*> tightMuons = theMuons.getMuons(Muons::TightGlobalMuons,tightMuon_);
  std::vector<const reco::Candidate*> looseMuons = theMuons.getMuons(Muons::LooseGlobalMuons,looseMuon_);
  std::vector<const reco::Candidate*> allMuons = theMuons.getMuons(Muons::AllGlobalMuons,allMuon_);

  // get vector of electrons
   tightElectron_.truthMatchingType = Cuts::Electron ; // require truth matching
   tightElectron_.setEventData(&theData);                // let the Cuts know where to get event info (mcInfo in this case)
   looseElectron_.truthMatchingType = Cuts::Electron;  // require truth matching
   looseElectron_.setEventData(&theData);                // let the Cuts know where to get event info (mcInfo in this case)

  std::vector<const reco::Candidate*> tightElectrons = theElectrons.getElectrons(Electrons::TightElectrons,tightElectron_);
  std::vector<const reco::Candidate*> looseElectrons = theElectrons.getElectrons(Electrons::LooseElectrons,looseElectron_);

// get vector of Jets
  std::vector<const reco::Candidate*> jets = theJets.getJets(Jets::DefaultJets,jetCut_);

// get MET without cut and correct
   const reco::Candidate* metObj = theMET.getMET(MET::DefaultMET);
   double met = metObj->pt();
   double mephi = metObj->phi();
// ACHTUNG, Baby -- here we change met and mephi in situ!!
// but metObj is not changed 
   MET::correctMETmuons(&allMuons, met, mephi);


// Dump Event contents
	if (events_ < MaxEventDebug_) {
	   const std::vector<reco::CaloJet>* jetColl =
	     theData.container_reco_CaloJet.getCollection(edm::InputTag("midPointCone5CaloJets",""));

		std::cout << "------------------------------------------------------------------------" << std::endl; 
		std::cout << "Dump of Event, " 
		<< " Ne = " << looseElectrons.size() 
		<< " Nmu = " << allMuons.size() 
		<< " Nj = " << jets.size()		 
		<< " Nj (def.Jets) = " << ( jetColl == 0 ? -1 : int(jetColl->size()) )
		<< std::endl;	
		theElectrons.dump(std::cout, looseElectrons);
 		theMuons.dump(std::cout, allMuons);
 		theJets.dump(std::cout, jets);
 		theMET.dump(std::cout, metObj);
		std::cout << "------------------------------------------------------------------------" << std::endl; 
	}
	
	EventHyp eventHyp_; 
	eventHyp_.setEventData(&theData);
	std::vector<const cms1::DiLeptonCandidate*> dlCandidates = eventHyp_.getEventHyp(
		tightElectrons, looseElectrons, tightMuons, looseMuons, jets, met,
		metCut_, metCutAroundZ_);
	if (events_ < MaxEventDebug_) {
		eventHyp_.dump(std::cout, dlCandidates);
	}

	for ( std::vector<const cms1::DiLeptonCandidate*>::iterator dli = dlCandidates.begin(), dle = dlCandidates.end(); dli != dle; ++dli ) {
		const cms1::DiLeptonCandidate* dl = *dli;
		FillHistograms(dl->jets, dl->lTight, dl->lLoose, dl->MET);
		int njet = dl->nJets(); if (njet > 4)  njet = 4;
		switch (dl->candidateType) {
			case cms1::DiLeptonCandidate::MuMu: countedMuMuJets_[njet]++; 
				break;
			case cms1::DiLeptonCandidate::MuEl: countedMuEJets_[njet]++;       
				break;
			case cms1::DiLeptonCandidate::ElMu: countedEMuJets_[njet]++;       
				break;
			case cms1::DiLeptonCandidate::ElEl: countedEEJets_[njet]++;
				break;
			default: std::cout << "ERROR: DiLeptonCandidate not MuMu, MuEl, ElMu or ElEl -- This can never happen!!" << std::endl;
		}
	}
	return; 
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

void cms1::TableMaker::configure(const edm::ParameterSet& iConfig)
{
   // load standard configuration (black boxes)
   BaseAnalyzer::configure( iConfig );

  // tight muon cuts
  tightMuon_.pt_min       = iConfig.getUntrackedParameter<double>("TightMuonPt");
  tightMuon_.eta_min      = iConfig.getUntrackedParameter<double>("TightMuonMinEta");
  tightMuon_.eta_max      = iConfig.getUntrackedParameter<double>("TightMuonMaxEta");

  // loose muon cuts
  looseMuon_.pt_min       = iConfig.getUntrackedParameter<double>("LooseMuonPt");
  looseMuon_.eta_min      = iConfig.getUntrackedParameter<double>("LooseMuonMinEta");
  looseMuon_.eta_max      = iConfig.getUntrackedParameter<double>("LooseMuonMaxEta");

  // all muon cuts
  allMuon_.pt_min       = iConfig.getUntrackedParameter<double>("AllMuonPt");
  allMuon_.eta_min      = iConfig.getUntrackedParameter<double>("AllMuonMinEta");
  allMuon_.eta_max      = iConfig.getUntrackedParameter<double>("AllMuonMaxEta");

  // tight electron cuts
  tightElectron_.pt_min   = iConfig.getUntrackedParameter<double>("TightElectronPt");
  tightElectron_.eta_min  = iConfig.getUntrackedParameter<double>("TightElectronMinEta");
  tightElectron_.eta_max  = iConfig.getUntrackedParameter<double>("TightElectronMaxEta");

  // loose electron cuts
  looseElectron_.pt_min   = iConfig.getUntrackedParameter<double>("LooseElectronPt");
  looseElectron_.eta_min  = iConfig.getUntrackedParameter<double>("LooseElectronMinEta");
  looseElectron_.eta_max  = iConfig.getUntrackedParameter<double>("LooseElectronMaxEta");

  // jet cuts
  jetCut_.pt_min             = iConfig.getUntrackedParameter<double>("JetPt");
  jetCut_.eta_min            = iConfig.getUntrackedParameter<double>("JetMinEta");
  jetCut_.eta_max            = iConfig.getUntrackedParameter<double>("JetMaxEta");

  // MET cuts
  metCut_.pt_min             = iConfig.getUntrackedParameter<double>("MET");

  // MET cuts around Z
  metCutAroundZ_.pt_min      = iConfig.getUntrackedParameter<double>("METAroundZ");
  ZRangeMinMass_             = iConfig.getUntrackedParameter<double>("ZRangeMinMass");
  ZRangeMaxMass_             = iConfig.getUntrackedParameter<double>("ZRangeMaxMass");

  fileTag 					= iConfig.getUntrackedParameter<std::string>("fileTag");

  MaxEventDebug_             = (unsigned int) iConfig.getUntrackedParameter<int>("MaxEventDebug");

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
	
  for(int i=0; i<5; ++i) {
    countedEEJets_[i] = 0;
    countedEMuJets_[i] = 0;
    countedMuEJets_[i] = 0;
    countedMuMuJets_[i] = 0;
  }

  events_ = 0;

}


