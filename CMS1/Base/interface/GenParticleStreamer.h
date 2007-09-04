#ifndef CMS1_GenParticleStreamer_h
#define CMS1_GenParticleStreamer_h
// Description:     GenParticle streamer class 
//
// Original Author: Dmytro Kovalskyi
//
// $Author: dmytro $
// $Date: 2007/08/07 11:13:29 $
// $Revision: 1.2 $
//
#include "CMS1/Base/interface/BaseStreamer.h"
#include "CLHEP/HepMC/GenParticle.h"
namespace cms1 {
   class GenParticleStreamer: public BaseStreamer
     {
      public:
	int *varPdgId, *varPdgMother;
	LorentzVector *varP4, *varProdVertex, *varEndVertex;
	     
	GenParticleStreamer();
	void fill( const HepMC::GenParticle*, bool reset = true );
     };
}

#endif
