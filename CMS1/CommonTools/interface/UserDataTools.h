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


  float JetCorrection(const reco::CaloJet* jet, EventData* data) {

    //make sure that the input jet is valid
    if( !jet ) {
      std::cout << "ERROR  in JetCorrection.cc: The input jet doesn't exist!!" << std::endl;
      return -998;
    }

    //Get the eta, phi and E_t of the input jet
    double injet_eta = jet->eta();
    double injet_phi = jet->phi();
    double injet_et = jet->et();

    // Check that the data is there
    if (! data ) {
      std::cout << "ERROR JetCorrection.cc: doesn't know where to find EvenData." << std::endl;
      return -997;
    }

    // Get the corrected jet collection and quit if it doesn't exist
    const std::vector<reco::CaloJet>* collection = data->getData<std::vector<reco::CaloJet> >("MCJetCorJetMcone5");
    if ( ! collection ) {
      std::cout << "ERROR: Corrected Jet collection is not found in the event. Return nothing." << std::endl;
      return -996;
    }

    //iterate over the corrected collection and find matching jet
    for ( std::vector<reco::CaloJet>::const_iterator jetiter = collection->begin();
	  jetiter != collection->end(); ++jetiter ) {
      double corrjet_eta = jetiter->eta();
      double corrjet_phi = jetiter->phi();
      double corrjet_et  = jetiter->et();

      if( fabs(corrjet_eta-injet_eta) < 0.001 && fabs(corrjet_phi-injet_phi) < 0.001 ) {
	return corrjet_et/injet_et;
      }

    } // for loop


    std::cout << "ERROR in JetCorrection.cc: NO JET FOUND WITH SAME ETA, PHI AS INPUT JET!!!" << std::endl;
    return -995.;
  }//float JetCorrection((const reco::CaloJet* jet, EventData* data) {




  StreamerArguments getStreamerArguments( EventData* data, 
					  const reco::Candidate* candidate )
  {
    StreamerArguments args;
    args.candidate = candidate;
	
    MCTruth mcTruth;
    mcTruth.setEventData( data );
	
    args.genParticle = mcTruth.matchCandToGen( *candidate );
    args.genJet = mcTruth.matchCandToGenJet( *candidate );
	
    if ( const reco::Muon* muon = dynamic_cast<const reco::Muon*>(candidate) )  
      args.track = muon->track().get();
	
    if ( const reco::PixelMatchGsfElectron* elec = dynamic_cast<const reco::PixelMatchGsfElectron*>(candidate) ) 
      args.track = elec->gsfTrack().get();

   //match the calo and gen jet
    if( const reco::CaloJet* jet = dynamic_cast<const reco::CaloJet*>(candidate) ) {
      args.jetcorrection = JetCorrection(jet, data);
    }


	
    return args;
  }
  std::vector<StreamerArguments> getStreamerArguments( EventData* data, 
						       std::vector<const reco::Candidate*> candidates )
  {
    std::vector<StreamerArguments> output;
    for(std::vector<const reco::Candidate*>::const_iterator itr = candidates.begin();
	itr != candidates.end(); ++itr)
      {
	StreamerArguments args;
	args.candidate = *itr;
	     
	MCTruth mcTruth;
	mcTruth.setEventData( data );
	
	args.genParticle = mcTruth.matchCandToGen( **itr );
	args.genJet = mcTruth.matchCandToGenJet( **itr );
	
	if ( const reco::Muon* muon = dynamic_cast<const reco::Muon*>(*itr) )
	  args.track = muon->track().get();
	
	if ( const reco::PixelMatchGsfElectron* elec = dynamic_cast<const reco::PixelMatchGsfElectron*>( *itr ) )
	  args.track = elec->gsfTrack().get();


        //match the calo and gen jet
        if ( const reco::CaloJet* jet = dynamic_cast<const reco::CaloJet*>(*itr) ) {
          args.jetcorrection = JetCorrection(jet, data);
        }
	
	output.push_back(args);
      }
    return output;
  }
  std::vector<StreamerArguments> getStreamerArguments( EventData* data,
						       std::vector<const reco::Track*> candidates )
  {
    std::vector<StreamerArguments> output;
    for(std::vector<const reco::Track*>::const_iterator itr = candidates.begin();
	itr != candidates.end(); ++itr)
      {
	StreamerArguments args;
	args.track = *itr;
	     
	MCTruth mcTruth;
	mcTruth.setEventData( data );
	LorentzVector p4((*itr)->px(),(*itr)->py(),(*itr)->pz(),(*itr)->p());
	args.genParticle = mcTruth.matchP4ToGen( p4 );
	output.push_back(args);
      }
    return output;
  }
  
   std::vector<int> getBlackBoxMap( const std::vector<const reco::Candidate*>& refCollection,
				    const std::vector<const reco::Candidate*>& testCollection )
     {
	std::vector<int> map;
	for(unsigned int i=0; i<refCollection.size(); ++i)
	  {
	     bool found = false;
	     for(unsigned int j=0; j<testCollection.size(); ++j)
	       {
		  if (refCollection[i] == testCollection[j]) {
		     found = true;
		     break;
		  }
		  const reco::Muon* refMuon = 0;
		  const reco::Muon* testMuon = 0;
		  if ( (refMuon = dynamic_cast<const reco::Muon*>(refCollection[i])) &&
		       (testMuon = dynamic_cast<const reco::Muon*>(testCollection[j])) &&
		       (refMuon->track() == testMuon->track()) )
		    {
		       found = true;
		       break;
		    }
	       }
	     if (found) 
	       map.push_back(1);
	     else 
	       map.push_back(0);
	  }
	return map;
     }
   
}
   

#endif

