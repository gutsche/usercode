//
// Package:         Gutsche/RoadSearchSeedHitDumper
// Class:           RoadSearchSeedHitDumperAlgorithm
// 
// Description:     dumps global positions of Seed RecHits
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Wed Jul 19 18:20:00 UTC 2006
//
// $Author: gutsche $
// $Date: 2006/07/19 23:20:31 $
// $Revision: 1.1 $
//

#include <vector>

#include "Gutsche/RoadSearchSeedHitDumper/interface/RoadSearchSeedHitDumperAlgorithm.h"

#include "FWCore/Framework/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit2DLocalPos.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit2DMatchedLocalPos.h"
#include "DataFormats/TrajectorySeed/interface/TrajectorySeed.h"
#include "Gutsche/RecHitGlobalPosition/interface/RecHitGlobalPosition.h"
#include "Gutsche/SeedHitsGlobalPositions/interface/SeedHitsGlobalPositions.h"

#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/Vector/interface/GlobalPoint.h"
#include "Geometry/Vector/interface/LocalPoint.h"
#include "RecoTracker/RoadMapRecord/interface/Roads.h"

#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"

RoadSearchSeedHitDumperAlgorithm::RoadSearchSeedHitDumperAlgorithm(const edm::ParameterSet& conf) : conf_(conf) { 
}

RoadSearchSeedHitDumperAlgorithm::~RoadSearchSeedHitDumperAlgorithm() {
}

void RoadSearchSeedHitDumperAlgorithm::run(const TrajectorySeedCollection* seeds,
					   const SiStripRecHit2DLocalPosCollection* rphiRecHits,
					   const SiStripRecHit2DMatchedLocalPosCollection* matchedRecHits,
					   const edm::EventSetup& es,
					   SeedHitsGlobalPositionsCollection &seedHits,
					   RecHitGlobalPositionCollection &innerSeedRecHits,
					   RecHitGlobalPositionCollection &outerSeedRecHits )
{

  // get vectors of available DetId's in rechit collection
  const std::vector<DetId> availableRPhiIDs    = rphiRecHits->ids();
  const std::vector<DetId> availableMatchedIDs = matchedRecHits->ids();


  // get roads
  edm::ESHandle<Roads> roads;
  es.get<TrackerDigiGeometryRecord>().get(roads);

  // get tracker geometry
  edm::ESHandle<TrackerGeometry> tracker;
  es.get<TrackerDigiGeometryRecord>().get(tracker);

  // extract global positions of RecHits in seed rings
  // loop over seed Ring pairs
  for ( Roads::const_iterator road = roads->begin(); road != roads->end(); ++road ) {
    Roads::RoadSeed seed = (*road).first;
    
    // loop over detid's in inner seed rings
    for ( Ring::const_iterator innerRingDetId = seed.first.begin(); innerRingDetId != seed.first.end(); ++innerRingDetId ) {
      // continue if detid is found in available detid's of matched rechit collection
      if ( availableMatchedIDs.end() != std::find(availableMatchedIDs.begin(),availableMatchedIDs.end(),innerRingDetId->second) ) {
	// get hits for detid
	SiStripRecHit2DMatchedLocalPosCollection::range innerSeedDetHits = matchedRecHits->get(innerRingDetId->second);
	// loop over inner dethits
	for ( SiStripRecHit2DMatchedLocalPosCollection::const_iterator innerSeedDetHit = innerSeedDetHits.first;
	      innerSeedDetHit != innerSeedDetHits.second; ++innerSeedDetHit ) {
	  GlobalPoint inner = tracker->idToDet(innerSeedDetHit->geographicalId())->surface().toGlobal(innerSeedDetHit->localPosition());
	  innerSeedRecHits.push_back(RecHitGlobalPosition(inner));
	}
      }
    }

    // loop over detid's in outer seed rings
    for ( Ring::const_iterator outerRingDetId = seed.second.begin(); outerRingDetId != seed.second.end(); ++outerRingDetId ) {
      // continue if detid is found in available detid's of rphi rechit collection
      if ( availableRPhiIDs.end() != std::find(availableRPhiIDs.begin(),availableRPhiIDs.end(),outerRingDetId->second) ) {
	// get hits for detid
	SiStripRecHit2DLocalPosCollection::range outerSeedDetHits = rphiRecHits->get(outerRingDetId->second);
	// loop over outer dethits
	for ( SiStripRecHit2DLocalPosCollection::const_iterator outerSeedDetHit = outerSeedDetHits.first;
	      outerSeedDetHit != outerSeedDetHits.second; ++outerSeedDetHit ) {
	  GlobalPoint outer = tracker->idToDet(outerSeedDetHit->geographicalId())->surface().toGlobal(outerSeedDetHit->localPosition());
	  outerSeedRecHits.push_back(RecHitGlobalPosition(outer));
	}
      }
    }
  }

  // loop over seeds
  for ( TrajectorySeedCollection::const_iterator seed = seeds->begin(); seed != seeds->end(); ++seed) {

    // get DetIds of SiStripRecHit2DLocalPos of Seed, assuming first is hit from inner SeedRing, second is hit from outer SeedRing
    if ( seed->nHits() < 2 ) {
      edm::LogError("RoadWarning") << "Seed has less then two linked TrackingRecHit, do not consider this seed.";
    } else {

      const TrackingRecHit *innerSeedRingHit = &(*(seed->recHits().first));
      const TrackingRecHit *outerSeedRingHit = &(*(--(seed->recHits().second)));

      // get global positions of the hits
      GlobalPoint innerSeedHitGlobalPosition = tracker->idToDet(innerSeedRingHit->geographicalId())->surface().toGlobal(innerSeedRingHit->localPosition());
      GlobalPoint outerSeedHitGlobalPosition = tracker->idToDet(outerSeedRingHit->geographicalId())->surface().toGlobal(outerSeedRingHit->localPosition());

      seedHits.push_back(SeedHitsGlobalPositions(innerSeedHitGlobalPosition,outerSeedHitGlobalPosition));
    }
  }

};
