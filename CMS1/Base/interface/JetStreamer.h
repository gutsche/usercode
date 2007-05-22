#ifndef CMS1_JetStreamer_h
#define CMS1_JetStreamer_h
// Description:     jet streamer class 
//
// Original Author: Dmytro Kovalskyi
//
// $Author: dmytro $
// $Date: 2007/05/11 04:08:51 $
// $Revision: 1.1 $
//
#include "CMS1/Base/interface/BaseStreamer.h"
namespace cms1 {
   class JetStreamer: public BaseStreamer
     {
      public:
	enum IntVars   { varMCId };
	enum FloatVars { varEmFrac, varChFrac, varMCEmEnergy, varMCHadEnergy, varMCInvEnergy, varMCOtherEnergy };
	enum P4Vars    { varP4, varMCP4, varMCparticleP4 };
	
	JetStreamer();
	void setDefaults();
	void fill( const reco::Candidate* candidate );
	void fill( const reco::CaloJet* jet );
	void fill( const StreamerArguments args);
     };
}

#endif
