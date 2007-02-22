#ifndef MuonAnalyzerLite_h
#define MuonAnalyzerLite_h

//
// Package:         CMS1/MuonAnalyzer
// Class:           MuonAnalyzer

// 
// Description:     FWLite analyzer using cms1 muons object
//
// Original Author: Dmytro Kovalskyi
//
// $Author: gutsche $
// $Date: 2007/01/29 18:15:27 $
// $Revision: 1.1 $
//

#include "TFile.h"

namespace cms1 {

   class dummy { }  ;
   void MuonTestAnalyzer(TFile&);
   void MuonTestAnalyzerSimplified(TFile&);

}

#endif
