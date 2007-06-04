#ifndef CMS1_ElectronIdFW_h
#define CMS1_ElectronIdFW_h

//
// Package:         CMS1/ElectronAnalyzer
// Class:           ElectronIdFW
// 
// Description:     frame work modue, hardly anything should be done here
//
// Original Author: Matteo Sani
//
// $Author: sani $
//
#include <vector>
#include "CMS1/ElectronAnalyzer/interface/ElectronId.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

namespace cms1 {
   class ElectronIdFW: public ElectronId, public edm::EDAnalyzer {
   public:
     explicit ElectronIdFW( const edm::ParameterSet& iConfig ) { 
	     // configure default stuff (black boxes)
	     BaseAnalyzer::configure( iConfig );
	     
	     // user configuration
	     configure( iConfig ); 
     }
     virtual ~ElectronIdFW(){}
   protected:
     // EDAnalyzer stuff
     virtual void analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
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
