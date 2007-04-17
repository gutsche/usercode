#ifndef CMS1_SampleAnalyzerFWLite_h
#define CMS1_SampleAnalyzerFWLite_h

//
// Package:         CMS1/Base
// Class:           SampleAnalyzerFWLite
// 
// Description:     FWLite code, hardly anything should be done here
//
// Original Author: Dmytro Kovalskyi
//
// $Author: dmytro $
// $Date: 2007/04/07 17:57:45 $
// $Revision: 1.1 $
//
#include <vector>
#include "CMS1/SampleAnalyzer/interface/SampleAnalyzer.h"
#include "CMS1/BaseAnalyzer/interface/BaseAnalyzerFWLite.h"

namespace cms1 {
   class SampleAnalyzerFWLite: public SampleAnalyzer, public BaseAnalyzerFWLite
     {
      public:
	SampleAnalyzerFWLite(){}
       	virtual ~SampleAnalyzerFWLite(){}
      protected:
	// BaseAnalyzerFWLite stuff
	virtual void process(const edm::Event& iEvent){
	   // process default stuff (load data in the EventData)
	   BaseAnalyzer::processEvent( iEvent );
	   
	   // run user code
	   processEvent( iEvent );
	}
	virtual void setup(const edm::ParameterSet& iConfig) {
	   // configure default stuff (black boxes)
	   BaseAnalyzer::configure( iConfig );
	     
	   // user configuration
	   configure( iConfig ); 
	}
	virtual void terminate( TList& ){ finishProcessing(); }
	
     };
}
   

#endif
