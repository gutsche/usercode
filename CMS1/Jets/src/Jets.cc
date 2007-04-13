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
// $Date: 2007/04/12 19:43:39 $
// $Revision: 1.6 $
//

#include "CMS1/Jets/interface/Jets.h"
#include "DataFormats/EgammaCandidates/interface/SiStripElectron.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"

std::vector<const reco::Candidate*> cms1::Jets::getJets( const JetType jetType,
						       const Cuts& userCuts )
{
  // this is the output list
  std::vector<const reco::Candidate*> output_list;
   
   switch (jetType) {
    case DefaultJets:
	{
	   if (! data_ ) {
	      std::cout << "ERROR: jet black box doesn't know where to find EvenData." << std::endl;
	      return output_list;
	   }
	   const std::vector<reco::CaloJet>* collection = 
	     data_->container_reco_CaloJet.getCollection(edm::InputTag("midPointCone5CaloJets",""));
	   if ( ! collection ) {
	      std::cout << "ERROR: jet collection is not found in the event. Return nothing." << std::endl;
	      return output_list;
	   }
       
	   // set the default cuts for this type
	   Cuts cuts;
	   cuts.pt_min = 20;
	   cuts.eta_min = -2.5;
	   cuts.eta_max = +2.5;
	   cuts.AND(userCuts);
       
	   for ( std::vector<reco::CaloJet>::const_iterator jet = collection->begin();
		 jet != collection->end();
		 ++jet ) 
	     {
		if ( cuts.testCandidate(*jet) ) output_list.push_back(&*jet);
	     }
	}
      break;
    case JetsWithoutElectrons:
	{
	   // get a set of preselected jets
	   std::vector<const reco::Candidate*> jets = getJets(DefaultJets,Cuts());
	   
	   // get a set of veto electrons (they should be clean so that the jet 
	   // selection efficiency is not too low )
	   // 
	   // one way would be calling another black box:
	   //   Electron myElectron;
	   //   myElectron.setEventData(data_);
	   //   std::vector<const reco::Candidate*> electrons = 
	   //         myElectron.getElectrons(TightElectrons, Cuts(), Cuts::Isolated);
	   // 
	   // However it would introduce cross dependencies between black boxes and it this moment
	   // it's not clear whether it's a good practice, so let's just clean up electrons ourself
	   // for this type of Jets 

	   if (! data_ ) {
	      std::cout << "ERROR: electron black box doesn't know where to find EvenData." << std::endl;
	      return output_list;
	   }
	   const std::vector<reco::SiStripElectron>* electronCollection = 
	     data_->container_reco_SiStripElectron.getCollection(edm::InputTag("siStripElectrons",""));
	   if ( ! electronCollection ) {
	      std::cout << "ERROR: electron muon collection is not found in the event. Return nothing." << std::endl;
	      return output_list;
	   }

	   std::vector<const reco::Candidate*> vetoElectrons;
	   
	   // set the default cuts for this type
	   Cuts cuts;
	   cuts.pt_min = 20;
	   cuts.eta_min = -2.4;
	   cuts.eta_max = -2.4;
	   cuts.isolated = Cuts::Isolated;
	   cuts.setEventData( data_ );
	   
	   for ( std::vector<reco::SiStripElectron>::const_iterator electron = electronCollection->begin();
		 electron != electronCollection->end();
		 ++electron ) 
	     {
		if ( ! cuts.testCandidate(*electron) ) continue;
		if ( ! userCuts.testCandidate(*electron) ) continue;
		
		// cut on E/p
		double energy = electron->superCluster()->energy();
		if (electron->p() < 1 || energy/electron->p() <0.5) continue;
		
		vetoElectrons.push_back(&*electron);
	     }
	   
	   // finally we have veto electron, so let's clean up jets
	   for ( std::vector<const reco::Candidate*>::const_iterator jet = jets.begin();
		 jet != jets.end(); ++ jet )
	     {
		if ( ! cuts.testCandidate(**jet) ) continue;
		if ( ! cuts.testJetForElectrons(**jet, vetoElectrons ) ) continue;
		output_list.push_back(*jet);
	     }
	}
      break;
    // You get here if you have requested a "jetType" that is not implemented
  default:
    std::cout << "Unkown or not implemented jet type" << std::endl;
  }
  return output_list;
}

void cms1::Jets::dump(ostream& o, std::vector<const reco::Candidate*> ml) {
	for ( std::vector<const reco::Candidate*>::iterator i = ml.begin(), ie = ml.end(); i != ie; ++i ) {
		const reco::Candidate* cp = *i;
		o << "Jet      "; 
		o << "Pt = " << cp->pt(); 
		o << ", Eta = " << cp->eta(); 
		o << ", Phi = " << cp->phi(); 
		o << std::endl; 
	}
}
