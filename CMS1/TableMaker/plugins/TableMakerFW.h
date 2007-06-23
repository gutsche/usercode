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
// $Author: kalavase $
// $Date: 2007/06/13 18:14:38 $
// $Revision: 1.3 $
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
	     
	     theData.trackAssociator = new TrackDetectorAssociator();
	     theData.trackAssociator->theEBRecHitCollectionLabel = edm::InputTag("ecalRecHit:EcalRecHitsEB");
	     theData.trackAssociator->theEERecHitCollectionLabel = edm::InputTag("ecalRecHit:EcalRecHitsEE");
	     theData.trackAssociator->theCaloTowerCollectionLabel = edm::InputTag("towerMaker");
	     theData.trackAssociator->theHBHERecHitCollectionLabel = edm::InputTag("hbhereco");
	     theData.trackAssociator->theHORecHitCollectionLabel = edm::InputTag("horeco");
	     theData.trackAssociator->theDTRecSegment4DCollectionLabel = edm::InputTag("dt4DSegmets");
	     theData.trackAssociator->theCSCSegmentCollectionLabel = edm::InputTag("cscSegments");
	     theData.trackAssociator->useDefaultPropagator();

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
	     theData.iSetup = &iSetup;
	     processEvent( iEvent );
	     finishEvent();
	  }
	virtual void endJob(){ finishProcessing(); }
     };
}
   

#endif
