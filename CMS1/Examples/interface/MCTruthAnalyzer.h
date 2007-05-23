#ifndef CMS1_MCTruthAnalyzer_h
#define CMS1_MCTruthAnalyzer_h

//
// Package:         CMS1/CommonTools
// Class:           MCTruthAnalyzer
// 
// Description:     working example of MC Truth 
//
// Original Author: Matteo Sani
//
#include <vector>
#include "CMS1/BaseAnalyzer/interface/BaseAnalyzer.h"

namespace cms1 {
   
  class MCTruthAnalyzer: public BaseAnalyzer {
  public:
    MCTruthAnalyzer() {
      pt_=0;               //usercode
      number_=0;           //usercode
    }
    virtual ~MCTruthAnalyzer(){}
  protected:
    // User configuration code lives here
    // Base class is called first externally to get
    // black boxes setup
    virtual void configure(const edm::ParameterSet&);
    
    // User code to process event lives here
    // Base class is called first externally to get
    // black boxes running
    virtual void processEvent(const edm::Event&);
    
  private:
    // analysis variables
    double pt_;              //usercode
    unsigned int number_;    //usercode
  };
}
#endif
