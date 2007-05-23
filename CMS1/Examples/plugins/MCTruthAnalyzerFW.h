#ifndef CMS1_MCTruthAnalyzerFW_h
#define CMS1_MCTruthAnalyzerFW_h

//
// Package:         CMS1/CommonTools
// Class:           MCTruthAnalyzerFW
// 
// Description:     frame work module, hardly anything should be done here
//
// Original Author: Matteo Sani
//
#include <vector>
#include "CMS1/Examples/interface/MCTruthAnalyzer.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

namespace cms1 {
  class MCTruthAnalyzerFW: public MCTruthAnalyzer, public edm::EDAnalyzer {
  public:
    explicit MCTruthAnalyzerFW(const edm::ParameterSet& iConfig) { 
      // configure default stuff (black boxes)
      BaseAnalyzer::configure(iConfig);
	     
      // user configuration
      configure( iConfig ); 
	  }
    virtual ~MCTruthAnalyzerFW(){}
  protected:
    // EDAnalyzer stuff
    virtual void analyze(const edm::Event& iEvent, const edm::EventSetup&) {
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
