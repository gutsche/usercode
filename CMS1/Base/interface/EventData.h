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
// $Date: 2007/05/11 04:08:51 $
// $Revision: 1.6 $
//

#include "CLHEP/HepMC/GenParticle.h"
#include "DataFormats/JetReco/interface/GenJet.h"
// #include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "CMS1/Base/interface/UserData.h"
#include "TTree.h"

namespace cms1 {
   struct EventData 
     {
	EventData():mcInfo(0), jetInfo(0){}
	
	// event data
	const edm::Event* iEvent;
	
	// ------------ DERIVED STUFF ----------------
	// generator info 
	std::vector<HepMC::GenParticle>  mcInfo;
	std::vector<reco::GenJet> jetInfo;

	// ------------ USER DATA ----------------
	// int container
	std::vector<UserDataInt*>    intUserData;
	// int vector container
	std::vector<UserDataVInt*>   vintUserData;
	// float container
	std::vector<UserDataFloat*>  floatUserData;
	// float vector container
	std::vector<UserDataVFloat*> vfloatUserData;
	// p4 container
	std::vector<UserDataP4*>     p4UserData;
	// p4 vector container
	std::vector<UserDataVP4*>    vp4UserData;

	// ------------ METHODS ----------------------
	template <class T> const T* getData(const std::string label, const std::string instance = "") const
	  {
	     edm::Handle<T> t;
	     iEvent->getByLabel(label,instance,t);
	     return t.product();
	  }
	void clearUserData()
	  {
	     for(std::vector<UserDataVInt*>::iterator itr=vintUserData.begin(); itr!=vintUserData.end(); ++itr)
	       (*itr)->get()->clear();
	     for(std::vector<UserDataVFloat*>::iterator itr=vfloatUserData.begin(); itr!=vfloatUserData.end(); ++itr)
	       (*itr)->get()->clear();
	     for(std::vector<UserDataVP4*>::iterator itr=vp4UserData.begin(); itr!=vp4UserData.end(); ++itr)
	       (*itr)->get()->clear();
	  }
	void addBranches( TTree& tree )
	  {
	     for(std::vector<UserDataInt*>::iterator itr=intUserData.begin(); itr!=intUserData.end(); ++itr)
	       tree.Branch((*itr)->name().c_str(),"int",(*itr)->getAddress());
	     for(std::vector<UserDataVInt*>::iterator itr=vintUserData.begin(); itr!=vintUserData.end(); ++itr)
	       tree.Branch((*itr)->name().c_str(),"std::vector<int>",(*itr)->getAddress());
	     for(std::vector<UserDataFloat*>::iterator itr=floatUserData.begin(); itr!=floatUserData.end(); ++itr)
	       tree.Branch((*itr)->name().c_str(),"float",(*itr)->getAddress());
	     for(std::vector<UserDataVFloat*>::iterator itr=vfloatUserData.begin(); itr!=vfloatUserData.end(); ++itr)
	       tree.Branch((*itr)->name().c_str(),"std::vector<float>",(*itr)->getAddress());
	     for(std::vector<UserDataP4*>::iterator itr=p4UserData.begin(); itr!=p4UserData.end(); ++itr)
	       tree.Branch((*itr)->name().c_str(),"ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >",(*itr)->getAddress());
	     for(std::vector<UserDataVP4*>::const_iterator itr=vp4UserData.begin(); itr!=vp4UserData.end(); ++itr)
	       tree.Branch((*itr)->name().c_str(),"std::vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > >",(*itr)->getAddress());
	     // tree.Branch(((*itr)->name()+".").c_str(),"std::vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > >",(*itr)->getAddress());
	  }
     };
   
}
#endif
