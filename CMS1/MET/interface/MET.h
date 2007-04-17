#ifndef CMS1MET_h
#define CMS1MET_h

//
// Package:         CMS1/MET
// Class:           MET
// 
// Description:     analysis functions for MET
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Wed Feb 21 00:15:42 UTC 2007
//
// $Author: latb $
// $Date: 2007/03/22 15:31:55 $
// $Revision: 1.5 $
//
#include <iostream>

#include "DataFormats/METReco/interface/CaloMETCollection.h"
#include "DataFormats/MuonReco/interface/Muon.h"

#include "CMS1/Base/interface/Cuts.h"
#include "CMS1/Base/interface/BlackBox.h"

namespace cms1 {
   class MET: public BlackBox  {
  public:
      MET(): BlackBox()	{}
      
      enum METType { DefaultMET };
     
      // This is the function that does all the work
      const reco::CaloMET* getMET (const METType );
      
      // here should be all corrections
      static void correctMETmuons(const std::vector<const reco::Candidate*>* m,
				  double& et, double& phi);
      void dump(std::ostream&, const reco::Candidate*);
  };
}

#endif
