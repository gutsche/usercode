#ifndef CMS1_UserDataTools_h
#define CMS1_UserDataTools_h

//
// Package:         CMS1/CommonTools
// 
// Description:     tools related to user data manipulation
//
// Original Author: Dmytro Kovalskyi
//
#include "CMS1/CommonTools/interface/MCTruth.h"
#include "CMS1/Base/interface/EventData.h"
#include "CMS1/Base/interface/BaseStreamer.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/MuonReco/interface/Muon.h"

namespace cms1 {

  /*
    Simple Function that returns a correction factor for a given jet.
    Assumes (without checking) that the input jet is a midPointCone5CaloJets
    Assumes (and checks) that the collection of corrected Jets is MCJetCorJetMcone5
    The correction factor returned is the ratio of the corrected jet E_t and the corresponding
    input jet E_t. The correspondence of the jets is done by requiring that the eta, phi
    of the 2 jets be identical.

    The code is inefficient 'cause the corrected collection is accessed not once per event
    but N times where N is the number of input jets

    Claudio, Puneeth
    5/30/07
  */


   float JetCorrection(const reco::CaloJet* jet, EventData* data);
   
   StreamerArguments getStreamerArguments( EventData* data, const reco::Candidate* candidate );
   
   std::vector<StreamerArguments> getStreamerArguments( EventData* data,
						       std::vector<const reco::Candidate*> candidates );
   
   std::vector<StreamerArguments> getStreamerArguments( EventData* data,
						       std::vector<const reco::Track*> candidates );
  
   std::vector<int> getBlackBoxMap( const std::vector<const reco::Candidate*>& refCollection,
				    const std::vector<const reco::Candidate*>& testCollection );
   
   void fillCaloTruth( const edm::Event& iEvent, const LorentzVector& p4,
		       float& ecalTrueEnergy, float& hcalTrueEnergyCorrected);
}
   

#endif

