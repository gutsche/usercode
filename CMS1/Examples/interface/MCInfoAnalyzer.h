#ifndef MCInfoAnalyzer_h
#define MCInfoAnalyzer_h

//
// Package:         CMS1/Examples
// Class:           MCInfoAnalyze
// 

#include "CMS1/BaseAnalyzer/interface/BaseAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include "CMS1/MCInfo/interface/MCInfo.h"
#include "CMS1/Base/interface/Cuts.h"

namespace cms1 {

  class MCInfoAnalyzer : public BaseAnalyzer {
  public:
    MCInfoAnalyzer() {}
    virtual ~MCInfoAnalyzer() {}

  protected:
    virtual void configure(const edm::ParameterSet&);
    virtual void processEvent(const edm::Event&);
    
  private:
    double ept_min, mpt_min, tpt_min, jet_min;
  };
}

#endif
