#ifndef FakeRateNtupleMaker_h
#define FakeRateNtupleMaker_h

//
// Package:         CMS1/FakeRateNtupleMaker
// Class:           FakeRateNtupleMaker

// 
// Description:     EDAnalyzer filling table for di-lepton analysis dependent on number of jets
//
// $Author: dmytro $
// $Date: 2007/08/30 14:16:14 $
// $Revision: 1.1 $
//

#include <vector>
#include "CMS1/BaseAnalyzer/interface/BaseAnalyzer.h"

#include "CMS1/Muons/interface/Muons.h"
#include "CMS1/Electrons/interface/Electrons.h"
#include "CMS1/Jets/interface/Jets.h"
#include "CMS1/MET/interface/MET.h"
#include "CMS1/Base/interface/Cuts.h"
#include "CMS1/Base/interface/UserDataBlocks.h"
#include "CMS1/Base/interface/JetStreamer.h"

namespace cms1 {

  class FakeRateNtupleMaker: public BaseAnalyzer {
  public:
    FakeRateNtupleMaker(){}
    virtual ~FakeRateNtupleMaker(){}
     

  protected:
     // analyzer configuration code
     virtual void configure(const edm::ParameterSet& iConfig);
	
     // process event using EventData
     virtual void processEvent(const edm::Event& );
    
     // finish process
     virtual void finishProcessing();
         
     // input types
     std::string  tightMuonType;
     std::string  looseMuonType;
     std::string  metMuonType; 
     std::string  tightElectronType; 
     std::string  looseElectronType;
     std::string  jetType;
     std::string  correctedJetType;
     std::string  referenceJetType;
  private:
    VectorUserBlock<JetStreamer> jetsUserBlock_;
    UserDataInt*                 nJets_;
    VectorUserBlock<JetStreamer> correctedJetsUserBlock_;
    UserDataInt*                 nCorrectedJets_;
    std::string                  correctedJetType_;
    std::string                  referenceJetType_;

  };
}

#endif
