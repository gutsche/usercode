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
// $Author: dmytro $
// $Date: 2007/08/30 13:13:52 $
// $Revision: 1.16 $
//

#include "CLHEP/HepMC/GenParticle.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "CMS1/Base/interface/UserData.h"
#include "TTree.h"
#include <vector>
class TrackDetectorAssociator;
namespace cms1 {
   struct EventData 
     {
	EventData():mcInfo(0), jetInfo(0){}
	
	// event data
	const edm::Event* iEvent;
	void setEvent( const edm::Event* iEvent );
	
	TrackDetectorAssociator* trackAssociator;

	// reference collections
	std::vector<const reco::Track*>     refTracks;
	
	// ------------ DERIVED STUFF ----------------

	// ------------ BLACK BOX COLLECTIONS ---------------
      public:
	const std::vector<const reco::Candidate*>&    getBBCollection( const std::string& name );
	bool                                          checkBBCollection( const std::string& name );
	
	// Add collection to the list of the blackbox collections.
	// If a vector of candidates corresponding to a given name is found, 
	// than this vector is assigned a new vector. If no entry with given 
	// name is found, a new entry is added to the list.
	void                                          addBBCollection( const std::string& name,
								  const std::vector<const reco::Candidate*>& coll );
	// clear all vectors in the list of collections
	void                                          clearBBCollections();
      private:
	// container for BlackBox outputs
	std::map<std::string, std::vector<const reco::Candidate*> > collections;
	
	// ------------ MC STUFF  ---------------
      public:
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
	
	template <class T> std::vector<const reco::Candidate*> getCandidates(const std::string label, const std::string instance = "") const
	  {
	     edm::Handle<std::vector<T> > t;
	     iEvent->getByLabel(label,instance,t);
	     std::vector<const reco::Candidate*> v;
	     for ( typename std::vector<T>::const_iterator i = t->begin(); i != t->end(); ++i )
	       v.push_back( &*i );
	     return v;
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
		     std::cout << "Sorry, data structure is inconsistent. Number of candidates in different blocks is different." <<
		       "\n name of the block: " << (*itr)->name() << "\t #Candidates in this block: " << (*itr)->getVector()->size() <<
		       "\t #Candidates in other blocks: " << nCandidates <<"\nAbort" << std::endl;
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
