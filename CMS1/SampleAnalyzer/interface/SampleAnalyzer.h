#ifndef CMS1_SampleAnalyzer_h
#define CMS1_SampleAnalyzer_h

//
// Package:         CMS1/SampleAnalyzer
// Class:           SampleAnalyzer
// 
// Description:     working example of analysis in the T'n'S framework
//
// Original Author: Dmytro Kovalskyi
//
// $Author: dmytro $
// $Date: 2007/04/07 17:57:45 $
// $Revision: 1.1 $
//
#include <vector>
#include "CMS1/BaseAnalyzer/interface/BaseAnalyzer.h"
#include "CMS1/BaseAnalyzer/interface/BaseAnalyzerFW.h"

namespace cms1 {
   
   class SampleAnalyzer: public BaseAnalyzer
     {
      public:
	SampleAnalyzer(){
	   pt_=0;               //usercode
	   number_=0;           //usercode
	}
	virtual ~SampleAnalyzer(){}
      protected:
	// analyzer configuration code
	virtual void configure(const edm::ParameterSet& iConfig);
	
	// process event using EventData
	virtual void processEvent();
      
      private:
	// analysis variables
	double pt_;              //usercode
	unsigned int number_;    //usercode
     };
}

   

#endif
