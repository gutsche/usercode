//
// Package:         CMS1/Jets
// Class:           JetsWithoutElectrons
// 
// Description:     functions for jets without electrons
//
// Original Author: Matteo Sani
//

#include "CMS1/Jets/interface/JetsWithoutElectrons.h"

#include "DataFormats/Math/interface/LorentzVector.h"
#include <Math/VectorUtil.h>

std::vector<const reco::CaloJet*> cms1::JetsWithoutElectrons::removeElectrons(const reco::SiStripElectron* e1, const reco::SiStripElectron* e2, double coneSize) const {

  std::vector<const reco::CaloJet*> output_list;
  for(std::vector<const reco::CaloJet*>::const_iterator itJet = jetCollection_.begin(); itJet != jetCollection_.end(); ++itJet) {

    bool isMatched = false;
    
    if (e1 != 0) {
      double dR = ROOT::Math::VectorUtil::DeltaR(e1->p4(), (*itJet)->p4());
      if (dR < coneSize) {
        isMatched = true;
      }
    }

    if (e2 != 0) {
      double dR = ROOT::Math::VectorUtil::DeltaR(e2->p4(), (*itJet)->p4());
      if (dR < coneSize)
        isMatched = true;
    }
    
    if(!isMatched)
      output_list.push_back(*itJet);
  }

  return output_list;
}

