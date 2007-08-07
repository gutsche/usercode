// Description:     GenParticle streamer class 
//
// Original Author: Dmytro Kovalskyi
//
// $Author: dmytro $
// $Date: 2007/06/02 18:03:49 $
// $Revision: 1.1 $
//
#include "CLHEP/HepMC/GenVertex.h"
#include "CMS1/Base/interface/GenParticleStreamer.h"
cms1::GenParticleStreamer::GenParticleStreamer() 
{
   varP4 = addP4("p4", "p4", LorentzVector(0,0,0,0) );
   // p4Names_.push_back("prod_vtx");      p4s_.push_back( LorentzVector(0,0,0,0) );
   // p4Names_.push_back("end_vtx");       p4s_.push_back( LorentzVector(0,0,0,0) );
   varPdgId = addInt("id", " PDG id", 0);
}

void cms1::GenParticleStreamer::fill( const HepMC::GenParticle* p, bool reset )
{
   if (reset) setDefaults();
   if (! p) return;
   *varP4 = LorentzVector(p->momentum().px(), 
			  p->momentum().py(), 
			  p->momentum().pz(), 
			  p->momentum().e());
   *varPdgId = p->pdg_id();
   
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
