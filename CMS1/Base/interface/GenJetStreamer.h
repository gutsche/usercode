#ifndef CMS1_GenJetStreamer_h
#define CMS1_GenJetStreamer_h
// Description:     GenJet streamer class 
//
// Original Author: Dmytro Kovalskyi
//
// $Author: dmytro $
// $Date: 2007/06/02 18:03:48 $
// $Revision: 1.1 $
//
#include "CMS1/Base/interface/BaseStreamer.h"
#include "DataFormats/JetReco/interface/GenJet.h"
namespace cms1 {
   class GenJetStreamer: public BaseStreamer
     {
      public:
	float *varEmEnergy, *varHadEnergy, *varInvEnergy, *varOtherEnergy;
	LorentzVector *varP4;

	GenJetStreamer();
	void fill( const reco::GenJet*, bool reset = true );
     };
}

#endif
