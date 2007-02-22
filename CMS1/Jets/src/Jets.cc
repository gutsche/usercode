//
// Package:         CMS1/Jets
// Class:           Jets
// 
// Description:     analysis functions for jets
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Wed Feb 21 00:50:30 UTC 2007
//
// $Author: dmytro $
// $Date: 2007/02/16 11:53:33 $
// $Revision: 1.2 $
//

#include "CMS1/Jets/interface/Jets.h"

std::vector<const reco::CaloJet*> cms1::Jets::getJets(const JetType jetType,
						      const Cuts& cuts,
						      const bool isolated )
{
  // this is the output list
  std::vector<const reco::CaloJet*> output_list;
   
  // Only TightGlobalJets are implemented for now
  switch (jetType) {
  case GlobalJets:
    {
      if (! data_.globalJetCollection) {
	std::cout << "ERROR: global jet collection is not set" << std::endl;
	return output_list;
      }
      for ( std::vector<reco::CaloJet>::const_iterator jet = data_.globalJetCollection->begin();
	    jet != data_.globalJetCollection->end();
	    ++jet ) 
	{
	  // Here we make simple cuts in a standard way
	  if ( cuts.testCandidate(*jet) ) output_list.push_back(&*jet);
	}
      
      // At this point the output_list has been filled with jets passing the simple cuts
      // In general, we will want to add more cuts.  This could go here.  It can be
      // done several ways, for example:
      //     Make another empty vector "final_output_list"
      //     loop over the vector "output_list" that we just made and add to final_output_list
      //     those jets that pass more complicated cuts
      // For now, none of this is implemented    
      
    }
    break;
    // You get here if you have requested a "jetType" that is not implemented
  default:
    std::cout << "Unkown or not implemented type" << std::endl;
  }
  return output_list;
}
