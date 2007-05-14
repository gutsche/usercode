#ifndef CMS1_testMCInfoFW_h
#define CMS1_testMCInfoFW_h

//
// Package:         CMS1/MCInfo
// Class:           testMCInfoFW
// 
// Description:     frame work modue, hardly anything should be done here
//
// Original Author: Matteo Sani
//

#include "CMS1/MCInfo/interface/testMCInfo.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

namespace cms1 {
  class testMCInfoFW: public testMCInfo, public edm::EDAnalyzer {
  public:
    explicit testMCInfoFW(const edm::ParameterSet& iConfig) { 
	     // configure default stuff (black boxes)
      BaseAnalyzer::configure(iConfig);

      // user configuration
      configure(iConfig); 
	  }
    virtual ~testMCInfoFW(){}

  protected:
    // EDAnalyzer stuff
    virtual void analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

      // process default stuff (load data in the EventData)
      BaseAnalyzer::processEvent(iEvent);

      // run user code
      processEvent(iEvent);

	  }
    virtual void endJob(){ finishProcessing(); }
  };
}
   

#endif
