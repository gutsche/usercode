#ifndef RoadSearchRecHitReader_h
#define RoadSearchRecHitReader_h

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

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Handle.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "DataFormats/Common/interface/EDProduct.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "Gutsche/RoadSearchRecHitReader/interface/RoadSearchRecHitReaderAlgorithm.h"

namespace cms
{
  class RoadSearchRecHitReader : public edm::EDAnalyzer
  {
  public:

    explicit RoadSearchRecHitReader(const edm::ParameterSet& conf);

    virtual ~RoadSearchRecHitReader();

    virtual void analyze(const edm::Event& e, const edm::EventSetup& c);

  private:
    RoadSearchRecHitReaderAlgorithm roadSearchRecHitReaderAlgorithm_;
    edm::ParameterSet conf_;

  };
}


#endif
