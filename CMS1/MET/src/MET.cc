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
// $Date: 2007/05/24 17:41:00 $
// $Revision: 1.9 $
//

#include <iostream>
#include "CMS1/MET/interface/MET.h"
#include "CMS1/CommonTools/interface/UserDataTools.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/CaloTowers/interface/CaloTower.h"

// tmp
#include "TrackingTools/TrackAssociator/interface/TrackDetectorAssociator.h"

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
void cms1::MET::correctMETmuons(EventData* event, double& met, double& phi) 
{
   // first, account for muon momentum
   const std::vector<const reco::Candidate*>& metMuons = event->getBBCollection("MuonsForMETCorrection");
   double metx =  met*std::cos(phi);
   double mety =  met*std::sin(phi);
   for ( std::vector<const reco::Candidate*>::const_iterator cand = metMuons.begin(); 
	 cand != metMuons.end(); ++cand ) {
      double pt0 = (*cand)->pt(); 
      double phi0 = (*cand)->phi(); 
      metx -= pt0*std::cos(phi0);
      mety -= pt0*std::sin(phi0);
   }
   met = std::sqrt(metx*metx+mety*mety);
   phi = std::atan2(mety, metx);
   
   // second, subtract calorimeter muon energy
   //PDK now correct the ET for the muon deposits
   TrackDetectorAssociator::AssociatorParameters trackparameters;
   trackparameters.useEcal = false ;
   trackparameters.useHcal = false ;
   trackparameters.useHO = true ;
   trackparameters.useCalo = true ;
   trackparameters.useMuon = false ;
   trackparameters.useOldMuonMatching = false;
   double muEx = 0.0;
   double muEy = 0.0;
   
   for(std::vector<const reco::Candidate*>::const_iterator cand = metMuons.begin();
       cand != metMuons.end();  ++cand) {
      if( const reco::Muon* mu = dynamic_cast<const reco::Muon*>( *cand ) ) {
	 const reco::Track* mu_track = mu->track().get();
	 //trackparameters.
	 TrackDetMatchInfo info = event->trackAssociator->associate( *(event->iEvent),
								     *(event->iSetup), 
								     event->trackAssociator->getFreeTrajectoryState( *(event->iSetup), *mu_track),
								     trackparameters );
	 // use position at HCAL as the most resonable estimate
	 // where majority of energy was deposited in the calorimeter
	 double theta = info.trkGlobPosAtHcal.theta();
	 double phi = info.trkGlobPosAtHcal.phi();
	 //added PDK - fix MET for muon deposit in calo
      
	 //there is a bug in the TrackDetMatchInfo class
	 //Asking for the HO tower energy returns the total energy in the tower
	 //so using the rechits for the HO will work better (hits and towers 
	 //should be the same for the HO anyway)
	 muEx += ( info.towerCrossedEnergy() )*sin(theta)*cos( phi );
	 muEy += ( info.towerCrossedEnergy() )*sin(theta)*sin( phi );
      }
   }
   metx = met*cos(phi) + muEx;
   mety = met*sin(phi) + muEy;
   met   = std::sqrt(metx*metx + mety*mety);
   phi = atan2(mety, metx);
}

// list of jets must be supplied
void cms1::MET::correctedJetMET(EventData* event, 
				const std::vector<const reco::Candidate*>* jets,
				double& met, double& phi) 
{
   //iterate over candidates, cast them to calojets and then correct for the energy
   double METX_uncorr = met*cos(phi);
   double METY_uncorr = met*sin(phi);  
   double Ex = 0.0;
   double Ey = 0.0;
   std::vector<const reco::Candidate*>::const_iterator cand_iter;
   for(cand_iter = jets->begin() ; cand_iter != jets->end(); ++cand_iter) {
      if(const reco::CaloJet* jet = dynamic_cast<const reco::CaloJet*>(*cand_iter) ) {
	 //jet correction doesn't do so well for recoJet pt < 30.0
	 if(jet->pt() > 30.0 ) {
	    StreamerArguments args = getStreamerArguments(event, *cand_iter);
	    double corr_factor = args.jetcorrection;
	    if(corr_factor > 0 ) { //args.jetcorrection is -999 in case of an error
	       Ex = Ex + (corr_factor-1)*(jet->et())*cos(jet->phi());
	       Ey = Ey + (corr_factor-1)*(jet->et())*sin(jet->phi());
	    }
	 }
      }
   }
   double metx = METX_uncorr - Ex;
   double mety = METY_uncorr - Ey;
   std::cout << "Before correcting, MET, METPhi, METx, METy: " 
     << met << "   " << phi << "   " << METX_uncorr 
     << "   " << METY_uncorr <<std::endl;
   met = sqrt(metx*metx+mety*mety);
   phi = atan2(mety, metx);
   std::cout << "After correcting, MET, METPhi, METx, METy: " 
     << met << "   " << phi << "   " << metx << "    " 
     << mety << std::endl;
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
   data_->floatUserData.push_back( new UserData<float>("met_jetcorr", "evt_", "cms1_evt_", false) );
   evtMet_corr = data_->floatUserData.back();
   data_->floatUserData.push_back( new UserData<float>("metphi_jetcorr", "evt_", "cms1_evt_", false) );
   evtMetPhi_corr = data_->floatUserData.back();
}

void cms1::MET::fillEventUserData()
{
   const reco::CaloMET* met = getMET(DefaultMET);
   evtMet->addData( met->et() );
   evtMetPhi->addData( met->phi() );
   evtSumEt->addData( met->sumEt() );
   evtMetSig->addData( met->mEtSig() );
}












