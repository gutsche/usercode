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
// $Author: slava77 $
// $Date: 2007/03/09 20:56:39 $
// $Revision: 1.3 $
//

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include "CMS1/TableMaker/interface/TableMaker.h"
#include "CMS1/BaseAnalyzer/interface/BaseAnalyzerFW.h"

namespace cms1 {

  class TableMakerFW : public TableMaker, public BaseAnalyzerFW
     {
      public:
	explicit TableMakerFW(const edm::ParameterSet&);
	virtual ~TableMakerFW();

      private:
	virtual void analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup);
     };
}

#endif
