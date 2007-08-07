// Description:     jet streamer class 
//
// Original Author: Dmytro Kovalskyi
//
// $Author: dmytro $
// $Date: 2007/07/06 19:22:54 $
// $Revision: 1.4 $
//
#include "CMS1/Base/interface/JetStreamer.h"
cms1::JetStreamer::JetStreamer() 
{
   varP4            = addP4( "p4", "", LorentzVector(0,0,0,0) );
   varMCP4          = addP4( "mc_p4", " p4 of the matched GenJet", LorentzVector(0,0,0,0) );
   varMCparticleP4  = addP4( "mc_gp_p4", " p4 of the matched MC particle", LorentzVector(0,0,0,0) );
   varEmFrac        = addFloat("emFrac", " electromagnetic energy fraction", -999);
   varChFrac        = addFloat("chFrac", " charged track energy fraction", -999);
   varMCEmEnergy    = addFloat("mc_emEnergy",    " energy of electromagnetic particles of the matched GenJet", -999);
   varMCHadEnergy   = addFloat("mc_hadEnergy",   " energy of hadronic particles of the matched GenJet", -999);
   varMCInvEnergy   = addFloat("mc_invEnergy",   " invisible energy of the matched GenJet", -999);
   varMCOtherEnergy = addFloat("mc_otherEnergy", " other energy (undecayed Sigmas etc.) of the matched GenJet", -999);
   varCor           = addFloat("cor", " energy scale correction", -999);
   varMCId          = addInt("mc_id", " PDG id of the matched MC particle", 0);
}

void cms1::JetStreamer::fill( const reco::Candidate* candidate, bool reset )
{
   if ( reset ) setDefaults();
   if (! candidate) return;
   if ( const reco::CaloJet* jet = dynamic_cast<const reco::CaloJet*>(candidate) ) fill(jet,false);
   *varP4 = candidate->p4();
}

void cms1::JetStreamer::fill( const reco::CaloJet* jet, bool reset )
{
   if ( reset ) setDefaults();
   if ( ! jet ) return;
   *varP4 = jet->p4();
   *varEmFrac = jet->emEnergyFraction() ;
   *varChFrac = -999.;
}

void cms1::JetStreamer::fill( const StreamerArguments args, bool reset )
{
   if ( reset ) setDefaults();
   // fill MC
   if ( args.genJet ) {
      *varMCP4 = args.genJet->p4();
      *varMCEmEnergy = args.genJet->emEnergy();
      *varMCHadEnergy = args.genJet->hadEnergy();
      *varMCInvEnergy = args.genJet->invisibleEnergy();
      *varMCOtherEnergy = args.genJet->auxiliaryEnergy();
   }
   if ( args.genParticle ) {
      *varMCId = args.genParticle->pdg_id();
      HepLorentzVector p = args.genParticle->momentum();
      *varMCparticleP4 = LorentzVector(p.px(),p.py(),p.pz(),p.e());
   }
   *varCor=args.jetcorrection;
   if ( args.candidate ) fill(args.candidate, false);
}
