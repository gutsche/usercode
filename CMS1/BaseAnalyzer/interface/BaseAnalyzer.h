#ifndef CMS1_BaseAnalyzer_h
#define CMS1_BaseAnalyzer_h

//
// Package:         CMS1/BaseAnalyzer
// Class:           BaseAnalyzer
// 
// Description:     abstract base class for TnS analysis
//
// Original Author: Dmytro Kovalskyi
//
// $Author: dmytro $
// $Date: 2007/03/16 07:00:21 $
// $Revision: 1.1 $
//
#include <vector>
#include "CMS1/Base/interface/EventData.h"
#include "CMS1/Muons/interface/Muons.h"
#include "CMS1/Electrons/interface/Electrons.h"
#include "CMS1/Jets/interface/Jets.h"
#include "CMS1/MET/interface/MET.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

namespace cms1 {
   
   class BaseAnalyzer 
     {
      public:
	BaseAnalyzer(){}
	virtual ~BaseAnalyzer(){}
      protected:
	// analyzer configuration code
	virtual void configure(const edm::ParameterSet& );
	
	// process event using EventData
	virtual void processEvent() = 0;
	
	// event data container
	EventData theData;
	
	// black boxes
	Muons         theMuons;
	Electrons     theElectrons;
	Jets          theJets;
	MET           theMET;
     };
}

   

#endif
