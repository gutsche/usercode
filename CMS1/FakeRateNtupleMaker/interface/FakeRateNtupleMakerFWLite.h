#ifndef CMS1_FakeRateNtupleMakerFWLite_h
#define CMS1_FakeRateNtupleMakerFWLite_h

//
// Package:         CMS1/FakeRateNtupleMaker
// Class:           FakeRateNtupleMakerFWLite
// 
// Description:     FWLite code, hardly anything should be done here
//
// Original Author: Dmytro Kovalskyi
//
// $Author: dmytro $
// $Date: 2007/08/30 14:16:14 $
// $Revision: 1.1 $
//
#include <vector>
#include "CMS1/FakeRateNtupleMaker/interface/FakeRateNtupleMaker.h"
#include "CMS1/BaseAnalyzer/interface/BaseAnalyzerFWLite.h"

namespace cms1 {
   class FakeRateNtupleMakerFWLite: public FakeRateNtupleMaker, public BaseAnalyzerFWLite
     {
      public:
	FakeRateNtupleMakerFWLite(){}
       	virtual ~FakeRateNtupleMakerFWLite(){}
      protected:
	// BaseAnalyzerFWLite stuff
	virtual void process(const edm::Event& iEvent){
	   // process default stuff (load data in the EventData)
	   BaseAnalyzer::processEvent( iEvent );
	   
	   // run user code
	   processEvent( iEvent );
	   finishEvent();
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
