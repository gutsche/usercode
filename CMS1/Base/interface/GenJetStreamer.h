#ifndef CMS1_GenJetStreamer_h
#define CMS1_GenJetStreamer_h
// Description:     GenJet streamer class 
//
// Original Author: Dmytro Kovalskyi
//
// $Author: dmytro $
// $Date: 2007/05/23 02:23:23 $
// $Revision: 1.2 $
//
#include "CMS1/Base/interface/BaseStreamer.h"
#include "DataFormats/JetReco/interface/GenJet.h"
namespace cms1 {
   class GenJetStreamer: public BaseStreamer
     {
      public:
	enum FloatVars { varEmEnergy, varHadEnergy, varInvEnergy, varOtherEnergy };
	enum P4Vars    { varP4 };

	GenJetStreamer();
	void setDefaults();
	void fill( const reco::GenJet* );
     };
}

#endif
