#ifndef CMS1_SampleAnalyzerFWLite_h
#define CMS1_SampleAnalyzerFWLite_h

//
// Package:         CMS1/Base
// Class:           SampleAnalyzerFWLite
// 
// Description:     FWLite code, hardly anything should be done here
//
// Original Author: Dmytro Kovalskyi
//
// $Author: dmytro $
// $Date: 2007/03/16 07:00:21 $
// $Revision: 1.1 $
//
#include <vector>
#include "CMS1/SampleAnalyzer/interface/SampleAnalyzer.h"
#include "CMS1/BaseAnalyzer/interface/BaseAnalyzerFWLite.h"

namespace cms1 {
   class SampleAnalyzerFWLite: public SampleAnalyzer, public BaseAnalyzerFWLite
     {
      public:
	SampleAnalyzerFWLite();
       	virtual ~SampleAnalyzerFWLite(){}
      protected:
	virtual void analyze( bool initialization = false );
	virtual void setup(const edm::ParameterSet& iConfig);
     };
}
   

#endif
