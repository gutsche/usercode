//
// Package:         CMS1/SampleAnalyzer
// Class:           SampleAnalyzer
// 
// Description:     working example of analysis in the T'n'S framework
//
// Original Author: Dmytro Kovalskyi
//
// $Author: dmytro $
// $Date: 2007/03/16 07:00:21 $
// $Revision: 1.1 $
//
#include "CMS1/SampleAnalyzer/interface/SampleAnalyzer.h"
#include <iostream>

void cms1::SampleAnalyzer::configure(const edm::ParameterSet& iConfig)
{
   // load standard configuration (black boxes)
   BaseAnalyzer::configure( iConfig );
   
   // get parameters from config file specific for your analysis
   pt_ = 2; //GeV
   number_ = 2;
   
   // let's try to read in something absolutely arbitrary, which has
   // supported C++ type. For example reco::Track collection rsWithMaterialTracks
   // register this collection in the EventData
   theData.container_reco_Track.registerCollection(edm::InputTag("rsWithMaterialTracks",""),
						   "recoTracks_rsWithMaterialTracks__RECO");
}

void cms1::SampleAnalyzer::processEvent()
{
   // define external cuts
   // Initialized to have all cuts turned off 
   Cuts thisCuts;  
   thisCuts.pt_min = pt_;  // turn on the pt_min cut
   
   // Get a vector of pointers to muons passing "thisCuts" and 
   // passing the "TightGlobalMuons" cuts
   // These will be the muons that we work with
   std::vector<const reco::Candidate*> selectedMuons = theMuons.getMuons(Muons::TightGlobalMuons,thisCuts);

   // number of muons in the event
   std::cout << "Number of muons found: " << selectedMuons.size() << std::endl;
   
   // How many muons did we select
   if ( selectedMuons.size() >= number_ ) {
      std::cout << number_ << " or more muons found with pT > " << pt_ << " GeV" << std::endl;
   }

   // This is a sample loop over selected muons. For each selected
   // muon we will print out its pt.  We will also find the highest
   // pt one
   double ptmax = 0;
   const reco::Candidate* stiffestMuon=0;
   for (std::vector<const reco::Candidate*>::const_iterator muon = selectedMuons.begin();
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
     std::cout << "The pt of the stiffest muon is " << stiffestMuon->pt() << std::endl;
   
   // lets check how many rsWithMaterialTracks tracks are found in the event and dump them
   // The EventData automatically loads registered collections, so we can simply use it
   const std::vector<reco::Track>* tracks = 
     theData.container_reco_Track.getCollection(edm::InputTag("rsWithMaterialTracks",""));
   if ( tracks ) {
      std::cout << "Number of rsWithMaterialTracks tracks found: " << tracks->size() << std::endl;
      for(std::vector<reco::Track>::const_iterator track = tracks->begin(); 
	  track != tracks->end(); ++track)
	std::cout << "\tPt: " << track->pt() << std::endl;
   }
}

