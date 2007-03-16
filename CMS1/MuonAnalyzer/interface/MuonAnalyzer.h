#ifndef MuonAnalyzer_h
#define MuonAnalyzer_h

//
// Package:         CMS1/MuonAnalyzer
// Class:           MuonAnalyzer

// 
// Description:     EDAnalyzer using cms1 muons object
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Mon Jan 29 17:42:34 UTC 2007
//
// $Author: gutsche $
// $Date: 2007/01/29 18:15:27 $
// $Revision: 1.1 $
//

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include "CMS1/Muons/interface/Muons.h"

namespace cms1 {

  class MuonAnalyzer : public edm::EDAnalyzer {
  public:
    explicit MuonAnalyzer(const edm::ParameterSet&);
    ~MuonAnalyzer();

  private:
    virtual void beginJob(const edm::EventSetup& setup) ;
    virtual void analyze(const edm::Event&, const edm::EventSetup&);
    virtual void endJob() ;
    
    edm::InputTag globalMuonInputTag_;
    
    unsigned int  number_;
    double        pt_;

    Muons   muons_;
    EventData data_;
  };
}

#endif
