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
// $Date: 2007/04/08 22:55:37 $
// $Revision: 1.2 $
//
#include <vector>
#include "CMS1/BaseAnalyzer/interface/BaseAnalyzer.h"

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
	// User configuration code lives here
	// Base class is called first externally to get
	// black boxes setup
	virtual void configure(const edm::ParameterSet&);
	
	// User code to process event lives here
	// Base class is called first externally to get
	// black boxes running
	virtual void processEvent(const edm::Event&);
      
      private:
	// analysis variables
	double pt_;              //usercode
	unsigned int number_;    //usercode
     };
}

   

#endif
