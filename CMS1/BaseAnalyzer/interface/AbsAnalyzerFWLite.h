#ifndef CMS1_AbsAnalyzerFWLite_h
#define CMS1_AbsAnalyzerFWLite_h

//
// Package:         CMS1/BaseAnalyzer
// Class:           AbsAnalyzerFWLite

// 
// Description:     FWLite analyzer abstract class providing the most basic ROOT related support
//
// Original Author: Dmytro Kovalskyi
//
// $Author: dmytro $
// $Date: 2007/02/16 11:42:00 $
// $Revision: 1.1 $
//

#include "TChain.h"
#include "TString.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "CMS1/Base/interface/EventData.h"

namespace cms1 {

   class AbsAnalyzerFWLite
     {
      public:
	AbsAnalyzerFWLite(){}
	virtual ~AbsAnalyzerFWLite(){}
	virtual void processData(TString configFile);
      protected:
	virtual void setup(const edm::ParameterSet& iConfig) = 0;
	virtual void analyze( bool initialization = false ) = 0;
	// load data in the EventData. If initialization is true - a special call with no data loaded
	// check this flag before you use your data. This cumbersome requirement allows to minimize
	// number of places where you need to list class containers (instantiation of templates). 
	template <class T> bool loadData(CollectionContainer<T>& container, bool initialization = false)
	  {
	     bool everythingIsLoaded = true;
	     for(typename std::vector<Collection<T> >::iterator collection = container.theContainer.begin();
		 collection != container.theContainer.end(); ++ collection)
	       {
		  if ( theChain->GetBranch((collection->theBranch+".obj").c_str()) ) // check whether such branch exists
		    {
		       if (initialization) {
			  //activate relevant branches
			  theChain->SetBranchStatus((collection->theBranch+"*").c_str(),1);
		       }
		       
		       theChain->SetBranchAddress((collection->theBranch+".obj").c_str(), &(collection->theDataHolder));
		       collection->theData = &(collection->theDataHolder);
		    }
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
	

	// TTree* theChain;
	TChain* theChain;
	int maxEvents;
	int skipEvents;
     };
}

#endif
