//
// Package:         OlisDebugProducers/RoadSearchSeedHitDumper
// Class:           RoadSearchSeedHitDumper
// 
// Description:     dumps global positions of Seed RecHits
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Wed Jul 19 18:20:00 UTC 2006
//
// $Author: burkett $
// $Date: 2006/03/15 16:07:16 $
// $Revision: 1.2 $
//

#include <memory>
#include <string>

#include "OlisDebugProducers/RoadSearchSeedHitDumper/interface/RoadSearchSeedHitDumper.h"

#include "FWCore/Framework/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit2DLocalPosCollection.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit2DMatchedLocalPosCollection.h"
#include "DataFormats/TrajectorySeed/interface/TrajectorySeedCollection.h"
#include "DataFormats/RecHitGlobalPosition/interface/RecHitGlobalPositionCollection.h"
#include "DataFormats/SeedHitsGlobalPositions/interface/SeedHitsGlobalPositionsCollection.h"

namespace cms
{

  RoadSearchSeedHitDumper::RoadSearchSeedHitDumper(edm::ParameterSet const& conf) : 
    roadSearchSeedHitDumperAlgorithm_(conf) ,
    conf_(conf)
  {
    produces<RecHitGlobalPositionCollection>("innerSeedHits");
    produces<RecHitGlobalPositionCollection>("outerSeedHits");
    produces<SeedHitsGlobalPositionsCollection>();
  }


  // Virtual destructor needed.
  RoadSearchSeedHitDumper::~RoadSearchSeedHitDumper() { }  

  // Functions that gets called by framework every event
  void RoadSearchSeedHitDumper::produce(edm::Event& e, const edm::EventSetup& es)
  {
    // retrieve producer name of input SeedCollection
    std::string seedProducer = conf_.getParameter<std::string>("SeedProducer");

    // Step A: Get Inputs 
    edm::Handle<TrajectorySeedCollection> seeds;
    e.getByLabel(seedProducer, seeds);

    // retrieve producer name of input SiStripRecHit2DLocalPosCollection
    std::string recHitProducer = conf_.getParameter<std::string>("RecHitProducer");
  
    // get Inputs
    edm::Handle<SiStripRecHit2DMatchedLocalPosCollection> matchedrecHits;
    e.getByLabel(recHitProducer,"matchedRecHit" ,matchedrecHits);
    edm::Handle<SiStripRecHit2DLocalPosCollection> rphirecHits;
    e.getByLabel(recHitProducer,"rphiRecHit" ,rphirecHits);


    // create empty output collections
    std::auto_ptr<RecHitGlobalPositionCollection>     innerSeedRecHits(new RecHitGlobalPositionCollection);
    std::auto_ptr<RecHitGlobalPositionCollection>     outerSeedRecHits(new RecHitGlobalPositionCollection);
    std::auto_ptr<SeedHitsGlobalPositionsCollection>  seedRecHits(new SeedHitsGlobalPositionsCollection);

    // Invoke the seed finding algorithm
    roadSearchSeedHitDumperAlgorithm_.run(seeds.product(),
					  rphirecHits.product(),
					  matchedrecHits.product(),
					  es,
					  *seedRecHits, 
					  *innerSeedRecHits, 
					  *outerSeedRecHits);

    // write output to file
    e.put(seedRecHits);
    e.put(innerSeedRecHits,"innerSeedHits");
    e.put(outerSeedRecHits,"outerSeedHits");

  }

}
