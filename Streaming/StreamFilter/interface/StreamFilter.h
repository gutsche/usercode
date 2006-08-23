#ifndef StreamFilter_h
#define StreamFilter_h

//
// Package:         Streaming/StreamFilter
// Class:           StreamFilter
// 
// Description:     Filter on streaming decision
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Tue Aug 22 21:33:57 UTC 2006
//
// $Author: gutsche $
// $Date: 2006/03/28 23:12:10 $
// $Revision: 1.1 $
//

#include <string>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

namespace cms
{
  class StreamFilter : public edm::EDFilter {
  public:
    explicit StreamFilter(const edm::ParameterSet&);
    ~StreamFilter();
    
  private:
    virtual void beginJob(const edm::EventSetup&) ;
    virtual bool filter(edm::Event&, const edm::EventSetup&);
    virtual void endJob() ;

    std::string mode_;
    std::string src_;
    
  };
}

#endif
