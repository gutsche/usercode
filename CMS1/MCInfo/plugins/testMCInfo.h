#ifndef testMCInfo_h
#define testMCInfo_h

//
// Package:         CMS1/MCInfo
// Class:           testMCInfo
// 

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include "CMS1/MCInfo/interface/MCInfo.h"

#include "CMS1/Base/interface/Cuts.h"

namespace cms1 {

  class testMCInfo : public edm::EDAnalyzer {
  public:
    explicit testMCInfo(const edm::ParameterSet&);
    ~testMCInfo();

  private:
    virtual void beginJob(const edm::EventSetup& setup) ;
    virtual void analyze(const edm::Event&, const edm::EventSetup&);
    virtual void endJob() ;
    
    edm::InputTag hepMC_;
    edm::InputTag genJetInputTag_;
    
    Cuts elCut_;
    Cuts muCut_;
    Cuts tauCut_;
    Cuts jetCut_;
  
    MCInfo mcInfo_;
  };
}

#endif
