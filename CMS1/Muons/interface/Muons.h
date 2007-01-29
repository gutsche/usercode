#ifndef CMS1Muons_h
#define CMS1Muons_h

//
// Package:         CMS1/Muons
// Class:           Muons
// 
// Description:     analysis functions for muons
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Mon Jan 29 16:40:39 UTC 2007
//
// $Author: gutsche $
// $Date: 2007/01/22 01:35:05 $
// $Revision: 1.2 $
//

#include "DataFormats/MuonReco/interface/Muon.h"

namespace cms1 {

  class Muons  {

  public:
    Muons();
    ~Muons();
   
    bool GlobalMuons(const reco::MuonCollection *muonCollection, 
		     unsigned int number,
		     double pt);
 
  };
}

#endif
