// Description:     GenParticle streamer class 
//
// Original Author: Dmytro Kovalskyi
//
// $Author: dmytro $
// $Date: 2007/08/07 11:13:30 $
// $Revision: 1.2 $
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

   std::cout << "POINT1" << std::endl;
   *varP4 = LorentzVector(p->momentum().px(), 
			  p->momentum().py(), 
			  p->momentum().pz(), 
			  p->momentum().e());
   *varPdgId = p->pdg_id();
   
   std::cout << "POINT2" << std::endl;
   HepMC::GenVertex* pVertex = p->production_vertex();
   std::cout << "POINT3" << std::endl;
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
   std::cout << "POINT4" << std::endl;
   *varPdgMother = motherid;
   std::cout << "POINT5:" << motherid << std::endl;
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
