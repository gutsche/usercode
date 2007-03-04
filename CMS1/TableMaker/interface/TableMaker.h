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
// $Author: dmytro $
// $Date: 2007/03/01 21:25:07 $
// $Revision: 1.4 $
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
    unsigned int  countedEEJets_[5];
    unsigned int  countedEMuJets_[5];
    unsigned int  countedMuEJets_[5];
    unsigned int  countedMuMuJets_[5];
  };
}

#endif
