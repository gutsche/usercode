#if !defined(__CINT__)
#include <vector>
#include "TTree.h"
#include "TFile.h"
#include <iostream>
#include "CMS1/Base/interface/Cuts.h"
#include "CMS1/Muons/interface/Muons.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "CMS1/MuonAnalyzerLite/interface/MuonAnalyzerLite.h"
#endif

void cms1::MuonTestAnalyzer(TFile& file)
{
   // get tree
   TTree *tree = (TTree*)file.Get("Events");

   // create and connect muon collection branch
   std::vector<reco::Muon> muonCollection;
   tree->SetBranchStatus("*",0);
   tree->SetBranchStatus("recoTracks_*",1);
   tree->SetBranchStatus("recoMuons_*",1);
   
   tree->SetBranchAddress("recoMuons_globalMuons__RECO.obj",&muonCollection);

   // instanciate cms1 muons object
   cms1::Muons muons;
   cms1::EventData eventData;
   muons.setEventData(&eventData);
   eventData.globalMuonCollection = &muonCollection;
   cms1::Cuts cuts;
   cuts.pt_min = 2;
   
   for ( unsigned int index = 0; index < tree->GetEntries(); ++index ) {
      std::cout << "event: " << index << std::endl;
      tree->GetEntry(index);
      tree->SetBranchAddress("recoMuons_globalMuons__RECO.obj",&muonCollection);
      
      std::cout << "Number of global muons: " << muonCollection.size() << std::endl;
      std::vector<const reco::Candidate*> output(muons.getMuons(cms1::Muons::TightGlobalMuons,cuts));
      
      // print if 2 or more muons are found with pT > 2 GeV
      if ( output.size() >= 2 ) {
	 std::cout << "Two or more muons found with pT > 2 GeV" << std::endl;
      }
   }
}


void cms1::MuonTestAnalyzerSimplified(TFile& file)
{
   // get tree
   TTree *tree = (TTree*)file.Get("Events");

   // create and connect muon collection branch
   std::vector<reco::Muon> muonCollection;
   tree->SetBranchStatus("*",0);
   tree->SetBranchStatus("recoTracks_*",1);
   tree->SetBranchStatus("recoMuons_*",1);
   
   tree->SetBranchAddress("recoMuons_globalMuons__RECO.obj",&muonCollection);

   // instanciate cms1 muons object
   cms1::Muons muons;
   cms1::EventData eventData;
   muons.setEventData(&eventData);
   eventData.globalMuonCollection = &muonCollection;
   cms1::Cuts cuts;
   cuts.pt_min = 2;
   
   for ( unsigned int index = 0; index < tree->GetEntries(); ++index ) {
      std::cout << "event: " << index << std::endl;
      tree->GetEntry(index);
      tree->SetBranchAddress("recoMuons_globalMuons__RECO.obj",&muonCollection);
      
      std::cout << "Number of global muons: " << muonCollection.size() << std::endl;
      
      // print if 2 or more muons are found with pT > 2 GeV
      if ( muons.numberOfMuons(cms1::Muons::TightGlobalMuons,cuts) >= 2 ) {
	 std::cout << "Two or more muons found with pT > 2 GeV" << std::endl;
      }
   }
}


