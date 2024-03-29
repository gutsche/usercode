#ifndef CMS1Jets_h
#define CMS1Jets_h

//
// Package:         CMS1/Jets
// Class:           Jets
// 
// Description:     analysis functions for jets
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Wed Feb 21 00:15:42 UTC 2007
//
// $Author: dmytro $
// $Date: 2007/07/27 06:59:58 $
// $Revision: 1.12 $
//

#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"
#include "CMS1/Base/interface/Cuts.h"
#include "CMS1/Base/interface/BlackBox.h"
#include "CMS1/Base/interface/UserDataBlocks.h"
#include "CMS1/Base/interface/JetStreamer.h"

#include "DataFormats/Math/interface/LorentzVector.h"
#include "Math/VectorUtil.h"

namespace cms1 {
    class Jets: public BlackBox  {
  public:
       Jets():BlackBox(){}
       
       std::vector<const reco::Candidate*> getJets ( const std::string jetType, const Cuts& = Cuts() );
       void dump(std::ostream& o, std::vector<const reco::Candidate*>);
       void registerEventUserData();
       void fillEventUserData();
     private:
       VectorUserBlock<JetStreamer> evtJets;
       UserDataInt*                 nJets;
  };
}

#endif
