#ifndef RoadSearchSeedHitDumper_h
#define RoadSearchSeedHitDumper_h

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

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Handle.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "DataFormats/Common/interface/EDProduct.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "OlisDebugProducers/RoadSearchSeedHitDumper/interface/RoadSearchSeedHitDumperAlgorithm.h"

namespace cms
{
  class RoadSearchSeedHitDumper : public edm::EDProducer
  {
  public:

    explicit RoadSearchSeedHitDumper(const edm::ParameterSet& conf);

    virtual ~RoadSearchSeedHitDumper();

    virtual void produce(edm::Event& e, const edm::EventSetup& c);

  private:
    RoadSearchSeedHitDumperAlgorithm roadSearchSeedHitDumperAlgorithm_;
    edm::ParameterSet conf_;

  };
}


#endif
