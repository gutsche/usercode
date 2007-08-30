#include "CMS1/CommonTools/interface/UserDataTools.h"
#include "SimDataFormats/TrackingHit/interface/PSimHit.h"
#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"
#include "SimDataFormats/Track/interface/SimTrack.h"
#include "SimDataFormats/Track/interface/SimTrackContainer.h"
#include "SimDataFormats/Vertex/interface/SimVertex.h"
#include "SimDataFormats/Vertex/interface/SimVertexContainer.h"
#include "SimDataFormats/CaloHit/interface/PCaloHit.h"
#include "SimDataFormats/CaloHit/interface/PCaloHitContainer.h"

float cms1::JetCorrection(const reco::CaloJet* jet, EventData* data) {

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
    const std::vector<const reco::Candidate*>& collection = data->getBBCollection("CorrectedJets");

    //iterate over the corrected collection and find matching jet
    for ( std::vector<const reco::Candidate*>::const_iterator iter = collection.begin(); iter != collection.end(); ++iter ) 
       {
	  const reco::CaloJet* corrjet = dynamic_cast<const reco::CaloJet*>(*iter);
	  if ( !corrjet ) {
	     std::cout << "collection CorrectedJets is not based on CaloJets. Abort." << std::endl;
	     assert(0);
	  }
	  if( fabs(corrjet->eta()-injet_eta) < 0.001 && fabs(corrjet->phi()-injet_phi) < 0.001 )
	    return corrjet->et()/injet_et;
       }
    
     std::cout << "ERROR in JetCorrection.cc: NO JET FOUND WITH SAME ETA, PHI AS INPUT JET!!!" << std::endl;
     return -995.;
  }//float JetCorrection((const reco::CaloJet* jet, EventData* data) {




cms1::StreamerArguments cms1::getStreamerArguments( EventData* data, 
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

    // fillCaloTruth(*data->iEvent, candidate->p4(), args.ecalTrueEnergy, args.hcalTrueEnergyCorrected);
	
    return args;
  }
  
std::vector<cms1::StreamerArguments> cms1::getStreamerArguments( EventData* data,
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
	 
	 // fillCaloTruth(*data->iEvent, (*itr)->p4(), args.ecalTrueEnergy, args.hcalTrueEnergyCorrected);
	
	output.push_back(args);
      }
    return output;
  }
  
std::vector<cms1::StreamerArguments> cms1::getStreamerArguments( EventData* data,
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
  

std::vector<int> cms1::getBlackBoxMap( const std::vector<const reco::Candidate*>& refCollection,
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

void cms1::fillCaloTruth( const edm::Event& iEvent,
			  const cms1::LorentzVector& p4,
			  float& ecalTrueEnergy, float& hcalTrueEnergyCorrected)
{
   // get list of simulated tracks and their vertices
   using namespace edm;
   Handle<SimTrackContainer> simTracks;
   iEvent.getByType<SimTrackContainer>(simTracks);
   if (! simTracks.isValid() ) throw cms::Exception("FatalError") << "No simulated tracks found\n";
   
   Handle<SimVertexContainer> simVertices;
   iEvent.getByType<SimVertexContainer>(simVertices);
   if (! simVertices.isValid() ) throw cms::Exception("FatalError") << "No simulated vertices found\n";
   
   // get sim calo hits
   Handle<PCaloHitContainer> simEcalHitsEB;
   iEvent.getByLabel("g4SimHits","EcalHitsEB",simEcalHitsEB);
   if (! simEcalHitsEB.isValid() ) throw cms::Exception("FatalError") << "No simulated ECAL EB hits found\n";

   Handle<PCaloHitContainer> simEcalHitsEE;
   iEvent.getByLabel("g4SimHits","EcalHitsEE",simEcalHitsEE);
   if (! simEcalHitsEE.isValid() ) throw cms::Exception("FatalError") << "No simulated ECAL EE hits found\n";

   Handle<PCaloHitContainer> simHcalHits;
   iEvent.getByLabel("g4SimHits","HcalHits",simHcalHits);
   if (! simHcalHits.isValid() ) throw cms::Exception("FatalError") << "No simulated HCAL hits found\n";

   // find truth partner
   SimTrackContainer::const_iterator simTrack = simTracks->begin();
   for( ; simTrack != simTracks->end(); ++simTrack){
      math::XYZVector simP3( simTrack->momentum().x(), simTrack->momentum().y(), simTrack->momentum().z() );
      math::XYZVector recoP3( p4.x(), p4.y(), p4.z() );
      // if ( ROOT::Math::VectorUtil::DeltaR(recoP3, simP3) < 0.1 )
      // std::cout << "Track (pt,eta,phi): " << p4.pt() << ", " << p4.eta() << ", " << 
      // p4.phi() << "\t " << "SimTrack (pt,eta,phi): " << simTrack->momentum().perp() << "," <<
      // simTrack->momentum().eta() << ", " << simTrack->momentum().phi() << std::endl;
      
      if ( simTrack->momentum().perp() > 1 &&
	   ROOT::Math::VectorUtil::DeltaR(recoP3, simP3) < 0.1 &&
	   fabs(p4.pt()-simTrack->momentum().perp())/simTrack->momentum().perp()< 0.5
	) break;
   }
   if ( simTrack != simTracks->end() ) {
      double ecalEnergy(0);
      double hcalEnergy(0);
      
      // loop over calo hits
      for( PCaloHitContainer::const_iterator hit = simEcalHitsEB->begin(); hit != simEcalHitsEB->end(); ++hit )
	if ( hit->geantTrackId() == simTrack->genpartIndex() ) ecalEnergy += hit->energy();
      
      for( PCaloHitContainer::const_iterator hit = simEcalHitsEE->begin(); hit != simEcalHitsEE->end(); ++hit )
	if ( hit->geantTrackId() == simTrack->genpartIndex() ) ecalEnergy += hit->energy();
      
      for( PCaloHitContainer::const_iterator hit = simHcalHits->begin(); hit != simHcalHits->end(); ++hit )
	if ( hit->geantTrackId() == simTrack->genpartIndex() ) hcalEnergy += hit->energy();
      
      ecalTrueEnergy = ecalEnergy;
      hcalTrueEnergyCorrected = hcalEnergy;
      // scale factor depends on eta
      if ( fabs(p4.eta()) < 1.3 )
	hcalTrueEnergyCorrected = hcalEnergy*113.2;
      else 
	if ( fabs(p4.eta()) < 3.0 )
	  hcalTrueEnergyCorrected = hcalEnergy*167.2;
      std::cout << "Stored: Track (pt,eta,phi): " << p4.pt() << ", " << p4.eta() << ", " << 
      p4.phi() << "\t " << "SimTrack (pt,eta,phi): " << simTrack->momentum().perp() << "," <<
      simTrack->momentum().eta() << ", " << simTrack->momentum().phi() << 
	"\n\tecal: " << ecalEnergy << " \t hcal: " << hcalEnergy << std::endl;
      // std::cout<<"\tstored\n"<<std::endl;
      
   }
	
	
}
