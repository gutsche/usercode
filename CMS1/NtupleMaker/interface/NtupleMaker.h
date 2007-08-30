#ifndef NtupleMaker_h
#define NtupleMaker_h

//
// Package:         CMS1/NtupleMaker
// Class:           NtupleMaker

// 
// Description:     EDAnalyzer filling table for di-lepton analysis dependent on number of jets
//
// $Author: dmytro $
// $Date: 2007/08/04 22:13:07 $
// $Revision: 1.22 $
//

#include <vector>
#include "CMS1/BaseAnalyzer/interface/BaseAnalyzer.h"

#include "CMS1/Muons/interface/Muons.h"
#include "CMS1/Electrons/interface/Electrons.h"
#include "CMS1/Jets/interface/Jets.h"
#include "CMS1/MET/interface/MET.h"
#include "CMS1/EventHyp/interface/DiLeptonCandidate.h"
#include "CMS1/EventHyp/interface/DiLeptonUserBlock.h"
#include "CMS1/Base/interface/Cuts.h"

namespace cms1 {

  class NtupleMaker: public BaseAnalyzer {
  public:
    NtupleMaker(){}
    virtual ~NtupleMaker(){}
     

  protected:
     // analyzer configuration code
     virtual void configure(const edm::ParameterSet& iConfig);
	
     // process event using EventData
     virtual void processEvent(const edm::Event& );
    
     // finish process
     virtual void finishProcessing();
    
     DiLeptonUserBlock diLeptonUserData;
     UserDataInt* nCandidates;
     UserDataFloat* crossSection;
     double crossSectionValue;
     
     // input types
     std::string  tightMuonType;
     std::string  looseMuonType;
     std::string  metMuonType; 
     std::string  tightElectronType; 
     std::string  looseElectronType;
     std::string  jetType;
     std::string  correctedJetType;
     std::string  referenceJetType;
  };
}

#endif
