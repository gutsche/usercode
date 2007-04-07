//
// Package:         CMS1/Base
// Class:           BaseAnalyzerFW
// 
// Description:     Base Framework analyzer
//
// Original Author: Dmytro Kovalskyi
//
// $Author: dmytro $
// $Date: 2007/03/16 07:00:21 $
// $Revision: 1.1 $
//
#include "CMS1/BaseAnalyzer/interface/BaseAnalyzerFW.h"
void cms1::BaseAnalyzerFW::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   // fill EvenData 
   // each call correspods to one C++ type of collection
   // loadData( iEvent, theDataFW->container_HepMC_GenParticle );
   loadData( iEvent, theDataFW->container_reco_Track );
   loadData( iEvent, theDataFW->container_reco_SiStripElectron );
   loadData( iEvent, theDataFW->container_reco_CaloJet );
   loadData( iEvent, theDataFW->container_reco_Muon );
   loadData( iEvent, theDataFW->container_reco_CaloMET );
}
