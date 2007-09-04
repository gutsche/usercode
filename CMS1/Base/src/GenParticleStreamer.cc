// Description:     GenParticle streamer class 
//
// Original Author: Dmytro Kovalskyi
//
// $Author: sani $
// $Date: 2007/09/04 10:43:53 $
// $Revision: 1.3 $
//
#include "CLHEP/HepMC/GenVertex.h"
#include "CMS1/Base/interface/GenParticleStreamer.h"
cms1::GenParticleStreamer::GenParticleStreamer() 
{
   varP4 = addP4("p4", "p4", LorentzVector(0,0,0,0) );
   varProdVertex = addP4("prod_vtx", " Production Vertex of the particle", LorentzVector(0,0,0,0));
   // p4Names_.push_back("end_vtx");       p4s_.push_back( LorentzVector(0,0,0,0) );
   varPdgId = addInt("id", " PDG id", 0);
   varPdgMother = addInt("id_mother", " PDG id of the mother", 0);
}

void cms1::GenParticleStreamer::fill( const HepMC::GenParticle* p, bool reset) {

   if (reset) 
     setDefaults();

   if (!p) 
     return;

   *varP4 = LorentzVector(p->momentum().px(), 
			  p->momentum().py(), 
			  p->momentum().pz(), 
			  p->momentum().e());
   *varPdgId = p->pdg_id();
   
   HepMC::GenVertex* pVertex = p->production_vertex();

   int motherid = 0;
   
   if (pVertex) {
     *varProdVertex = LorentzVector(pVertex->position().px(),
                                    pVertex->position().py(),
                                    pVertex->position().pz(),
                                    pVertex->position().e());
     
     if (pVertex->particles_in_size() > 0) {
       std::set<HepMC::GenParticle*>::const_iterator iter = pVertex->particles_in_const_begin();
       motherid = (*iter)->pdg_id();
     }
   }  
   *varPdgMother = motherid;

   /*
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
