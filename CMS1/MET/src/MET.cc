//
// Package:         CMS1/MET
// Class:           MET
// 
// Description:     analysis functions for MET
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Wed Feb 21 00:50:30 UTC 2007
//
// $Author: dmytro $
// $Date: 2007/04/17 04:57:25 $
// $Revision: 1.8 $
//

#include <iostream>
#include "CMS1/MET/interface/MET.h"

const reco::CaloMET* cms1::MET::getMET(const METType type)
{
   
   switch (type) {
    case DefaultMET:
    {
       if (! data_ ) {
	  std::cout << "ERROR: MET black box doesn't know where to find EvenData." << std::endl;
	  return 0;
       }
       const std::vector<reco::CaloMET>* collection = data_->getData<std::vector<reco::CaloMET> >("met");
       if ( ! collection ) {
	  std::cout << "ERROR: MET is not found in the event. Return nothing." << std::endl;
	  return 0;
       }

       if ( ! collection->empty() )
	 return &(collection->front());
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
void cms1::MET::correctMETmuons(const std::vector<const reco::Candidate*>* m, double& et, double& phi) {
//		std::cout << "We're correctiong METs for Muons here" << std::endl;
// ACHTUNG: we should also correct for the energy MIP

   	double metx =  et*std::cos(phi);
		double mety =  et*std::sin(phi);
		for ( std::vector<const reco::Candidate*>::const_iterator i = m->begin(), ie = m->end();
			i != ie;
			++i ) {
				const reco::Candidate* cp = *i;
				double pt0 = cp->pt(); 
				double phi0 = cp->phi(); 
				metx -= pt0*std::cos(phi0);
				mety -= pt0*std::sin(phi0);
		}
		et = std::sqrt(metx*metx+mety*mety);
		phi = std::atan2(mety, metx);
}




void cms1::MET::dump(std::ostream& o, const reco::Candidate* mo) {

	o << "MET      "; 
	o << "Pt = " << mo->pt(); 
	o << ", Eta = " << mo->eta(); 
	o << ", Phi = " << mo->phi(); 
	o << std::endl; 
}

void cms1::MET::registerEventUserData()
{
   data_->floatUserData.push_back( new UserData<float>("met", "evt_", "cms1_evt_", false) );
   evtMet = data_->floatUserData.back();
   data_->floatUserData.push_back( new UserData<float>("metPhi", "evt_", "cms1_evt_", false) );
   evtMetPhi = data_->floatUserData.back();
   data_->floatUserData.push_back( new UserData<float>("sumEt", "evt_", "cms1_evt_", false) );
   evtSumEt = data_->floatUserData.back();
   data_->floatUserData.push_back( new UserData<float>("metSig", "evt_", "cms1_evt_", false) );
   evtMetSig = data_->floatUserData.back();
}

void cms1::MET::fillEventUserData()
{
   const reco::CaloMET* met = getMET(DefaultMET);
   evtMet->addData( met->et() );
   evtMetPhi->addData( met->phi() );
   evtSumEt->addData( met->sumEt() );
   evtMetSig->addData( met->mEtSig() );
}












