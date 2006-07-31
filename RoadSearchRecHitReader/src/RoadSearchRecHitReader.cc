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

#include <memory>
#include <string>

#include "Gutsche/RoadSearchRecHitReader/interface/RoadSearchRecHitReader.h"

#include "FWCore/Framework/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit2DLocalPosCollection.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit2DMatchedLocalPosCollection.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHitCollection.h"

namespace cms
{

  RoadSearchRecHitReader::RoadSearchRecHitReader(edm::ParameterSet const& conf) : 
    roadSearchRecHitReaderAlgorithm_(conf) ,
    conf_(conf)
  {
  }


  // Virtual destructor needed.
  RoadSearchRecHitReader::~RoadSearchRecHitReader() { }  

  // Functions that gets called by framework every event
  void RoadSearchRecHitReader::analyze(const edm::Event& e, const edm::EventSetup& es)
  {
    // retrieve producer name of input collections
    std::string stripRecHitProducer = conf_.getParameter<std::string>("StripRecHitProducer");
    std::string pixelRecHitProducer = conf_.getParameter<std::string>("PixelRecHitProducer");
  
    // get Inputs
    edm::Handle<SiStripRecHit2DMatchedLocalPosCollection> matchedRecHits;
    e.getByLabel(stripRecHitProducer,"matchedRecHit" ,matchedRecHits);
    edm::Handle<SiStripRecHit2DLocalPosCollection> rphiRecHits;
    e.getByLabel(stripRecHitProducer,"rphiRecHit" ,rphiRecHits);
    edm::Handle<SiStripRecHit2DLocalPosCollection> stereoRecHits;
    e.getByLabel(stripRecHitProducer,"stereoRecHit" ,stereoRecHits);

    // special treatment for getting pixel collection
    // if collection exists in file, use collection from file
    // if collection does not exist in file, create empty collection
    const SiPixelRecHitCollection *pixelRecHitCollection = 0;

    try {
      edm::Handle<SiPixelRecHitCollection> pixelRecHits;
      e.getByLabel(pixelRecHitProducer, pixelRecHits);
      pixelRecHitCollection = pixelRecHits.product();
    }
    catch (edm::Exception const& x) {
      if ( x.categoryCode() == edm::errors::ProductNotFound ) {
	if ( x.history().size() == 1 ) {
	  pixelRecHitCollection = new SiPixelRecHitCollection();
	}
      }
    }


    // Invoke the seed finding algorithm if pixelRecHitCollection is valid
    if ( pixelRecHitCollection != 0 ) {
      roadSearchRecHitReaderAlgorithm_.run(rphiRecHits.product(),
					   stereoRecHits.product(),
					   matchedRecHits.product(),
					   pixelRecHitCollection);
    } else {
      edm::LogError("RoadSearch") << "Pixel RecHit Collection was either not found in file or emtpy collection could not been created";
    }
  }

}
