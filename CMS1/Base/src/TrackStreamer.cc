// Description:     track streamer class 
//
// Original Author: Dmytro Kovalskyi
//
// $Author: dmytro $
// $Date: 2007/08/07 11:13:31 $
// $Revision: 1.8 $
//
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"
#include "CMS1/Base/interface/TrackStreamer.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"

cms1::TrackStreamer::TrackStreamer() 
{
   varP4        = addP4("p4",           " candidate p4", LorentzVector(0,0,0,0) );
   varTrkP4     = addP4("trk_p4",       " track p4", LorentzVector(0,0,0,0) );
   varD0        = addFloat("d0",        " impact parameter at the point of closest approach", -999);
   varZ0        = addFloat("z0",        " z position of the point of closest approach", -999);
   varVertexPhi = addFloat("vertexphi", " phi angle of the point of closest approach", -999);
   varChi2      = addFloat("chi2",      " chi2 of the silicon tracker fit", -999);
   varNdof      = addFloat("ndof",      " number of degrees of freedom of the fit", -999);
   varValidHits = addInt("validHits",   " number of used hits in the fit", -999);
   varlostHits  = addInt("lostHits",    " number of lost hits in the fit", -999);
   varD0Err     = addFloat("d0Err",     " error on the impact parameter", -999);
   varZ0Err     = addFloat("z0Err",     " error on z position of the point of closest approach", -999);
   varPtErr     = addFloat("ptErr",     " track Pt error", -999);
   varEtaErr    = addFloat("etaErr",    " track eta error", -999);
   varPhiErr    = addFloat("phiErr",    " track phi error", -999);
   // truth matching
   varMCP4      = addP4("mc_p4", " p4 of matched MC particle", LorentzVector(0,0,0,0) );
   varPdgId     = addInt("mc_id", " PDG id of matched MC particle", 0 );
   varMotherId  = addInt("mc_motherid", " PDG id of the mother of the particle", -1);
   
   mass_ = 0;
}

void cms1::TrackStreamer::fill( const reco::Candidate* candidate, bool reset )
{
   if ( reset ) setDefaults();
   if (! candidate) return;
   if ( const reco::Muon* muon = dynamic_cast<const reco::Muon*>(candidate) ) fill(muon->track().get(),false);
   if ( const reco::PixelMatchGsfElectron* elec = dynamic_cast<const reco::PixelMatchGsfElectron*>(candidate) ) 
     fill( (const reco::Track*)elec->gsfTrack().get(), false );
   *varP4 = candidate->p4();
}

void cms1::TrackStreamer::fill( const reco::Track* track, bool reset ) 
{
   if ( reset ) setDefaults();
   if ( ! track ) return;
   *varTrkP4 = LorentzVector( track->px(), track->py(), track->pz(), sqrt(track->p()*track->p()+mass_*mass_) );
   *varD0 = track->d0() ;
   *varZ0 = track->dz() ;
   *varVertexPhi = atan2(track->vy(), track->vx());
   *varD0Err = track->d0Error() ;
   *varZ0Err = track->dzError() ;
   *varChi2 = track->chi2() ;
   *varNdof = track->ndof();
   *varValidHits = track->numberOfValidHits() ;
   *varlostHits = track->numberOfLostHits() ;
   *varEtaErr = track->etaError() ;
   *varPhiErr = track->phiError() ;

   float pt = track->pt();
   float p = track->p();
   float q = track->charge();
   float pz = track->pz();
   float err = (track->charge()!=0) ? sqrt(pt*pt*p*p/pow(q, 2)*(track->covariance(0,0))
					   +2*pt*p/q*pz*(track->covariance(0,1))
					   + pz*pz*(track->covariance(1,1) ) )
                                           : -999.;
   *varPtErr = err;
}

void cms1::TrackStreamer::fill( const StreamerArguments& args, bool reset )
{
   if (reset) 
     setDefaults();
   // fill MC
   if ( args.genParticle ) {
     HepLorentzVector m = args.genParticle->momentum();
     *varMCP4 = LorentzVector(m.px(), m.py(), m.pz(), m.e());
     *varPdgId = args.genParticle->pdg_id();

     const HepMC::GenParticle* p = args.genParticle;
     int motherid = -1;

     while ((p->production_vertex()) && (motherid == -1)) {
       HepMC::GenVertex* inVertex = p->production_vertex();
       for(std::set<HepMC::GenParticle*>::const_iterator iter = inVertex->particles_in_const_begin();
           iter != inVertex->particles_in_const_end(); ++iter) {
         if ((*iter)->pdg_id() != p->pdg_id()) {
           motherid = (*iter)->pdg_id();
           break;
         } else {
           p = *iter;
           break;
         }
       }
     }

     *varMotherId = motherid;
   }

   if ( args.track ) 
     fill( args.track, false );
   if ( args.candidate ) 
     fill( args.candidate, false );
}

