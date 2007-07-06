#ifndef CMS1_TrackStreamer_h
#define CMS1_TrackStreamer_h
// Description:     track streamer class 
//
// Original Author: Dmytro Kovalskyi
//
// $Author: dmytro $
// $Date: 2007/07/03 22:44:25 $
// $Revision: 1.3 $
//
#include "CMS1/Base/interface/BaseStreamer.h"
namespace cms1 {
   class TrackStreamer: public BaseStreamer
     {
      public:
	enum IntVars   { varValidHits, varlostHits, varPdgId };
	//added varVertPhi - PDK
	enum FloatVars { varD0, varZ0, varVertexPhi, varChi2, varNdof, varD0Err, varZ0Err, varPtErr, varEtaErr, varPhiErr };
	enum P4Vars    { varP4, varTrkP4, varMCP4 };
	     
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
