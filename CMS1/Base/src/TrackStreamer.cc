// Description:     track streamer class 
//
// Original Author: Dmytro Kovalskyi
//
// $Author: dmytro $
// $Date: 2007/05/23 02:23:23 $
// $Revision: 1.2 $
//
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"
#include "CMS1/Base/interface/TrackStreamer.h"
cms1::TrackStreamer::TrackStreamer() 
{
   // ORDER IS CRITICAL !
   p4Names_.push_back("p4");            p4s_.push_back( LorentzVector(0,0,0,0) );
   floatNames_.push_back("d0");         floats_.push_back(0);
   floatNames_.push_back("z0");         floats_.push_back(0);
   floatNames_.push_back("chi2");       floats_.push_back(0);
   floatNames_.push_back("ndof");       floats_.push_back(0);
   intNames_.push_back("validHits");    ints_.push_back(0);
   intNames_.push_back("lostHits");     ints_.push_back(0);
   floatNames_.push_back("d0Err");      floats_.push_back(0);
   floatNames_.push_back("z0Err");      floats_.push_back(0);
   floatNames_.push_back("ptErr");      floats_.push_back(0);
   floatNames_.push_back("etaErr");      floats_.push_back(0);
   floatNames_.push_back("phiErr");      floats_.push_back(0);
   // truth matching
   p4Names_.push_back("mc_p4");         p4s_.push_back( LorentzVector(0,0,0,0) );
   intNames_.push_back("mc_id");        ints_.push_back(0);
   
   mass_ = 0;
}

void cms1::TrackStreamer::setDefaults()
{
   p4s_[varP4]   = LorentzVector(0,0,0,0);
   p4s_[varMCP4] = LorentzVector(0,0,0,0);
   floats_[varD0] = -999. ;
   floats_[varZ0] = -999. ;
   floats_[varChi2] = -999. ;
   ints_[varValidHits] = -999 ;
   ints_[varlostHits] = -999 ;
   ints_[varPdgId] = 0 ; 
   floats_[varD0Err] = -999. ;
   floats_[varZ0Err] = -999. ;
   floats_[varPtErr] = -999. ;
   floats_[varEtaErr] = -999. ;
   floats_[varPhiErr] = -999. ;
}

void cms1::TrackStreamer::fill( const reco::Candidate* candidate ) 
{
   if (! candidate) {
      setDefaults();
      return;
   }
   if ( const reco::Muon* muon = dynamic_cast<const reco::Muon*>(candidate) ) {
      fill(muon->track().get());
      return;
   }
   if ( const reco::PixelMatchGsfElectron* elec = dynamic_cast<const reco::PixelMatchGsfElectron*>(candidate) ) {
      fill((const reco::Track*)elec->gsfTrack().get());
      p4s_[varP4] = candidate->p4(); // Candidate has a proper mix of calorimeter and tracker information (energy from ECAL, direction from tracker)
      return;
   }
   p4s_[varP4] = candidate->p4();
}

void cms1::TrackStreamer::fill( const reco::Track* track ) 
{
   if ( ! track ) {
      setDefaults();
      return;
   }
   p4s_[varP4] = LorentzVector( track->px(), track->py(), track->pz(), sqrt(track->p()*track->p()+mass_*mass_) );
   floats_[varD0] = track->d0() ;
   floats_[varZ0] = track->dz() ;
   floats_[varD0Err] = track->d0Error() ;
   floats_[varZ0Err] = track->dzError() ;
   floats_[varChi2] = track->chi2() ;
   floats_[varNdof] = track->ndof();
   ints_[varValidHits] = track->numberOfValidHits() ;
   ints_[varlostHits] = track->numberOfLostHits() ;
   floats_[varEtaErr] = track->etaError() ;
   floats_[varPhiErr] = track->phiError() ;
}

void cms1::TrackStreamer::fill( const StreamerArguments& args )
{
   setDefaults();
   
   // fill MC
   if ( args.genParticle ) {
      HepLorentzVector p = args.genParticle->momentum();
      p4s_[varMCP4] = LorentzVector(p.px(),p.py(),p.pz(),p.e());
      ints_[varPdgId] = args.genParticle->pdg_id();
   }
   // reco::Track has priority over reco::Candidate
   if ( args.track ) {
      fill(args.track);
      return;
   }
   if ( args.candidate ) {
      fill(args.candidate);
      return;
   }
}

