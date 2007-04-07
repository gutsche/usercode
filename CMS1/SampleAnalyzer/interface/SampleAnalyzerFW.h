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
// $Date: 2007/03/16 07:00:21 $
// $Revision: 1.1 $
//
#include <vector>
#include "CMS1/SampleAnalyzer/interface/SampleAnalyzer.h"
#include "CMS1/BaseAnalyzer/interface/BaseAnalyzerFW.h"

namespace cms1 {
   class SampleAnalyzerFW: public SampleAnalyzer, public BaseAnalyzerFW
     {
      public:
	explicit SampleAnalyzerFW( const edm::ParameterSet& iConfig )
	  { 
	     theDataFW = &theData;
	     configure(iConfig); 
	  }
       	virtual ~SampleAnalyzerFW(){}
      protected:
	virtual void analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
	  {
	     // process default stuff (load data in the EventData)
	     BaseAnalyzerFW::analyze( iEvent, iSetup );
	     
	     // run user code
	     processEvent();
	  }
     };
}
   

#endif
