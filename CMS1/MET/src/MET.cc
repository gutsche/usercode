//
// Package:         CMS1/MET
// Class:           MET
// 
// Description:     analysis functions for MET
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Wed Feb 21 00:50:30 UTC 2007
//
// $Author: latb $
// $Date: 2007/03/01 19:06:59 $
// $Revision: 1.3 $
//

#include <iostream>
#include "CMS1/MET/interface/MET.h"

std::vector<const reco::CaloMET*> cms1::MET::getMET(const METType METType,
						      const Cuts& cuts,
						      const bool isolated )
{
  // this is the output list
  std::vector<const reco::CaloMET*> output_list;
   
  // Only TightGlobalMET are implemented for now
  switch (METType) {
  case GlobalMET:
    {
      if (! data_.globalMETCollection) {
	std::cout << "ERROR: global MET collection is not set" << std::endl;
	return output_list;
      }

      if ( data_.globalMETCollection->size() > 0 ) {

	const reco::CaloMET *MET = &(*(data_.globalMETCollection->begin()));

	if ( cuts.testCandidate(*MET) ) output_list.push_back(MET);
      }
      
      // At this point the output_list has been filled with MET passing the simple cuts
      // In general, we will want to add more cuts.  This could go here.  It can be
      // done several ways, for example:
      //     Make another empty vector "final_output_list"
      //     loop over the vector "output_list" that we just made and add to final_output_list
      //     those MET that pass more complicated cuts
      // For now, none of this is implemented    
      
    }
    break;
    // You get here if you have requested a "METType" that is not implemented
  default:
    std::cout << "Unkown or not implemented type" << std::endl;
  }
  return output_list;
}




















