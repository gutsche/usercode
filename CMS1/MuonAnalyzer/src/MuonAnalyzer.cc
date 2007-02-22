//
// Package:         CMS1/MuonAnalyzer
// Class:           MuonAnalyzer

// 
// Description:     EDAnalyzer using cms1 muons object
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Mon Jan 29 17:42:34 UTC 2007
//
// $Author: gutsche $
// $Date: 2007/01/29 18:15:27 $
// $Revision: 1.1 $
//

#include "CMS1/MuonAnalyzer/interface/MuonAnalyzer.h"
#include "CMS1/Base/interface/Cuts.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/MuonReco/interface/Muon.h"

cms1::MuonAnalyzer::MuonAnalyzer(const edm::ParameterSet& iConfig)
{

  // input tags
  globalMuonInputTag_ = iConfig.getUntrackedParameter<edm::InputTag>("GlobalMuonInputTag");

  // parameter
  number_             = iConfig.getUntrackedParameter<unsigned int>("number");
  pt_                 = iConfig.getUntrackedParameter<double>("pt");

}


cms1::MuonAnalyzer::~MuonAnalyzer()
{
 
}


void
cms1::MuonAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   // get muon collection from the event (In this exaple: global muons)
   // This needs to be done for EVERY EVENT
   const reco::MuonCollection *muonCollection = 0;
   edm::Handle<reco::MuonCollection> muonCollectionHandle;
   iEvent.getByLabel(globalMuonInputTag_,muonCollectionHandle);
   muonCollection = muonCollectionHandle.product();
   
   // pass the collection pointer of the collection that we 
   // want to use to the MuonData object.
   // This needs to be done for EVERY EVENT
   Muons::MuonData& data = muons_.getData();
   data.globalMuonCollection = muonCollection;
   
   // define external cuts
   // Initialized to have all cuts turned off 
   Cuts thisCuts;  
   thisCuts.pt_min = pt_;  // turn on the pt_min cut
   
   // Get a vector of pointers to muons passing "thisCuts" and 
   // passing the "TightGlobalMuons" cuts
   // These will be the muons that we work with
   std::vector<const reco::Muon*> selectedMuons = muons_.getMuons(Muons::TightGlobalMuons,thisCuts);

   // How many muons did we select
   if ( selectedMuons.size() >= number_ ) {
      edm::LogVerbatim("CMS1MuonAnalyzer") << number_ << " or more muons found with pT > " << pt_ << " GeV";
   }

   // This is a sample loop over selected muons. For each selected
   // muon we will print out its pt.  We will also find the highest
   // pt one
   double ptmax = 0;
   const reco::Muon* stiffestMuon=0;
   for (std::vector<const reco::Muon*>::const_iterator muon = selectedMuons.begin();
	muon != selectedMuons.end(); ++muon)
     {
	if ((*muon)->pt() > ptmax)
	  {
	     ptmax = (*muon)->pt();
	     stiffestMuon = *muon;
	  }
	std::cout << "muon pt: " <<  (*muon)->pt() << std::endl; 
     }
   
   // As a sanity check, print the highest pt
   if (stiffestMuon)
     edm::LogVerbatim("CMS1MuonAnalyzer") << "The pt of the stiffest muon is " << stiffestMuon->pt();
}


void 
cms1::MuonAnalyzer::beginJob(const edm::EventSetup& setup)
{
}

void 
cms1::MuonAnalyzer::endJob() {
}
