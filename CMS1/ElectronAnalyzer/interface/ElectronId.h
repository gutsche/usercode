#ifndef ElectronId_h
#define ElectronId_h

//
// Package:         CMS1/ElectronAnalyzer
// Class:           ElectronId

// 
// Description:     EDAnalyzer producing ntuples for ElectronId
//
// Original Author: Matteo Sani, matteo.sani@cern.ch


#include "CMS1/BaseAnalyzer/interface/BaseAnalyzer.h"
#include "CMS1/Electrons/interface/Electrons.h"
#include "CMS1/ElectronAnalyzer/interface/ElIdUserBlock.h"
#include "CMS1/Base/interface/Cuts.h"

namespace cms1 {

  class ElectronId: public BaseAnalyzer {
  public:
    ElectronId(){}
    virtual ~ElectronId() {}
     

  protected:
    void configure(const edm::ParameterSet& iConfig);
          
    // process event using EventData
    virtual void processEvent(const edm::Event&);
    
    // finish process
    virtual void finishProcessing();
  private:
    ElIdUserBlock elidUserData;
    UserDataInt* nCandidates;

  };
}

#endif
