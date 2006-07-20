#ifndef RoadSearchSeedHitDumperAlgorithm_h
#define RoadSearchSeedHitDumperAlgorithm_h

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

#include <string>

#include "FWCore/Framework/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "DataFormats/TrajectorySeed/interface/TrajectorySeedCollection.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit2DLocalPosCollection.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit2DMatchedLocalPosCollection.h"
#include "Gutsche/RecHitGlobalPosition/interface/RecHitGlobalPositionCollection.h"
#include "Gutsche/SeedHitsGlobalPositions/interface/SeedHitsGlobalPositionsCollection.h"

class RoadSearchSeedHitDumperAlgorithm 
{
 public:
  
  RoadSearchSeedHitDumperAlgorithm(const edm::ParameterSet& conf);
  ~RoadSearchSeedHitDumperAlgorithm();

  /// Runs the algorithm
  void run(const TrajectorySeedCollection* seeds,
	   const SiStripRecHit2DLocalPosCollection* rphiRecHits,
	   const SiStripRecHit2DMatchedLocalPosCollection* matchedRecHits,
	   const edm::EventSetup& es,
	   SeedHitsGlobalPositionsCollection &seedhits,
	   RecHitGlobalPositionCollection &innerSeedRecHits,
	   RecHitGlobalPositionCollection &outerSeedRecHits );

 private:
  edm::ParameterSet conf_;

};

#endif
