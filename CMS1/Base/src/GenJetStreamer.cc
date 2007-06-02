// Description:     generated jet streamer class 
//
// Original Author: Dmytro Kovalskyi
//
// $Author: dmytro $
// $Date: 2007/05/23 02:23:23 $
// $Revision: 1.2 $
//
#include "CMS1/Base/interface/GenJetStreamer.h"
cms1::GenJetStreamer::GenJetStreamer() 
{
   p4Names_.push_back("p4");              p4s_.push_back( LorentzVector(0,0,0,0) );
   floatNames_.push_back("emEnergy");     floats_.push_back(0);
   floatNames_.push_back("hadEnergy");    floats_.push_back(0);
   floatNames_.push_back("invEnergy");    floats_.push_back(0);
   floatNames_.push_back("otherEnergy");  floats_.push_back(0);
}

void cms1::GenJetStreamer::setDefaults()
{
   p4s_[varP4] = LorentzVector(0,0,0,0);
   floats_[varEmEnergy] = -999. ;
   floats_[varHadEnergy] = -999. ;
   floats_[varInvEnergy] = -999. ;
   floats_[varOtherEnergy] = -999. ;
}

void cms1::GenJetStreamer::fill( const reco::GenJet* jet )
{
   setDefaults();
   
   p4s_[varP4] = jet->p4();
   floats_[varEmEnergy] = jet->emEnergy();
   floats_[varHadEnergy] = jet->hadEnergy();
   floats_[varInvEnergy] = jet->invisibleEnergy();
   floats_[varOtherEnergy] = jet->auxiliaryEnergy();
}
