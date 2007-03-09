#ifndef TableMakerFW_h
#define TableMakerFW_h

//
// Package:         CMS1/TableMaker
// Class:           TableMaker

// 
// Description:     EDAnalyzer filling table for di-lepton analysis dependent on number of jets
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Tue Feb 20 23:00:01 UTC 2007
//
// $Author: fisk $
// $Date: 2007/03/09 18:22:23 $
// $Revision: 1.2 $
//

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include "CMS1/TableMaker/interface/TableMaker.h"

namespace cms1 {

  class TableMakerFW : public edm::EDAnalyzer, 
     TableMaker {
  public:
    explicit TableMakerFW(const edm::ParameterSet&);
    ~TableMakerFW();

  private:
    virtual void beginJob(const edm::EventSetup& setup) ;
    virtual void analyze(const edm::Event&, const edm::EventSetup&);
    virtual void endJob() ;
    edm::InputTag globalTrackInputTag_;

    edm::InputTag globalMuonInputTag_;
    edm::InputTag globalElectronInputTag_;
    edm::InputTag globalJetInputTag_;
    edm::InputTag globalMETInputTag_;

  };
}

#endif
