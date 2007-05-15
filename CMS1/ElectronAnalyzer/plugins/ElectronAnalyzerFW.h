#ifndef ElectronAnalyzerFW_h
#define ElectronAnalyzerFW_h

//
// Package:         CMS1/ElectronAnalyzer
// Class:           ElectronAnalyzerFW
// 
//
// Original Author: Matteo Sani
//

#include "CMS1/ElectronAnalyzer/interface/ElectronAnalyzer.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

namespace cms1 {
  class ElectronAnalyzerFW: public ElectronAnalyzer, public edm::EDAnalyzer {
  public:
    explicit ElectronAnalyzerFW(const edm::ParameterSet& iConfig) { 
	     // configure default stuff (black boxes)
      BaseAnalyzer::configure(iConfig);

      // user configuration
      configure(iConfig); 
	  }
    virtual ~ElectronAnalyzerFW(){}

  protected:
    // EDAnalyzer stuff
    virtual void analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

      // process default stuff (load data in the EventData)
      BaseAnalyzer::processEvent(iEvent);

      // run user code
      processEvent(iEvent);
      
      finishEvent();

	  }
    virtual void endJob(){ finishProcessing(); }
  };
}
   
#endif
