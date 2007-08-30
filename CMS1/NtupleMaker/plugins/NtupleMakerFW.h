#ifndef CMS1_NtupleMakerFW_h
#define CMS1_NtupleMakerFW_h

//
// Package:         CMS1/Base
// Class:           NtupleMakerFW
// 
// Description:     frame work modue, hardly anything should be done here
//
// Original Author: Dmytro Kovalskyi
//
// $Author: dmytro $
// $Date: 2007/08/04 22:13:07 $
// $Revision: 1.5 $
//
#include <vector>
#include "CMS1/NtupleMaker/interface/NtupleMaker.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"


namespace cms1 {
   class NtupleMakerFW: public NtupleMaker, public edm::EDAnalyzer
     {
      public:
	explicit NtupleMakerFW( const edm::ParameterSet& iConfig )
	  { 
	     // configure default stuff (black boxes)
	     BaseAnalyzer::configure( iConfig );
	     
	     // user configuration
	     configure( iConfig );
	  }
       	virtual ~NtupleMakerFW(){}
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
