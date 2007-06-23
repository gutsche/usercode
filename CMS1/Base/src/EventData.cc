#include "CMS1/Base/interface/EventData.h"

const std::vector<const reco::Candidate*>& cms1::EventData::getBBCollection( const std::string& name )
{
   std::map<std::string, std::vector<const reco::Candidate*> >::const_iterator coll = collections.find(name);
   if (coll == collections.end())
     {
	std::cout << "Cannot find collection with name: " << name << 
	  "\nMake sure something puts this collection into event or that you spelled the name correctly. Abort";
	assert(0);
     }
   return coll->second;
   
}

void cms1::EventData::addBBCollection( const std::string& name, 
				       const std::vector<const reco::Candidate*>& coll )
{
   collections[name] = coll;
}

void cms1::EventData::clearBBCollections()
{
   for (std::map<std::string, std::vector<const reco::Candidate*> >::iterator coll =
	collections.begin(); coll != collections.end(); ++coll)
     coll->second.clear();
}

void cms1::EventData::clearUserData()
{
   for(std::vector<UserDataInt*>::iterator itr=intUserData.begin(); itr!=intUserData.end(); ++itr)
     (*itr)->clearData();
   for(std::vector<UserDataInt1D*>::iterator itr=intUserData1D.begin(); itr!=intUserData1D.end(); ++itr)
     (*itr)->clearData();
   for(std::vector<UserDataFloat*>::iterator itr=floatUserData.begin(); itr!=floatUserData.end(); ++itr)
     (*itr)->clearData();
   for(std::vector<UserDataFloat1D*>::iterator itr=floatUserData1D.begin(); itr!=floatUserData1D.end(); ++itr)
     (*itr)->clearData();
   for(std::vector<UserDataP4*>::iterator itr=p4UserData.begin(); itr!=p4UserData.end(); ++itr)
     (*itr)->clearData();
   for(std::vector<UserDataP41D*>::iterator itr=p4UserData1D.begin(); itr!=p4UserData1D.end(); ++itr)
     (*itr)->clearData();
}

void cms1::EventData::clearEventData()
{
   clearBBCollections();
   refTracks.clear();
   mcInfo.clear();
   jetInfo.clear();
}

void cms1::EventData::setEvent( const edm::Event* event )
{
   clearEventData();
   clearUserData();
   iEvent = event;
}

void cms1::EventData::addBranches( TTree& tree, bool candidateBased )
{
   for(std::vector<UserDataInt*>::iterator itr=intUserData.begin(); itr!=intUserData.end(); ++itr)
     if ( ! candidateBased && (*itr)->isCandidate() ){
	tree.Branch((*itr)->name().c_str(),"std::vector<int>",(*itr)->getVectorAddress());
     }else{
	tree.Branch((*itr)->name().c_str(),(*itr)->get(),((*itr)->name()+"/I").c_str());
     }
	
   for(std::vector<UserDataInt1D*>::iterator itr=intUserData1D.begin(); itr!=intUserData1D.end(); ++itr)
     if ( ! candidateBased && (*itr)->isCandidate() ){
	tree.Branch((*itr)->name().c_str(),"std::vector<std::vector<int> >",(*itr)->getVectorAddress());
     }else{
	tree.Branch((*itr)->name().c_str(),"std::vector<int>",(*itr)->getAddress());
     }
      
   for(std::vector<UserDataFloat*>::iterator itr=floatUserData.begin(); itr!=floatUserData.end(); ++itr)
     if ( ! candidateBased && (*itr)->isCandidate() ){
	tree.Branch((*itr)->name().c_str(),"std::vector<float>",(*itr)->getVectorAddress());
     }else{
	tree.Branch((*itr)->name().c_str(),(*itr)->get(),((*itr)->name()+"/F").c_str());
     }
   for(std::vector<UserDataFloat1D*>::iterator itr=floatUserData1D.begin(); itr!=floatUserData1D.end(); ++itr)
     if ( ! candidateBased && (*itr)->isCandidate() ){
	tree.Branch((*itr)->name().c_str(),"std::vector<std::vector<float> >",(*itr)->getVectorAddress());
     }else{
	tree.Branch((*itr)->name().c_str(),"std::vector<float>",(*itr)->getAddress());
     }
   
   for(std::vector<UserDataP4*>::iterator itr=p4UserData.begin(); itr!=p4UserData.end(); ++itr)
     if ( ! candidateBased && (*itr)->isCandidate() ){
	tree.Branch((*itr)->name().c_str(),"std::vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > >",(*itr)->getVectorAddress());
     }else{
	tree.Branch((*itr)->name().c_str(),"ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >",(*itr)->getAddress());
     }
   for(std::vector<UserDataP41D*>::const_iterator itr=p4UserData1D.begin(); itr!=p4UserData1D.end(); ++itr)
     if ( ! candidateBased && (*itr)->isCandidate() ){
	tree.Branch((*itr)->name().c_str(),"std::vector<std::vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > > >",(*itr)->getVectorAddress());
     }else{
	tree.Branch((*itr)->name().c_str(),"std::vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > >",(*itr)->getAddress());
     }
}

void cms1::EventData::fillEvent( TTree& tree, bool candidateBase)
{
   if (! candidateBase ) {
      tree.Fill();
      return;
   }
   // Ok, here we need a six-pack to understand what is going on.
   // Basically we read the internal vector of a user data block,
   // assign its values one by one to theData and call Fill method 
   // for the tree. Clearly it will store the same information for
   // event blocks for all candidate. All candidates must be 
   // synchronized. So let's first check consistency of all candidate 
   // blocks.
   int nCandidates = 0;
   checkDataConsistency( intUserData,   nCandidates );
   checkDataConsistency( intUserData1D, nCandidates );
   checkDataConsistency( floatUserData,   nCandidates );
   checkDataConsistency( floatUserData1D, nCandidates );
   checkDataConsistency( p4UserData,   nCandidates );
   checkDataConsistency( p4UserData1D, nCandidates );
   
   // if we are still alive it's already pretty good achievement
   if ( nCandidates == 0 ) return;  //we are not interested in empty events
   
   for(int i=0; i<nCandidates; ++i) {
      projectData( intUserData,   i );
      projectData( intUserData1D, i );
      projectData( floatUserData,   i );
      projectData( floatUserData1D, i );
      projectData( p4UserData,   i );
      projectData( p4UserData1D, i );
      tree.Fill();
   }
}
