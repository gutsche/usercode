//
// Package:         CMS1/Base
// Class:           BaseAnalyzer
// 
// Description:     abstract base class for TnS analysis
//
// Original Author: Dmytro Kovalskyi
//
// $Author: dmytro $
// $Date: 2007/04/12 19:37:15 $
// $Revision: 1.2 $
//
#include "CMS1/BaseAnalyzer/interface/BaseAnalyzer.h"

void cms1::BaseAnalyzer::configure(const edm::ParameterSet& iConfig)
{
   // let black boxes know where to read data from
   theMuons.setEventData(     &theData );
   theElectrons.setEventData( &theData );
   theJets.setEventData(      &theData );
   theMET.setEventData(       &theData );
   
   theData.container_reco_Track.registerCollection(            edm::InputTag("ctfWithMaterialTracks",""),"recoTracks_ctfWithMaterialTracks__RECO");
   theData.container_reco_Muon.registerCollection(             edm::InputTag("globalMuons",""),"recoMuons_globalMuons__RECO");
   theData.container_reco_SiStripElectron.registerCollection(  edm::InputTag("siStripElectrons",""),"recoSiStripElectrons_siStripElectrons__SIM");
   theData.container_reco_CaloJet.registerCollection(          edm::InputTag("midPointCone5CaloJets",""),"recoCaloJets_midPointCone5CaloJets__SIM");
   theData.container_reco_CaloMET.registerCollection(          edm::InputTag("met",""),"recoCaloMETs_met__SIM");
}
