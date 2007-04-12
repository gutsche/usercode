//
// Package:         CMS1/Base
// Class:           BaseAnalyzerFWLite
// 
// Description:     Base FWLite analyzer
//
// Original Author: Dmytro Kovalskyi
//
// $Author: dmytro $
// $Date: 2007/04/07 17:54:23 $
// $Revision: 1.1 $
//
#include "CMS1/BaseAnalyzer/interface/BaseAnalyzerFWLite.h"
void cms1::BaseAnalyzerFWLite::analyze(bool initialization)
{
   // fill EvenData 
   // each call correspods to one C++ type of collection
   loadData( theDataFW->container_HepMC_GenParticle, initialization);
   loadData( theDataFW->container_reco_Track, initialization );
   loadData( theDataFW->container_reco_SiStripElectron, initialization );
   loadData( theDataFW->container_reco_CaloJet, initialization );
   loadData( theDataFW->container_reco_Muon, initialization );
   loadData( theDataFW->container_reco_CaloMET, initialization );
}
