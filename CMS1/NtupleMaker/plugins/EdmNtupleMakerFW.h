#ifndef CMS1_EdmNtupleMakerFW_h
#define CMS1_EdmNtupleMakerFW_h
//
// Original Author: Dmytro Kovalskyi
//
// $Author: dmytro $
// $Date: 2007/08/30 14:16:15 $
// $Revision: 1.1 $
//
#include <vector>
#include "CMS1/NtupleMaker/interface/NtupleMaker.h"
#include "FWCore/Framework/interface/EDProducer.h"

namespace cms1 {
   class EdmNtupleMakerFW: public NtupleMaker, public edm::EDProducer
     {
      public:
	explicit EdmNtupleMakerFW( const edm::ParameterSet& iConfig )
	  { 
	     // configure default stuff (black boxes)
	     BaseAnalyzer::configure( iConfig );
	     
	     // user configuration
	     configure( iConfig );
	     
	     // register EDM products
	     // produces.
	     theData.registerProducts( this );
	  }
       	virtual ~EdmNtupleMakerFW(){}
      protected:
	// EDProducer stuff
	virtual void produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
	  {
	     // process default stuff (load data in the EventData)
	     BaseAnalyzer::processEvent( iEvent );
	     
	     // run user code
	     processEvent( iEvent );
	     
	     theData.putProducts( iEvent );
	     
	     finishEvent();
	  }
	virtual void endJob(){ finishProcessing(); }
	
	// What time of ntuples to create (EDM or plain ROOT)
	virtual bool isPlainRootNtuple() { return false; }

     };
}

#endif
