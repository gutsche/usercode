// Description:     generated jet streamer class 
//
// Original Author: Dmytro Kovalskyi
//
// $Author: dmytro $
// $Date: 2007/06/02 18:03:49 $
// $Revision: 1.1 $
//
#include "CMS1/Base/interface/GenJetStreamer.h"
cms1::GenJetStreamer::GenJetStreamer() 
{
   varP4 = addP4("p4", "", LorentzVector(0,0,0,0) );
   varEmEnergy = addFloat("emEnergy", " energy of electromagnetic particles", -999);
   varHadEnergy = addFloat("hadEnergy", " energy of hadronic particles", -999);
   varInvEnergy = addFloat("invEnergy", " invisible energy", -999);
   varOtherEnergy = addFloat("otherEnergy", " other energy (undecayed Sigmas etc.)", -999);
}

void cms1::GenJetStreamer::fill( const reco::GenJet* jet, bool reset )
{
   if (reset) setDefaults();
   
   *varP4 = jet->p4();
   *varEmEnergy = jet->emEnergy();
   *varHadEnergy = jet->hadEnergy();
   *varInvEnergy = jet->invisibleEnergy();
   *varOtherEnergy = jet->auxiliaryEnergy();
}
