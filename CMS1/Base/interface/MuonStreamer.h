#ifndef CMS1_MuonStreamer_h
#define CMS1_MuonStreamer_h
// Description:     track streamer class 
//
// Original Author: Dmytro Kovalskyi
//
// $Author: dmytro $
// $Date: 2007/08/07 11:13:29 $
// $Revision: 1.1 $
//
#include "CMS1/Base/interface/TrackStreamer.h"
namespace cms1 {
   class MuonStreamer: public TrackStreamer
     {
      public:
	MuonStreamer();
	void fill( const reco::Candidate* candidate, bool reset = true );
	void fill( const StreamerArguments& args, bool reset = true );
      protected:
	int *varNMatches, *varIsoR03_ntrk, *varIsoR05_ntrk;
	int *var_pid_TMLastStationLoose, *var_pid_TMLastStationTight, 
	  *var_pid_TM2DCompatibilityLoose, *var_pid_TM2DCompatibilityTight;
	float *varEnergy_em, *varEnergy_had, *varEnergy_ho,  *varEnergy_emS9, *varEnergy_hadS9, *varEnergy_hoS9,
	  *varIsoR03_sumPt, *varIsoR03_emEt, *varIsoR03_hadEt, *varIsoR03_hoEt,
	  *varIsoR05_sumPt, *varIsoR05_emEt, *varIsoR05_hadEt, *varIsoR05_hoEt;
	float *var_gfit_chi2, *var_gfit_ndof;
	int *var_gfit_validHits;
     };
}

#endif
