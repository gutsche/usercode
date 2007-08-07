#ifndef CMS1_JetStreamer_h
#define CMS1_JetStreamer_h
// Description:     jet streamer class 
//
// Original Author: Dmytro Kovalskyi
//
// $Author: kalavase $
// $Date: 2007/06/05 01:07:14 $
// $Revision: 1.2 $
//
#include "CMS1/Base/interface/BaseStreamer.h"
namespace cms1 {
   class JetStreamer: public BaseStreamer
     {
      public:
	int *varMCId;
	float *varEmFrac, *varChFrac, *varMCEmEnergy, *varMCHadEnergy, 
	  *varMCInvEnergy, *varMCOtherEnergy, *varCor;
	LorentzVector *varP4, *varMCP4, *varMCparticleP4;
	
	JetStreamer();
	void fill( const reco::Candidate* candidate, bool reset = true);
	void fill( const reco::CaloJet* jet, bool reset = true);
	void fill( const StreamerArguments args, bool reset = true);
     };
}

#endif
