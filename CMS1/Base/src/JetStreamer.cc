// Description:     jet streamer class 
//
// Original Author: Dmytro Kovalskyi
//
// $Author: dmytro $
// $Date: 2007/05/22 07:12:39 $
// $Revision: 1.1 $
//
#include "CMS1/Base/interface/JetStreamer.h"
cms1::JetStreamer::JetStreamer() 
{
   p4Names_.push_back("p4");                 p4s_.push_back( LorentzVector(0,0,0,0) );
   p4Names_.push_back("mc_p4");              p4s_.push_back( LorentzVector(0,0,0,0) );
   p4Names_.push_back("mc_gp_p4");           p4s_.push_back( LorentzVector(0,0,0,0) );
   floatNames_.push_back("emFrac");          floats_.push_back(0);
   floatNames_.push_back("chFrac");          floats_.push_back(0);
   floatNames_.push_back("mc_emEnergy");     floats_.push_back(0);
   floatNames_.push_back("mc_hadEnergy");    floats_.push_back(0);
   floatNames_.push_back("mc_invEnergy");    floats_.push_back(0);
   floatNames_.push_back("mc_otherEnergy");  floats_.push_back(0);
   intNames_.push_back("mc_id");             ints_.push_back(0);
}

void cms1::JetStreamer::setDefaults()
{
   p4s_[varP4] = LorentzVector(0,0,0,0);
   p4s_[varMCP4] = LorentzVector(0,0,0,0);
   p4s_[varMCparticleP4] = LorentzVector(0,0,0,0);
   floats_[varEmFrac] = -999. ;
   floats_[varChFrac] = -999. ;
   floats_[varMCEmEnergy] = -999. ;
   floats_[varMCHadEnergy] = -999. ;
   floats_[varMCInvEnergy] = -999. ;
   floats_[varMCOtherEnergy] = -999. ;
   ints_[varMCId] = -999;
}

void cms1::JetStreamer::fill( const reco::Candidate* candidate ) 
{
   if (! candidate) {
      setDefaults();
      return;
   }
   if ( const reco::CaloJet* jet = dynamic_cast<const reco::CaloJet*>(candidate) ) {
      fill(jet);
      return;
   }
   p4s_[varP4] = candidate->p4();
}

void cms1::JetStreamer::fill( const reco::CaloJet* jet )
{
   if ( ! jet ){
      setDefaults();
      return;
   }
   p4s_[varP4] = jet->p4();
   floats_[varEmFrac] = jet->emEnergyFraction() ;
   floats_[varChFrac] = -999.;
}

void cms1::JetStreamer::fill( const StreamerArguments args )
{
   setDefaults();
   
   // fill MC
   if ( args.genJet ) {
      p4s_[varMCP4] = args.genJet->p4();
      floats_[varMCEmEnergy] = args.genJet->emEnergy();
      floats_[varMCHadEnergy] = args.genJet->hadEnergy();
      floats_[varMCInvEnergy] = args.genJet->invisibleEnergy();
      floats_[varMCOtherEnergy] = args.genJet->auxiliaryEnergy();
   }
   if ( args.genParticle ) {
      ints_[varMCId] = args.genParticle->pdg_id();
      HepLorentzVector p = args.genParticle->momentum();
      p4s_[varMCparticleP4] = LorentzVector(p.px(),p.py(),p.pz(),p.e());
   }
   if ( args.candidate ) {
      fill(args.candidate);
      return;
   }
}
