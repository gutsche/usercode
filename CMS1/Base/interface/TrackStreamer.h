#ifndef CMS1_TrackStreamer_h
#define CMS1_TrackStreamer_h
// Description:     track streamer class 
//
// Original Author: Dmytro Kovalskyi
//
// $Author: dmytro $
// $Date: 2007/07/06 07:56:11 $
// $Revision: 1.4 $
//
#include "CMS1/Base/interface/BaseStreamer.h"
namespace cms1 {
   class TrackStreamer: public BaseStreamer
     {
      public:
	TrackStreamer();
	void setMass( float m ){ mass_ = m; }
	void fill( const reco::Candidate* candidate, bool reset = true );
	void fill( const reco::Track* track, bool reset = true );
	void fill( const StreamerArguments& args, bool reset = true );
      protected:
	float mass_;
	int *varValidHits, *varlostHits, *varPdgId ;
	float *varD0, *varZ0, *varVertexPhi, *varChi2, *varNdof, *varD0Err, *varZ0Err, 
	  *varPtErr, *varEtaErr, *varPhiErr;
	LorentzVector *varP4, *varTrkP4, *varMCP4;
     };
}

#endif
