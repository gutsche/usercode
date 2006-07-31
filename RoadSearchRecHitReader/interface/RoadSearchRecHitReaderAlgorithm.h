#ifndef RoadSearchRecHitReaderAlgorithm_h
#define RoadSearchRecHitReaderAlgorithm_h

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

#include <string>

#include "FWCore/Framework/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit2DLocalPosCollection.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit2DMatchedLocalPosCollection.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHitCollection.h"

class RoadSearchRecHitReaderAlgorithm 
{
 public:
  
  RoadSearchRecHitReaderAlgorithm(const edm::ParameterSet& conf);
  ~RoadSearchRecHitReaderAlgorithm();

  /// Runs the algorithm
  void run(const SiStripRecHit2DLocalPosCollection* rphiRecHits,
	   const SiStripRecHit2DLocalPosCollection* stereoRecHits,
	   const SiStripRecHit2DMatchedLocalPosCollection* matchedRecHits,
	   const SiPixelRecHitCollection* pixelRecHits);

 private:
  edm::ParameterSet conf_;

};

#endif
