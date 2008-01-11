#ifndef CMS1_FakeRateNtupleMakerFW_h
#define CMS1_FakeRateNtupleMakerFW_h

//
// Package:         CMS1/Base
// Class:           FakeRateNtupleMakerFW
// 
// Description:     frame work modue, hardly anything should be done here
//
// Original Author: Dmytro Kovalskyi
//
// $Author: dmytro $
// $Date: 2007/08/30 14:16:15 $
// $Revision: 1.1 $
//
#include <vector>
#include "CMS1/FakeRateNtupleMaker/interface/FakeRateNtupleMaker.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"


namespace cms1 {
   class FakeRateNtupleMakerFW: public FakeRateNtupleMaker, public edm::EDAnalyzer
     {
      public:
	explicit FakeRateNtupleMakerFW( const edm::ParameterSet& iConfig )
	  { 
	     // configure default stuff (black boxes)
	     BaseAnalyzer::configure( iConfig );
	     
	     // user configuration
	     configure( iConfig );
	  }
       	virtual ~FakeRateNtupleMakerFW(){}
      protected:
	// EDAnalyzer stuff
	virtual void analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
	  {
	     // process default stuff (load data in the EventData)
	     BaseAnalyzer::processEvent( iEvent );
	     
	     // run user code
	     processEvent( iEvent );
	     finishEvent();
	  }
	virtual void endJob(){ finishProcessing(); }
     };
}
   

#endif
