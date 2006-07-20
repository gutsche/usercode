#ifndef RoadSearchSeedHitDumper_h
#define RoadSearchSeedHitDumper_h

//
// Package:         Gutsche/RoadSearchSeedHitDumper
// Class:           RoadSearchSeedHitDumper
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

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Handle.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "DataFormats/Common/interface/EDProduct.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "Gutsche/RoadSearchSeedHitDumper/interface/RoadSearchSeedHitDumperAlgorithm.h"

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
