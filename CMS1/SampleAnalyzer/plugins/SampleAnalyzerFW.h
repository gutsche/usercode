#ifndef CMS1_SampleAnalyzerFW_h
#define CMS1_SampleAnalyzerFW_h

//
// Package:         CMS1/Base
// Class:           SampleAnalyzerFW
// 
// Description:     frame work modue, hardly anything should be done here
//
// Original Author: Dmytro Kovalskyi
//
// $Author: dmytro $
// $Date: 2007/04/17 05:03:39 $
// $Revision: 1.2 $
//
#include <vector>
#include "CMS1/SampleAnalyzer/interface/SampleAnalyzer.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

namespace cms1 {
   class SampleAnalyzerFW: public SampleAnalyzer, public edm::EDAnalyzer
     {
      public:
	explicit SampleAnalyzerFW( const edm::ParameterSet& iConfig )
	  { 
	     // configure default stuff (black boxes)
	     BaseAnalyzer::configure( iConfig );
	     
	     // user configuration
	     configure( iConfig ); 
	  }
       	virtual ~SampleAnalyzerFW(){}
      protected:
	// EDAnalyzer stuff
	virtual void analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
	  {
	     // process default stuff (load data in the EventData)
	     BaseAnalyzer::processEvent( iEvent );
	     
	     // run user code
	     processEvent( iEvent );
	  }
	virtual void endJob(){ finishProcessing(); }
     };
}
   

#endif
