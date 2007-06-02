#ifndef CMS1_GenParticleStreamer_h
#define CMS1_GenParticleStreamer_h
// Description:     GenParticle streamer class 
//
// Original Author: Dmytro Kovalskyi
//
// $Author: dmytro $
// $Date: 2007/05/23 02:23:23 $
// $Revision: 1.2 $
//
#include "CMS1/Base/interface/BaseStreamer.h"
#include "CLHEP/HepMC/GenParticle.h"
namespace cms1 {
   class GenParticleStreamer: public BaseStreamer
     {
      public:
	enum IntVars   { varPdgId };
	enum P4Vars    { varP4, varProdVertex, varEndVertex };
	     
	GenParticleStreamer();
	void setDefaults();
	void fill( const HepMC::GenParticle* );
     };
}

#endif
