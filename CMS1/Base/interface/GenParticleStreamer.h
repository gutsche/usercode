#ifndef CMS1_GenParticleStreamer_h
#define CMS1_GenParticleStreamer_h
// Description:     GenParticle streamer class 
//
// Original Author: Dmytro Kovalskyi
//
// $Author: dmytro $
// $Date: 2007/06/02 18:03:48 $
// $Revision: 1.1 $
//
#include "CMS1/Base/interface/BaseStreamer.h"
#include "CLHEP/HepMC/GenParticle.h"
namespace cms1 {
   class GenParticleStreamer: public BaseStreamer
     {
      public:
	int *varPdgId;
	LorentzVector *varP4, *varProdVertex, *varEndVertex;
	     
	GenParticleStreamer();
	void fill( const HepMC::GenParticle*, bool reset = true );
     };
}

#endif
