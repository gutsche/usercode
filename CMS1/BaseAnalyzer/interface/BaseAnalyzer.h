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
// $Date: 2007/04/07 17:54:23 $
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
	// User configuration code 
	// In this class basic stuff is setup (black boxes)
	virtual void configure(const edm::ParameterSet& );
	
	// User code to process event 
	// In this class basic stuff is done (black boxes)
	virtual void processEvent(const edm::Event& );

	// User code to finilize data processing
	virtual void finishProcessing(){}
	
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
