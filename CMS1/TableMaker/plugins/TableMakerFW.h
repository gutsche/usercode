#ifndef CMS1_TableMakerFW_h
#define CMS1_TableMakerFW_h

//
// Package:         CMS1/Base
// Class:           TableMakerFW
// 
// Description:     frame work modue, hardly anything should be done here
//
// Original Author: Dmytro Kovalskyi
//
// $Author: dmytro $
// $Date: 2007/05/11 04:20:12 $
// $Revision: 1.2 $
//
#include <vector>
#include "CMS1/TableMaker/interface/TableMaker.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

namespace cms1 {
   class TableMakerFW: public TableMaker, public edm::EDAnalyzer
     {
      public:
	explicit TableMakerFW( const edm::ParameterSet& iConfig )
	  { 
	     // configure default stuff (black boxes)
	     BaseAnalyzer::configure( iConfig );
	     
	     // user configuration
	     configure( iConfig ); 
	  }
       	virtual ~TableMakerFW(){}
      protected:
	// EDAnalyzer stuff
	virtual void analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
	  {
	     // process default stuff (load data in the EventData)
	     BaseAnalyzer::processEvent( iEvent );
	     
	     // run user code
	     //Breaking FWLite for now by adding iSetup to fix MET for muons - PDK
	     processEvent( iEvent, iSetup);
	     finishEvent();
	  }
	virtual void endJob(){ finishProcessing(); }
     };
}
   

#endif
