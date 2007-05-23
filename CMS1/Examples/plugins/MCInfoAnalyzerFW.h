#ifndef CMS1_MCInfoAnalyzerFW_h
#define CMS1_MCInfoAnalyzerFW_h

//
// Package:         CMS1/MCInfo
// Class:           MCInfoAnalyzertestMCInfoFW
// 
// Description:     frame work modue, hardly anything should be done here
//
// Original Author: Matteo Sani
//

#include "CMS1/Examples/interface/MCInfoAnalyzer.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

namespace cms1 {
  class MCInfoAnalyzerFW: public MCInfoAnalyzer, public edm::EDAnalyzer {
  public:
    explicit MCInfoAnalyzerFW(const edm::ParameterSet& iConfig) { 
	     // configure default stuff (black boxes)
      BaseAnalyzer::configure(iConfig);

      // user configuration
      configure(iConfig); 
	  }
    virtual ~MCInfoAnalyzerFW(){}

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
