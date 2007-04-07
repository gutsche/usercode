#ifndef CMS1_CollectionContainer_h
#define CMS1_CollectionContainer_h

//
// Package:         CMS1/Base
// Class:           CollectionContainer
// 
// Description:     template classes for EventData collections
//
// Original Author: Dmytro Kovalskyi
//
// $Author: dmytro $
// $Date: 2007/03/16 07:00:21 $
// $Revision: 1.1 $
//
#include <vector>
#include "FWCore/ParameterSet/interface/InputTag.h"
namespace cms1 {
   // single collection info
   template <class T> struct Collection 
     {
	// pointer to collection data
	const std::vector<T>* theData;
	// collection label and instance according to CMSSW Framework definitions
	edm::InputTag theTag;
	// branch name for FWLite
	std::string theBranch;
	// unique collection identifier
	unsigned int theId;
	// data holder for FWLite (no use for the Framework). Don't mess with it.
	std::vector<T> theDataHolder;
	
	Collection(): theData(0), theId(0){}
	Collection( const edm::InputTag& tag, std::string branch )
	  : theData(0), theTag(tag), theBranch(branch), theId(0){}
     };
   
   // container for collections of the same type
   template <class T> class CollectionContainer 
     {
      public:
	// register a collection in a the container
	bool registerCollection( const edm::InputTag& tag,
				 const std::string& branch )
	  {
	     // return true if collection is new and false if an identical collection
	     // is registed already (not implemented yet)
	     theContainer.push_back( Collection<T>(tag,branch) );
	     return true;
	  }
	
	// various accessors to the collection
	const std::vector<T>* getCollection( const edm::InputTag& tag )
	  {
	     for ( typename std::vector<Collection<T> >::const_iterator collection = theContainer.begin();
		   collection != theContainer.end(); ++collection )
	       if ( collection->theTag.label() == tag.label() && collection->theTag.instance() == tag.instance() )
		 return collection->theData;
	     return 0;
	  }
	const std::vector<T>* getCollection( std::string branch )
	  {
	     for( typename std::vector<Collection<T> >::const_iterator collection = theContainer.begin();
		  collection != theContainer.end(); ++ collection )
	       if ( theContainer->theBranch == branch ) return theContainer->theData;
	     return 0;
	  }
	const std::vector<T>* getCollection( unsigned int id )
	  {
	     for( typename std::vector<Collection<T> >::const_iterator collection = theContainer.begin();
		  collection != theContainer.end(); ++ collection )
	       if ( theContainer->theId == id ) return theContainer->theData;
	     return 0;
	  }
	
	// various modifiers, returns true if succesfully modified collection pointer
	bool setCollection( const std::vector<T>* data, const edm::InputTag& tag )
	  {
	     for( typename std::vector<Collection<T> >::iterator collection = theContainer.begin();
		  collection != theContainer.end(); ++ collection )
	       if ( theContainer->theTag.label() == tag.label() && theContainer->theTag.instance() == tag.instance() )
		 {
		    theContainer->theData = data;
		    return true;
		 }
	     return false;
	  }
	bool setCollection( const std::vector<T>* data, std::string branch )
	  {
	     for( typename std::vector<Collection<T> >::iterator collection = theContainer.begin();
		  collection != theContainer.end(); ++ collection )
	       if ( theContainer->theBranch == branch )
		 {
		    theContainer->theData = data;
		    return true;
		 }
	     return false;
	  }
	bool setCollection( unsigned int id )
	  {
	     for( typename std::vector<Collection<T> >::iterator collection = theContainer.begin();
		  collection != theContainer.end(); ++ collection )
	       if ( theContainer->theId == id )
		 {
		    theContainer->theData = data;
		    return true;
		 }
	     return false;
	  }

	std::vector<Collection<T> > theContainer;
     };
}

   

#endif
