#ifndef CMS1_BaseAnalyzerFW_h
#define CMS1_BaseAnalyzerFW_h

//
// Package:         CMS1/Base
// Class:           BaseAnalyzerFW
// 
// Description:     Base Framework analyzer
//
// Original Author: Dmytro Kovalskyi
//
// $Author: dmytro $
// $Date: 2007/03/16 07:00:21 $
// $Revision: 1.1 $
//
#include <vector>
#include "CMS1/BaseAnalyzer/interface/BaseAnalyzer.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "CMS1/Base/interface/EventData.h"

namespace cms1 {
   
   class BaseAnalyzerFW: public edm::EDAnalyzer
     {
      public:
	BaseAnalyzerFW():theDataFW(0){};
       	virtual ~BaseAnalyzerFW(){}
      protected:
	virtual void analyze(const edm::Event&, const edm::EventSetup&);
	template <class T> bool loadData(const edm::Event& iEvent, CollectionContainer<T>& container)
	  {
	     bool everythingIsLoaded = true;
	     for(typename std::vector<Collection<T> >::iterator collection = container.theContainer.begin();
		 collection != container.theContainer.end(); ++ collection)
	       {
		  edm::Handle<std::vector<T> > collectionHandle;
		  iEvent.getByLabel(collection->theTag, collectionHandle);
		  if ( collectionHandle.isValid() )
		    collection->theData = collectionHandle.product();
		  else 
		    {
		       std::cout << "WARNING: failed to load collection:\n" <<
			 "\tlabel: " << collection->theTag.label() <<
			 "\n\tinstance: " << collection->theTag.instance() <<
			 "\n\tbranch: " << collection->theBranch;
		       collection->theData = 0;
		       everythingIsLoaded = false;
		    }
	       }
	     return everythingIsLoaded;
	  }
	
	// pointer to the EventData
	EventData* theDataFW;
     };
}

   

#endif
