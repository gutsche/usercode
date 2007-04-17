//
// Package:         CMS1/SampleAnalyzer
// Class:           SampleAnalyzer
// 
// Description:     working example of analysis in the T'n'S framework
//
// Original Author: Dmytro Kovalskyi
//
// $Author: dmytro $
// $Date: 2007/04/08 22:55:38 $
// $Revision: 1.3 $
//
#include "CMS1/SampleAnalyzer/interface/SampleAnalyzer.h"
#include <iostream>

void cms1::SampleAnalyzer::configure(const edm::ParameterSet& iConfig)
{
   // get parameters from config file specific for your analysis                                   //usercode
   pt_ = 2; //GeV                                                                                  //usercode
   number_ = 2;                                                                                    //usercode
}

void cms1::SampleAnalyzer::processEvent( const edm::Event& iEvent )
{
   // define external cuts                                                                         //usercode
   // Initialized to have all cuts turned off                                                      //usercode
   Cuts thisCuts;                                                                                  //usercode
   thisCuts.pt_min = pt_;  // turn on the pt_min cut                                               //usercode
                                                                                                   //usercode
   // Get a vector of pointers to muons passing "thisCuts" and                                     //usercode
   // passing the "TightGlobalMuons" cuts                                                          //usercode
   // These will be the muons that we work with                                                    //usercode
   std::vector<const reco::Candidate*> selectedMuons =                                             //usercode
     theMuons.getMuons(Muons::AllGlobalMuons,thisCuts);                                            //usercode
                                                                                                   //usercode
   // number of muons in the event                                                                 //usercode
   std::cout << "Number of AllGlobalMuons: " << selectedMuons.size() << std::endl;                 //usercode
                                                                                                   //usercode
   // How many muons did we select                                                                 //usercode
   if ( selectedMuons.size() >= number_ ) {                                                        //usercode
      std::cout << number_ << " or more muons found with pT > " << pt_ << " GeV" << std::endl;     //usercode
   }                                                                                               //usercode
                                                                                                   //usercode
   // This is a sample loop over selected muons. For each selected                                 //usercode
   // muon we will print out its pt.  We will also find the highest                                //usercode
   // pt one                                                                                       //usercode
   double ptmax = 0;                                                                               //usercode
   const reco::Candidate* stiffestMuon=0;                                                          //usercode
   for (std::vector<const reco::Candidate*>::const_iterator muon = selectedMuons.begin();          //usercode
	muon != selectedMuons.end(); ++muon)                                                       //usercode
     {                                                                                             //usercode
	if ((*muon)->pt() > ptmax)                                                                 //usercode
	  {                                                                                        //usercode
	     ptmax = (*muon)->pt();                                                                //usercode
	     stiffestMuon = *muon;                                                                 //usercode
	  }                                                                                        //usercode
	std::cout << "muon pt: " <<  (*muon)->pt() << std::endl;                                   //usercode
     }                                                                                             //usercode
                                                                                                   //usercode
   // As a sanity check, print the highest pt                                                      //usercode
   if (stiffestMuon)                                                                               //usercode
     std::cout << "The pt of the stiffest muon is " << stiffestMuon->pt() << std::endl;            //usercode
                                                                                                   //usercode
   // let's try to read in something absolutely arbitrary, which has                               //usercode
   // supported C++ type. For example reco::Track collection rsWithMaterialTracks                  //usercode
   // register this collection in the EventData                                                    //usercode
   // lets check how many rsWithMaterialTracks tracks are found in the event and dump them         //usercode
   // The EventData automatically loads registered collections, so we can simply use it            //usercode
   const std::vector<reco::Track>* tracks =                                                        //usercode
     theData.getData<std::vector<reco::Track> >("rsWithMaterialTracks");                           //usercode
   if ( tracks ) {                                                                                 //usercode
      std::cout << "Number of rsWithMaterialTracks tracks found: " << tracks->size() << std::endl; //usercode
      for(std::vector<reco::Track>::const_iterator track = tracks->begin();                        //usercode
	  track != tracks->end(); ++track)                                                         //usercode
	std::cout << "\tPt: " << track->pt() << std::endl;                                         //usercode
   }                                                                                               //usercode
}

