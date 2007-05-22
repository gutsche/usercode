#ifndef CMS1_TrackStreamer_h
#define CMS1_TrackStreamer_h
// Description:     track streamer class 
//
// Original Author: Dmytro Kovalskyi
//
// $Author: dmytro $
// $Date: 2007/05/11 04:08:51 $
// $Revision: 1.1 $
//
#include "CMS1/Base/interface/BaseStreamer.h"
namespace cms1 {
   class TrackStreamer: public BaseStreamer
     {
      public:
	enum IntVars   { varValidHits, varlostHits, varPdgId };
	enum FloatVars { varD0, varZ0, varChi2, varNdof };
	enum P4Vars    { varP4, varMCP4 };
	     
	TrackStreamer();
	void setMass( float m ){ mass_ = m; }
	void setDefaults();
	void fill( const reco::Candidate* candidate );
	void fill( const reco::Track* track );
	void fill( const StreamerArguments& args);
      protected:
	float mass_;
     };
}

#endif
