#ifndef CMS1EventData_h
#define CMS1EventData_h
//
// Package:         CMS1/Base
// Class:           EventData
// 
// Description:     container of event information (collections, objects etc)
//
// Original Author: Dmytro Kovalskyi
//
// $Author: mangano $
// $Date: 2007/05/31 17:06:32 $
// $Revision: 1.11 $
//

#include "CLHEP/HepMC/GenParticle.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/TrackReco/interface/Track.h"
// #include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "CMS1/Base/interface/UserData.h"
#include "TTree.h"
#include <vector>
namespace cms1 {
   struct EventData 
     {
	EventData():mcInfo(0), jetInfo(0){}
	
	// event data
	const edm::Event* iEvent;
	void setEvent( const edm::Event* iEvent );
	
	// reference collections
	std::vector<const reco::Track*>     refTracks;
	std::vector<const reco::Candidate*> refMuons;
	std::vector<const reco::Candidate*> refElectrons;
	std::vector<const reco::Candidate*> refJets;
	
	// ------------ DERIVED STUFF ----------------
	// generator info 
	std::vector<HepMC::GenParticle*>  mcInfo;
	std::vector<reco::GenJet> jetInfo;

	// ------------ USER DATA by types ----------------
	std::vector<UserDataInt*>      intUserData;
	std::vector<UserDataInt1D*>    intUserData1D;
	std::vector<UserDataFloat*>    floatUserData;
	std::vector<UserDataFloat1D*>  floatUserData1D;
	std::vector<UserDataP4*>       p4UserData;
	std::vector<UserDataP41D*>     p4UserData1D;

	// ----------------- METHODS ----------------------
	// clear the event data to avoid copying old event information
	void clearUserData();
	void clearEventData();
	
	// ntuple specific stuff (not used for edm root files)
	void addBranches( TTree& tree, bool candidateBased = false );
	void fillEvent( TTree& tree, bool candidateBased = false );
	
	// --------------- the rest is a brain surgery ---------------
	// proceed only if you are a compiler or close to it in	your 
	// brain setup.
	
	template <class T> const T* getData(const std::string label, const std::string instance = "") const
	  {
	     edm::Handle<T> t;
	     iEvent->getByLabel(label,instance,t);
	     return t.product();
	  }

	template <class T> edm::Handle<T> getHandle(const std::string label, const std::string instance = "") {
	   edm::Handle<T> t;
	   iEvent->getByLabel(label, instance, t);
	   return t;
	}

	template <class T> void checkDataConsistency(const std::vector<T>& block, int& nCandidates )
	  {
	     for(typename std::vector<T>::const_iterator itr=block.begin(); itr!=block.end(); ++itr)
	       if ( (*itr)->isCandidate() ){
		  if (nCandidates == 0){
		     nCandidates = (*itr)->getVector()->size();
		     continue;
		  }
		  if (nCandidates != int((*itr)->getVector()->size()) ) {
		     std::cout << "Sorry, data structure is inconsistent. Number of candidates in different blocks is different. Abort" << std::endl;
		     assert(0);
		  }
	       }
	  }
	template <class T> void projectData(std::vector<T>& block, int index )
	  {
	     for(typename std::vector<T>::iterator itr=block.begin(); itr!=block.end(); ++itr)
	       if ( (*itr)->isCandidate() ){
		  *((*itr)->get()) = (*itr)->getVector()->at(index);
	       }
	  }

     };
   
}
#endif
