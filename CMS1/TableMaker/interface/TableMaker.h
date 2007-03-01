#ifndef TableMaker_h
#define TableMaker_h

//
// Package:         CMS1/TableMaker
// Class:           TableMaker

// 
// Description:     EDAnalyzer filling table for di-lepton analysis dependent on number of jets
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Tue Feb 20 23:00:01 UTC 2007
//
// $Author: latb $
// $Date: 2007/03/01 21:09:56 $
// $Revision: 1.3 $
//

#include "CMS1/Muons/interface/Muons.h"
#include "CMS1/Electrons/interface/Electrons.h"
#include "CMS1/Jets/interface/Jets.h"
#include "CMS1/MET/interface/MET.h"

#include "CMS1/Base/interface/Cuts.h"

namespace cms1 {

  class TableMaker {
  public:
    void analyze();
    void beginJob();
    void endJob();

  protected:
    Cuts          tightMuon_;
    Cuts          looseMuon_;
    Cuts          allMuon_;

    Cuts          tightElectron_;
    Cuts          looseElectron_;

    Cuts          jet_;

    Cuts          metCut_;
    Cuts          metCutAroundZ_;

    Cuts          noCut_;

    double        ZRangeMinMass_;
    double        ZRangeMaxMass_;

    Muons         muons_;

    Electrons     electrons_;

    Jets          jets_;

    MET           MET_;

    unsigned int  events_;
    unsigned int  countedEE0Jets_;
    unsigned int  countedEMu0Jets_;
    unsigned int  countedMuE0Jets_;
    unsigned int  countedMuMu0Jets_;
    unsigned int  countedEE1Jets_;
    unsigned int  countedEMu1Jets_;
    unsigned int  countedMuE1Jets_;
    unsigned int  countedMuMu1Jets_;
    unsigned int  countedEE2Jets_;
    unsigned int  countedEMu2Jets_;
    unsigned int  countedMuE2Jets_;
    unsigned int  countedMuMu2Jets_;
    unsigned int  countedEE3Jets_;
    unsigned int  countedEMu3Jets_;
    unsigned int  countedMuE3Jets_;
    unsigned int  countedMuMu3Jets_;
    unsigned int  countedEE4Jets_;
    unsigned int  countedEMu4Jets_;
    unsigned int  countedMuE4Jets_;
    unsigned int  countedMuMu4Jets_;

  };
}

#endif
