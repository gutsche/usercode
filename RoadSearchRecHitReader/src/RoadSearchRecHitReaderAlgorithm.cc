//
// Package:         Gutsche/RoadSearchRecHitReader
// Class:           RoadSearchRecHitReader
// 
// Description:     reads all 4 RecHit collections independent of their existence in the input file
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Mon Jul 31 00:58:38 UTC 2006
//
// $Author: gutsche $
// $Date: 2006/07/20 21:59:14 $
// $Revision: 1.1 $
//

#include <vector>

#include "Gutsche/RoadSearchRecHitReader/interface/RoadSearchRecHitReaderAlgorithm.h"

#include "FWCore/Framework/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit2DLocalPos.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit2DMatchedLocalPos.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHit.h"

RoadSearchRecHitReaderAlgorithm::RoadSearchRecHitReaderAlgorithm(const edm::ParameterSet& conf) : conf_(conf) { 
}

RoadSearchRecHitReaderAlgorithm::~RoadSearchRecHitReaderAlgorithm() {
}

void RoadSearchRecHitReaderAlgorithm::run(const SiStripRecHit2DLocalPosCollection* rphiRecHits,
					  const SiStripRecHit2DLocalPosCollection* stereoRecHits,
					  const SiStripRecHit2DMatchedLocalPosCollection* matchedRecHits,
					  const SiPixelRecHitCollection* pixelRecHits)
{

  edm::LogInfo("RoadSearch") << "Number of rphi strip RecHits: " << rphiRecHits->size();
  edm::LogInfo("RoadSearch") << "Number of stereo strip RecHits: " << stereoRecHits->size();
  edm::LogInfo("RoadSearch") << "Number of matched strip RecHits: " << matchedRecHits->size();
  edm::LogInfo("RoadSearch") << "Number of pixel RecHits: " << pixelRecHits->size();

};
