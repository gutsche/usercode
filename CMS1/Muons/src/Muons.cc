//
// Package:         CMS1/Muons
// Class:           Muons
// 
// Description:     analysis functions for muons
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Mon Jan 29 16:40:39 UTC 2007
//
// $Author: dmytro $
// $Date: 2007/08/07 11:13:37 $
// $Revision: 1.17 $
//

#include "CMS1/Muons/interface/Muons.h"
#include <iostream>
#include "CMS1/CommonTools/interface/UserDataTools.h"
#include "RecoMuon/MuonIdentification/interface/IdGlobalFunctions.h"

std::vector<const reco::Candidate*> cms1::Muons::getMuons(const std::string& muonType,
							  const Cuts& userCuts,
							  Cuts::IsolationType isolationFlag )
{
   // this is the output list
   std::vector<const reco::Candidate*> output_list;

   if (! data_ ) {
      std::cout << "ERROR: muon black box doesn't know where to find EvenData." << std::endl;
      assert(0);
   }
   
   ///////////////////////////////////////////////////////
   
   if ( muonType.compare("AllGlobalMuons")==0 )
     {
	const std::vector<reco::Muon>* collection = data_->getData<std::vector<reco::Muon> >("muons");
	if ( ! collection ) {
	   std::cout << "ERROR: global muon collection is not found in the event. Return nothing." << std::endl;
	   return output_list;
	}
	   	   
	// set the default cuts for this type
	Cuts cuts;
	cuts.isolated = isolationFlag;
	cuts.setEventData( data_ );
	cuts.AND(userCuts);
	   
	for ( std::vector<reco::Muon>::const_iterator muon = collection->begin();  muon != collection->end(); ++muon ) 
	  if ( cuts.testCandidate(*muon) ) output_list.push_back(&*muon);
	return output_list;
     }
   
   ////////////////////////////////////////////////////////
   
   if ( muonType.compare("AllTrackerMuons")==0 )
     {
	const std::vector<reco::Muon>* collection = data_->getData<std::vector<reco::Muon> >("trackerMuons");
	if ( ! collection ) {
	   std::cout << "ERROR: tracker muon collection is not found in the event. Return nothing." << std::endl;
	   return output_list;
	}
	   	   
	// set the default cuts for this type
	Cuts cuts;
	cuts.isolated = isolationFlag;
	cuts.setEventData( data_ );
	cuts.AND(userCuts);
	
	for ( std::vector<reco::Muon>::const_iterator muon = collection->begin(); muon != collection->end(); ++muon ) 
	  if ( cuts.testCandidate(*muon) ) output_list.push_back(&*muon);
	return output_list;
     }
   
   //////////////////////////////////////////////////////////
  
   if ( muonType.compare("VeryLooseGlobalMuons")==0 )
     {
	Cuts cuts;
	cuts.pt_min = 20;
	cuts.AND(userCuts);
	cuts.setEventData(data_);
	   
	return getMuons("AllGlobalMuons", cuts, isolationFlag);
     }

   //////////////////////////////////////////////////////////
  
   if ( muonType.compare("VeryLooseTrackerMuons")==0 )
     {
	Cuts cuts;
	cuts.pt_min = 20;
	cuts.AND(userCuts);
	cuts.setEventData(data_);
	   
	return getMuons("AllTrackerMuons", cuts, isolationFlag);
     }
   
   //////////////////////////////////////////////////////////
   
   if ( muonType.compare("LooseTrackerMuons")==0 )
     {
	Cuts cuts;
	cuts.pt_min = 20;
	cuts.AND(userCuts);
	cuts.setEventData(data_);
	
	const std::vector<const reco::Candidate*>& muons = getMuons("AllTrackerMuons", cuts, isolationFlag);
	for (std::vector<const reco::Candidate*>::const_iterator cand = muons.begin();
	     cand != muons.end(); ++cand)
	  if ( const reco::Muon* muon = dynamic_cast<const reco::Muon*>(*cand) )
	    if ( muonid::isGoodMuon(*muon, muonid::TMLastStationLoose) )
	      output_list.push_back(muon);
	return output_list;
     }

   //////////////////////////////////////////////////////////
   
   if ( muonType.compare("TightTrackerMuons")==0 )
     {
	Cuts cuts;
	cuts.pt_min = 20;
	cuts.AND(userCuts);
	cuts.setEventData(data_);
	
	const std::vector<const reco::Candidate*>& muons = getMuons("AllTrackerMuons", cuts, isolationFlag);
	for (std::vector<const reco::Candidate*>::const_iterator cand = muons.begin();
	     cand != muons.end(); ++cand)
	  if ( const reco::Muon* muon = dynamic_cast<const reco::Muon*>(*cand) )
	    if ( muonid::isGoodMuon(*muon, muonid::TMLastStationTight) )
	      output_list.push_back(muon);
	return output_list;
     }

   //////////////////////////////////////////////////////////
   
   std::cout << "ERROR: Unkown or not implemented muon type: " << muonType << "\nAbort" << std::endl;
   assert(0);
   return output_list;
}

void cms1::Muons::dump(std::ostream& o, std::vector<const reco::Candidate*> ml) {
	for ( std::vector<const reco::Candidate*>::iterator i = ml.begin(), ie = ml.end(); i != ie; ++i ) {
		const reco::Candidate* cp = *i;
		o << "Muon     "; 
		o << "Pt = " << cp->pt(); 
		o << ", Eta = " << cp->eta(); 
		o << ", Phi = " << cp->phi(); 
	   const  std::vector<reco::Track>* tracks = data_->getData<std::vector<reco::Track> >("ctfWithMaterialTracks");
	   if ( tracks) {
	      double isoRel = cms1::Cuts::trackRelIsolation(cp->momentum(), cp->vertex(), tracks, 0.3, 0.01, 0.1, 999.9, 0.5, 1.5, 7);
	      o << ", isol = " << isoRel;
	   }
	   o << std::endl; 
	}
}

void cms1::Muons::registerEventUserData()
{
   evtMuons.registerBlock( *data_, "mus_", "reference muons,");
   data_->intUserData.push_back( new UserData<int>("evt_nmus", "number of muons in the reference collection", false) );
   nMuons = data_->intUserData.back();
   data_->intUserData1D.push_back( new UserDataInt1D("mus_map_LooseTrackerMuons", "RefMuons to LooseTrackerMuons map", false) );
   map_LooseTrackerMuons = data_->intUserData1D.back();
   data_->intUserData1D.push_back( new UserDataInt1D("mus_map_TightTrackerMuons", "RefMuons to TightTrackerMuons map", false) );
   map_TightTrackerMuons = data_->intUserData1D.back();
}

void cms1::Muons::fillEventUserData()
{
   // std::vector<const reco::Candidate*> muons = getMuons("AllGlobalMuons");
   // data_->addBBCollection("refMuons", muons);
   std::vector<const reco::Candidate*> muons = data_->getBBCollection("refMuons");
   evtMuons.fill( getStreamerArguments(data_, muons) );
   nMuons->addData( muons.size() );
   map_LooseTrackerMuons->addData( getBlackBoxMap(muons,getMuons("LooseTrackerMuons")) );
   map_TightTrackerMuons->addData( getBlackBoxMap(muons,getMuons("TightTrackerMuons")) );
}


