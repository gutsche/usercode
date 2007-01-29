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

#include "CMS1/Muons/interface/Muons.h"

cms1::Muons::Muons()
{
}


cms1::Muons::~Muons()
{
}

bool cms1::Muons::GlobalMuons(const reco::MuonCollection *muonCollection, 
			      unsigned int number,
			      double pt) {

  // return value
  bool result = false;

  unsigned int counter = 0;
  for ( reco::MuonCollection::const_iterator muon = muonCollection->begin();
	muon != muonCollection->end();
	++muon ) {
    if (muon->pt() > pt ) {
      ++counter;
    }
  }

  if ( counter >= number ) {
    result = true;
  }
    
  return result;
}
