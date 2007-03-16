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
// $Date: 2007/03/01 21:09:55 $
// $Revision: 1.4 $
//

#include <iostream>
#include "CMS1/MET/interface/MET.h"

const reco::CaloMET* cms1::MET::getMET(const METType type)
{
   
   switch (type) {
    case DefaultMET:
    {
      if (! data_ || ! data_->metCollection) {
	std::cout << "ERROR: MET collection is not set" << std::endl;
	 return 0;
      }

      if ( ! data_->metCollection->empty() )
	 return &(data_->metCollection->front());
       else
	 return 0;
    }
    break;
    // You get here if you have requested a "METType" that is not implemented
  default:
    std::cout << "Unkown or not implemented type" << std::endl;
  }
   return 0;
}

// correct MET energies for Muons
// (input parameters are corrected by the algorithm)
void cms1::MET::correctMETmuons(const std::vector<const reco::Muon*>* m, double& et, double& phi) {
//		std::cout << "We're correctiong METs for Muons here" << std::endl;
// ACHTUNG: we should also correct for the energy MIP

   	double metx =  et*std::cos(phi);
		double mety =  et*std::sin(phi);
		for ( std::vector<const reco::Muon*>::const_iterator i = m->begin(), ie = m->end();
			i != ie;
			++i ) {
				const reco::Muon* cp = *i;
				double pt0 = cp->pt(); 
				double phi0 = cp->phi(); 
				metx -= pt0*std::cos(phi0);
				mety -= pt0*std::sin(phi0);
		}
		et = std::sqrt(metx*metx+mety*mety);
		phi = std::atan2(mety, metx);
}



















