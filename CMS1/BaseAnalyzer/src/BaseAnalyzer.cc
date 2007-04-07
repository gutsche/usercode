//
// Package:         CMS1/Base
// Class:           BaseAnalyzer
// 
// Description:     abstract base class for TnS analysis
//
// Original Author: Dmytro Kovalskyi
//
// $Author: dmytro $
// $Date: 2007/03/16 07:00:21 $
// $Revision: 1.1 $
//
#include "CMS1/BaseAnalyzer/interface/BaseAnalyzer.h"

void cms1::BaseAnalyzer::configure(const edm::ParameterSet& iConfig)
{
   // let black boxes know where to read data from
   theMuons.setEventData(     &theData );
   theElectrons.setEventData( &theData );
   theJets.setEventData(      &theData );
   theMET.setEventData(       &theData );
   
   // register standart collections
   theData.container_reco_Track.registerCollection(edm::InputTag("ctfWithMaterialTracks",""),"recoTracks_ctfWithMaterialTracks__RECO");
   theData.container_reco_Muon.registerCollection( edm::InputTag("globalMuons",""),"recoMuons_globalMuons__RECO");
   
}
