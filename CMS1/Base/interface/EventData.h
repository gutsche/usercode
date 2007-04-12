#ifndef CMS1EventData_h
#define CMS1EventData_h
//
// Package:         CMS1/Base
// Class:           EventData
// 
// Description:     container of event information (collections, objects etc)
//
// Original Author: Dmytro Kovalskyi
//
// $Author: dmytro $
// $Date: 2007/04/07 18:05:54 $
// $Revision: 1.3 $
//

#include "CLHEP/HepMC/GenParticle.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuIsoDeposit.h"
#include "DataFormats/MuonReco/interface/MuonWithMatchInfo.h"
#include "DataFormats/EgammaCandidates/interface/Electron.h"
#include "DataFormats/EgammaCandidates/interface/SiStripElectron.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"
#include "CMS1/Base/interface/CollectionContainer.h"

namespace cms1 {
   struct EventData 
     {
	CollectionContainer<HepMC::GenParticle>     container_HepMC_GenParticle;
	CollectionContainer<reco::Track>            container_reco_Track;
	CollectionContainer<reco::SiStripElectron>  container_reco_SiStripElectron;
	CollectionContainer<reco::CaloJet>          container_reco_CaloJet;
	CollectionContainer<reco::Muon>             container_reco_Muon;
	CollectionContainer<reco::CaloMET>          container_reco_CaloMET;
	
	EventData():mcInfo(0){};
	
	
	/////////////////////////////////////////////////////////////////
	// DEPRECIATED OLD STUFF - WILL BE DROPPED

	// generator info 
	const std::vector<HepMC::GenParticle>*         mcInfo;
/*   
	// track collection
	const std::vector<reco::Track>*                tracks;
	
	// electron collection
	const std::vector<reco::SiStripElectron>*      siStripElectrons;
	
	// jet collection
	const std::vector<reco::CaloJet>*              defaultJets;
	
	// muon stuff
	const std::vector<reco::Muon>*                 globalMuonCollection;
	const std::vector<reco::MuonWithMatchInfo>*    muonIdCollection;
	const std::vector<reco::MuIsoDeposit>*         muonIsolationCollection;

	// MET
	const std::vector<reco::CaloMET>*              metCollection;
	
	EventData():
	   mcInfo(0)
	 , tracks(0)
	 , siStripElectrons(0)
	 , defaultJets(0)
	 , globalMuonCollection(0)
	 , muonIdCollection(0)
	 , muonIsolationCollection(0)
	 , metCollection(0)
	{}
 */
     };
   
}
#endif
