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
#include "CMS1/Base/interface/BaseStreamer.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/MuonReco/interface/Muon.h"

namespace cms1 {
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
}

#endif

