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
// $Date: 2007/08/30 13:33:38 $
// $Revision: 1.17 $
//

#include <iostream>
#include "CMS1/MET/interface/MET.h"
#include "CMS1/CommonTools/interface/UserDataTools.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/CaloTowers/interface/CaloTower.h"

const reco::CaloMET* cms1::MET::getMET(const std::string& type)
{
   if (! data_ ) {
      std::cout << "ERROR: MET black box doesn't know where to find EvenData." << std::endl;
      assert(0);
   }
   
   if ( type.compare("DefaultMET")==0 )
    {
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
   std::cout << "Unkown or not implemented MET type: " << type << "\nAbort" << std::endl;
   assert(0);
   return 0;
}

// correct MET energies for Muons
// (input parameters are corrected by the algorithm)
void cms1::MET::correctMETmuons(EventData& event, const std::vector<const reco::Candidate*>& metMuons, 
				double& met, double& metPhi, CorrectionType type)
{
   // first, account for muon momentum
   // const std::vector<const reco::Candidate*>& metMuons = event->getBBCollection("MuonsForMETCorrection");
   double metx =  met*std::cos(metPhi);
   double mety =  met*std::sin(metPhi);
   for ( std::vector<const reco::Candidate*>::const_iterator cand = metMuons.begin(); 
	 cand != metMuons.end(); ++cand ) {
      double pt0 = (*cand)->pt(); 
      double phi0 = (*cand)->phi(); 
      metx -= pt0*std::cos(phi0);
      mety -= pt0*std::sin(phi0);
   }
   met = std::sqrt(metx*metx+mety*mety);
   metPhi = std::atan2(mety, metx);
   
   if ( type == NoCaloCorrection ) return;
   
   double muEx = 0.0;
   double muEy = 0.0;
   
   for(std::vector<const reco::Candidate*>::const_iterator cand = metMuons.begin();
       cand != metMuons.end();  ++cand) {
      if( const reco::Muon* mu = dynamic_cast<const reco::Muon*>( *cand ) ) {
	 const reco::Track* mu_track = mu->track().get();
	 // use muon position at the outer most state of the silicon track if 
	 // TrackExtra is available and momentum direction at the origin 
	 // otherwise. Both should be fine.
	 // NOTICE: MET is built out of towers, which are 5x5 ECAL crystals + one 
	 // element of HCAL and HO. Muon energy is reported for individual crossed 
	 // elements of all the detectors and 3x3 elements of each time as an 
	 // alternative way of energy calculation.
	 double theta = mu_track->theta();
	 double phi = mu_track->phi();
	 /*  BROKEN
	 if ( ! mu_track->extra().isNull() ) {
	    theta = mu_track->extra()->outerPosition().theta();
	    phi = mu_track->extra()->outerPosition().phi();
	 }
	 */
	  
	 switch (type) {
	  case CrossedEnergyCorrection:
	    muEx += ( mu->getCalEnergy().em + mu->getCalEnergy().had + mu->getCalEnergy().ho )*sin(theta)*cos( phi );
	    muEy += ( mu->getCalEnergy().em + mu->getCalEnergy().had + mu->getCalEnergy().ho )*sin(theta)*sin( phi );
	    break;
	  case S9EnergyCorrection:
	    muEx += ( mu->getCalEnergy().emS9 + mu->getCalEnergy().hadS9 + mu->getCalEnergy().hoS9 )*sin(theta)*cos( phi );
	    muEy += ( mu->getCalEnergy().emS9 + mu->getCalEnergy().hadS9 + mu->getCalEnergy().hoS9 )*sin(theta)*sin( phi );
	    break;
	  case ExpectedMipEnergyCorrection:
	    // numbers are essential a wild guess
	    if ( fabs(mu->eta()) < 1.5 ) { 
	       // barrel
	       muEx += ( 0.3 + 3.0 + 1.0 )*sin(theta)*cos( phi );
	       muEy += ( 0.3 + 3.0 + 1.0 )*sin(theta)*sin( phi );
	    } else {
	       // endcap
	       muEx += ( 0.35 + 3.5 )*sin(theta)*cos( phi );
	       muEy += ( 0.35 + 3.5 )*sin(theta)*sin( phi );
	    }
	    break;
	  default:
	    std::cout << "Uknown MET correction type. Abort" << std::endl;
	    assert(0);
	 }
      }
   }
   
   metx = met*cos(metPhi) + muEx;
   mety = met*sin(metPhi) + muEy;
   met   = std::sqrt(metx*metx + mety*mety);
   metPhi = atan2(mety, metx);
}

// list of jets must be supplied
void cms1::MET::correctedJetMET(EventData& event,
				const std::vector<const reco::Candidate*>& jets,
				double& met, double& metPhi, 
				const double min_pt) 
{
   //iterate over candidates, cast them to calojets and then correct for the energy
   double METX_uncorr = met*cos(metPhi);
   double METY_uncorr = met*sin(metPhi);  
   double Ex = 0.0;
   double Ey = 0.0;
   std::vector<const reco::Candidate*>::const_iterator cand_iter;
   for(cand_iter = jets.begin() ; cand_iter != jets.end(); ++cand_iter) {
      if(const reco::CaloJet* jet = dynamic_cast<const reco::CaloJet*>(*cand_iter) ) {
	 //jet correction doesn't do so well for recoJet pt < 30.0
	 if(jet->pt() > min_pt ) {
	    StreamerArguments args = getStreamerArguments(&event, *cand_iter);
	    double corr_factor = args.jetcorrection;
	    if(corr_factor > 0 ) { //args.jetcorrection is -999 in case of an error
	       // std::cout << "Jet Pt: " << jet->pt() << "\t Jet phi: " << jet->phi() << "\tcorr: " << corr_factor <<std::endl;
	       Ex = Ex + (corr_factor-1)*(jet->et())*cos(jet->phi());
	       Ey = Ey + (corr_factor-1)*(jet->et())*sin(jet->phi());
	    }
	 }
      }
   }
   double metx = METX_uncorr - Ex;
   double mety = METY_uncorr - Ey;
   /* std::cout << "Before correcting, MET, METPhi, METx, METy: " 
     << met << "   " << metPhi << "   " << METX_uncorr 
     << "   " << METY_uncorr <<std::endl; */
   met = sqrt(metx*metx+mety*mety);
   metPhi = atan2(mety, metx);
   /* std::cout << "After correcting, MET, METPhi, METx, METy: " 
     << met << "   " << metPhi << "   " << metx << "    " 
     << mety << std::endl; */
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
   data_->floatUserData.push_back( new UserData<float>("evt_met", "uncorrected MET", false) );
   evtMet = data_->floatUserData.back();
   data_->floatUserData.push_back( new UserData<float>("evt_metPhi", "uncorrected MET phi", false) );
   evtMetPhi = data_->floatUserData.back();
   data_->floatUserData.push_back( new UserData<float>("evt_sumEt", "uncorrected sum Et ", false) );
   evtSumEt = data_->floatUserData.back();
   data_->floatUserData.push_back( new UserData<float>("evt_metSig", "uncorrected relative MET - MET significance", false) );
   evtMetSig = data_->floatUserData.back();
   data_->floatUserData.push_back( new UserData<float>("evt_met_jetcorr", "jet energy scale corrected MET (no other corrections)", false) );
   evtMet_corr = data_->floatUserData.back();
   data_->floatUserData.push_back( new UserData<float>("metphi_jetcorr", "jet energy scale corrected MET phi (no other corrections)",  false) );
   evtMetPhi_corr = data_->floatUserData.back();
}

void cms1::MET::fillEventUserData()
{
   const reco::CaloMET* met = getMET("DefaultMET");
   evtMet->addData( met->et() );
   evtMetPhi->addData( met->phi() );
   evtSumEt->addData( met->sumEt() );
   evtMetSig->addData( met->mEtSig() );
}












