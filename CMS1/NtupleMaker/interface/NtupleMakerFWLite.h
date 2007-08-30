#ifndef CMS1_NtupleMakerFWLite_h
#define CMS1_NtupleMakerFWLite_h

//
// Package:         CMS1/NtupleMaker
// Class:           NtupleMakerFWLite
// 
// Description:     FWLite code, hardly anything should be done here
//
// Original Author: Dmytro Kovalskyi
//
// $Author: dmytro $
// $Date: 2007/05/11 04:20:12 $
// $Revision: 1.2 $
//
#include <vector>
#include "CMS1/NtupleMaker/interface/NtupleMaker.h"
#include "CMS1/BaseAnalyzer/interface/BaseAnalyzerFWLite.h"

namespace cms1 {
   class NtupleMakerFWLite: public NtupleMaker, public BaseAnalyzerFWLite
     {
      public:
	NtupleMakerFWLite(){}
       	virtual ~NtupleMakerFWLite(){}
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
