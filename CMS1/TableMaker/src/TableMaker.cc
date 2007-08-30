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
// $Date: 2007/08/07 11:13:37 $
// $Revision: 1.34 $
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

#include "Utilities/Timing/interface/TimingReport.h"
#include "Utilities/Timing/interface/TimerStack.h" 

//Number of bins in histograms
#define BINS 20

// correct the number of Jets according to electron selection
// this is a very simple algorithm which will be improved, 
// by the way the cone threshold can be passed as parameter to TableMaker.
unsigned int nJetsWithoutEl(std::vector<const reco::Candidate*> jets, const Candidate* e1=0, const Candidate* e2=0) {

  unsigned int nJets = jets.size();

  for(std::vector<const reco::Candidate*>::const_iterator itJet = jets.begin(); itJet != jets.end(); ++itJet) {
     if (e1 != 0 && ROOT::Math::VectorUtil::DeltaR(e1->p4(), (*itJet)->p4()) < 0.4) 
       --nJets;
     else
       if (e2 != 0 && ROOT::Math::VectorUtil::DeltaR(e2->p4(), (*itJet)->p4()) < 0.4) --nJets;
  }

  if (nJets > 4) nJets = 4;
  return nJets;
} 

void cms1::TableMaker::finishProcessing()
{
  // let the base analyzer finish its work
  BaseAnalyzer::finishProcessing();
	
  histogramFile->Write();
  histogramFile->Close();

  ostringstream output;
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
cms1::TableMaker::processEvent( const edm::Event& iEvent )
{
  TimerStack timers;
  timers.push("TableMaker::processEvent");
  ++events_;
  
  timers.push("TableMaker::processEvent::getInfoFromBlackBoxes");
  // get vector of muons
  std::vector<const reco::Candidate*> tightMuons = theMuons.getMuons("VeryLooseGlobalMuons",tightMuon_);
  std::vector<const reco::Candidate*> looseMuons = theMuons.getMuons("VeryLooseGlobalMuons",looseMuon_);
  std::vector<const reco::Candidate*> allMuons = theMuons.getMuons("AllGlobalMuons",allMuon_);
  theData.addBBCollection("MuonsForMETCorrection", allMuons);
  

  // get vector of electrons
  
   // tightElectron_.truthMatchingType = Cuts::Electron ; // require truth matching
   // tightElectron_.setEventData(&theData);                // let the Cuts know where to get event info (mcInfo in this case)
   // looseElectron_.truthMatchingType = Cuts::Electron;  // require truth matching
   // looseElectron_.setEventData(&theData);                // let the Cuts know where to get event info (mcInfo in this case)

   
  std::vector<const reco::Candidate*> tightElectrons = theElectrons.getElectrons("TightElectrons",tightElectron_);
  std::vector<const reco::Candidate*> looseElectrons = theElectrons.getElectrons("LooseElectrons",looseElectron_);
  
  // get vector of Jets
  // std::vector<const reco::Candidate*> jets = theJets.getJets("LooseMidPointCone5CaloJets", Cuts());
  std::vector<const reco::Candidate*> jets = theJets.getJets("LooseIterativeCone5CaloJets", Cuts());
  // add AllJets to EventData
  // theData.addBBCollection("AllJets",  theJets.getJets("AllMidPointCone5CaloJets", Cuts() ) );
  theData.addBBCollection("AllJets",  theJets.getJets("AllIterativeCone5CaloJets", Cuts() ) );
  // theData.addBBCollection("CorrectedJets",  theJets.getJets( "MCJetCorJetMcone5" ) );
  theData.addBBCollection("CorrectedJets",  theJets.getJets( "MCJetCorJetIcone5" ) );

  // get MET without cut and correct
  const reco::Candidate* metObj = theMET.getMET("DefaultMET");
  double met = metObj->pt();
  double metphi = metObj->phi();

  timers.pop_and_push("TableMaker::processEvent::dumpEventContent");

  // Dump Event contents
  if (events_ < MaxEventDebug_) {
    const std::vector<reco::CaloJet>* jetColl =
      theData.getData<std::vector<reco::CaloJet> >("midPointCone5CaloJets");

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
	
  timers.pop_and_push("TableMaker::processEvent::makeHypothesis");
  EventHyp eventHyp_; 
  eventHyp_.setEventData(&theData);
  std::vector<const cms1::DiLeptonCandidate*> dlCandidates = eventHyp_.getEventHyp(tightElectrons, looseElectrons, tightMuons, looseMuons, jets, met, metphi,
										   metCut_, metCutAroundZ_);
  if (events_ < MaxEventDebug_) {
    eventHyp_.dump(std::cout, dlCandidates);
  }

  timers.pop_and_push("TableMaker::processEvent::storeHypothesisInfo");
   
  if (makeNtuples) nCandidates->addData(dlCandidates.size()); // Fill ntuples

  for ( std::vector<const cms1::DiLeptonCandidate*>::iterator dli = dlCandidates.begin(), dle = dlCandidates.end(); dli != dle; ++dli ) {
    const cms1::DiLeptonCandidate* dl = *dli;
    if (makeNtuples) diLeptonUserData.fill(theData,*dl); // Fill ntuples
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
  if (makeNtuples) {
    diLeptonUserData.registerBlock(theData, "hyp_", "hypothesis");
    theData.intUserData.push_back( new UserData<int>("evt_nCand", "number of dilepton hypotheses in the event", false) );
    nCandidates = theData.intUserData.back();
  }
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
  //jetCut_.pt_min             = iConfig.getUntrackedParameter<double>("JetPt");
  //jetCut_.eta_min            = iConfig.getUntrackedParameter<double>("JetMinEta");
  //jetCut_.eta_max            = iConfig.getUntrackedParameter<double>("JetMaxEta");

  // MET cuts
  metCut_.pt_min             = iConfig.getUntrackedParameter<double>("MET");

  // MET cuts around Z
  metCutAroundZ_.pt_min      = iConfig.getUntrackedParameter<double>("METAroundZ");
  ZRangeMinMass_             = iConfig.getUntrackedParameter<double>("ZRangeMinMass");
  ZRangeMaxMass_             = iConfig.getUntrackedParameter<double>("ZRangeMaxMass");

  fileTag 					= iConfig.getUntrackedParameter<std::string>("fileTag");

  MaxEventDebug_             = (unsigned int) iConfig.getUntrackedParameter<int>("MaxEventDebug");

  ostringstream output;
  output << "pass0_" << fileTag << ".root";
  histogramFile = new TFile(output.str().c_str(),"RECREATE");

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
