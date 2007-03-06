#ifndef CMS1JetsWithoutElectrons_h
#define CMS1JetsWithoutElectrons_h

//
// Package:         CMS1/Jets
// Class:           JetsWithoutElectrons
// 
// Description:     analysis functions for jet without wlectrons
//
// Original Author: Matteo Sani
//

#include "DataFormats/EgammaCandidates/interface/SiStripElectron.h"
#include "DataFormats/JetReco/interface/CaloJet.h"

#include<vector>

namespace cms1 {
  class JetsWithoutElectrons  {
  public:

    JetsWithoutElectrons(std::vector<const reco::CaloJet*> jets) : jetCollection_(jets)
      {};
    
    ~JetsWithoutElectrons() 
      {};
    
    // This is the function that removes the "electrons"
    std::vector<const reco::CaloJet*> removeElectrons(const reco::SiStripElectron* e1=0, const reco::SiStripElectron* e2=0, double coneSize=0.4) const; 
      
  private:
    //JetCollection;
    std::vector<const reco::CaloJet*> jetCollection_;
  };
}

#endif
