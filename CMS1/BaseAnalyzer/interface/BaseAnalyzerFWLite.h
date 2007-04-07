#ifndef CMS1_BaseAnalyzerFWLite_h
#define CMS1_BaseAnalyzerFWLite_h

//
// Package:         CMS1/BaseAnalyzer
// Class:           BaseAnalyzerFWLite

// 
// Description:     base FWLite analyzer
//
// Original Author: Dmytro Kovalskyi
//
// $Author: dmytro $
// $Date: 2007/02/16 11:42:00 $
// $Revision: 1.1 $
//
#include "CMS1/BaseAnalyzer/interface/AbsAnalyzerFWLite.h"
#include "CMS1/Base/interface/EventData.h"

namespace cms1 {

   class BaseAnalyzerFWLite: public AbsAnalyzerFWLite
     {
      public:
	BaseAnalyzerFWLite():theDataFW(0){}
	virtual ~BaseAnalyzerFWLite(){}
      protected:
	virtual void analyze( bool initialization = false );
	
	// pointer to the EventData
	EventData* theDataFW;
     };
}

#endif
