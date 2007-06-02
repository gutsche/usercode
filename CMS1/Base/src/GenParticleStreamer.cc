// Description:     GenParticle streamer class 
//
// Original Author: Dmytro Kovalskyi
//
// $Author: dmytro $
// $Date: 2007/05/23 02:23:23 $
// $Revision: 1.2 $
//
#include "CLHEP/HepMC/GenVertex.h"
#include "CMS1/Base/interface/GenParticleStreamer.h"
cms1::GenParticleStreamer::GenParticleStreamer() 
{
   // ORDER IS CRITICAL !
   p4Names_.push_back("p4");            p4s_.push_back( LorentzVector(0,0,0,0) );
   // p4Names_.push_back("prod_vtx");      p4s_.push_back( LorentzVector(0,0,0,0) );
   // p4Names_.push_back("end_vtx");       p4s_.push_back( LorentzVector(0,0,0,0) );
   intNames_.push_back("id");           ints_.push_back(0);
}

void cms1::GenParticleStreamer::setDefaults()
{
   p4s_[varP4]   = LorentzVector(0,0,0,0);
   // p4s_[varProdVertex] = LorentzVector(0,0,0,0);
   // p4s_[varEndVertex] = LorentzVector(0,0,0,0);
   ints_[varPdgId] = 0 ; 
}

void cms1::GenParticleStreamer::fill( const HepMC::GenParticle* p )
{
   if (! p) {
      setDefaults();
      return;
   }

   p4s_[varP4] = LorentzVector(p->momentum().px(), 
			       p->momentum().py(), 
			       p->momentum().pz(), 
			       p->momentum().e());
   ints_[varPdgId] = p->pdg_id();
   
   /*
   HepMC::GenVertex* pVertex = p->production_vertex();
   if (pVertex)
     p4s_[varProdVertex] = LorentzVector(pVertex->position().px(),
					 pVertex->position().py(),
					 pVertex->position().pz(),
					 pVertex->position().e());
   else
     p4s_[varProdVertex] = LorentzVector(0,0,0,0);
   
   HepMC::GenVertex* eVertex = p->end_vertex();
   if (eVertex)
     p4s_[varEndVertex] =  LorentzVector(eVertex->position().px(),
					 eVertex->position().py(),
					 eVertex->position().pz(),
					 eVertex->position().e());
   else
     p4s_[varEndVertex] =  LorentzVector(0,0,0,0);
    */
     
}
