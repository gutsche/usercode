#ifndef CMS1_TableMakerFWLite_h
#define CMS1_TableMakerFWLite_h

//
// Package:         CMS1/TableMaker
// Class:           TableMakerFWLite
// 
// Description:     FWLite code, hardly anything should be done here
//
// Original Author: Dmytro Kovalskyi
//
// $Author: dmytro $
// $Date: 2007/04/17 05:06:25 $
// $Revision: 1.1 $
//
#include <vector>
#include "CMS1/TableMaker/interface/TableMaker.h"
#include "CMS1/BaseAnalyzer/interface/BaseAnalyzerFWLite.h"

namespace cms1 {
   class TableMakerFWLite: public TableMaker, public BaseAnalyzerFWLite
     {
      public:
	TableMakerFWLite(){}
       	virtual ~TableMakerFWLite(){}
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
