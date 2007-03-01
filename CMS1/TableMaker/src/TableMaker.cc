//
// Package:         CMS1/TableMaker
// Class:           TableMaker

// 
// Description:     EDAnalyzer filling table for di-lepton analysis dependent on number of jets
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Tue Feb 20 23:00:01 UTC 2007
//
// $Author: gutsche $
// $Date: 2007/03/01 16:15:02 $
// $Revision: 1.2 $
//

#include <vector>
#include <utility>
#include <sstream>
#include <iomanip>

#include "CMS1/TableMaker/interface/TableMaker.h"
#include "CMS1/Base/interface/Dump.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/EgammaCandidates/interface/Electron.h"
#include "DataFormats/EgammaCandidates/interface/ElectronFwd.h"

#include "DataFormats/JetReco/interface/CaloJet.h"

#include "DataFormats/METReco/interface/CaloMETCollection.h"

#include "DataFormats/Math/interface/LorentzVector.h"

cms1::TableMaker::TableMaker(const edm::ParameterSet& iConfig)
{

  // input tags
  globalMuonInputTag_     = iConfig.getUntrackedParameter<edm::InputTag>("GlobalMuonInputTag");
  globalElectronInputTag_ = iConfig.getUntrackedParameter<edm::InputTag>("GlobalElectronInputTag");
  globalJetInputTag_      = iConfig.getUntrackedParameter<edm::InputTag>("GlobalJetInputTag");
  globalMETInputTag_      = iConfig.getUntrackedParameter<edm::InputTag>("GlobalMETInputTag");

  // tight muon cuts
  tightMuon_.pt_min       = iConfig.getUntrackedParameter<double>("TightMuonPt");
  tightMuon_.eta_min      = iConfig.getUntrackedParameter<double>("TightMuonMinEta");
  tightMuon_.eta_max      = iConfig.getUntrackedParameter<double>("TightMuonMaxEta");

  // loose muon cuts
  looseMuon_.pt_min       = iConfig.getUntrackedParameter<double>("LooseMuonPt");
  looseMuon_.eta_min      = iConfig.getUntrackedParameter<double>("LooseMuonMinEta");
  looseMuon_.eta_max      = iConfig.getUntrackedParameter<double>("LooseMuonMaxEta");

  // all muon cuts
  allMuon_.pt_min       = iConfig.getUntrackedParameter<double>("AllMuonPt");
  allMuon_.eta_min      = iConfig.getUntrackedParameter<double>("AllMuonMinEta");
  allMuon_.eta_max      = iConfig.getUntrackedParameter<double>("AllMuonMaxEta");

  // tight electron cuts
  tightElectron_.pt_min   = iConfig.getUntrackedParameter<double>("TightElectronPt");
  tightElectron_.eta_min  = iConfig.getUntrackedParameter<double>("TightElectronMinEta");
  tightElectron_.eta_max  = iConfig.getUntrackedParameter<double>("TightElectronMaxEta");

  // loose electron cuts
  looseElectron_.pt_min   = iConfig.getUntrackedParameter<double>("LooseElectronPt");
  looseElectron_.eta_min  = iConfig.getUntrackedParameter<double>("LooseElectronMinEta");
  looseElectron_.eta_max  = iConfig.getUntrackedParameter<double>("LooseElectronMaxEta");

  // jet cuts
  jet_.pt_min             = iConfig.getUntrackedParameter<double>("JetPt");
  jet_.eta_min            = iConfig.getUntrackedParameter<double>("JetMinEta");
  jet_.eta_max            = iConfig.getUntrackedParameter<double>("JetMaxEta");

  // MET cuts
  metCut_.pt_min             = iConfig.getUntrackedParameter<double>("MET");

  // MET cuts around Z
  metCutAroundZ_.pt_min      = iConfig.getUntrackedParameter<double>("METAroundZ");
  ZRangeMinMass_             = iConfig.getUntrackedParameter<double>("ZRangeMinMass");
  ZRangeMaxMass_             = iConfig.getUntrackedParameter<double>("ZRangeMaxMass");

}


cms1::TableMaker::~TableMaker()
{
 
}


void
cms1::TableMaker::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  ++events_;

  // get muon collection from the event
  const reco::MuonCollection *muonCollection = 0;
  edm::Handle<reco::MuonCollection> muonCollectionHandle;
  iEvent.getByLabel(globalMuonInputTag_,muonCollectionHandle);
  muonCollection = muonCollectionHandle.product();

  // get electron collection from the event
  const reco::ElectronCollection *electronCollection = 0;
  edm::Handle<reco::ElectronCollection> electronCollectionHandle;
  iEvent.getByLabel(globalElectronInputTag_,electronCollectionHandle);
  electronCollection = electronCollectionHandle.product();

  // get jet collection from the event
  const reco::CaloJetCollection *jetCollection = 0;
  edm::Handle<reco::CaloJetCollection> jetCollectionHandle;
  iEvent.getByLabel(globalJetInputTag_,jetCollectionHandle);
  jetCollection = jetCollectionHandle.product();

  // get MET collection from the event
  const reco::CaloMETCollection *METCollection = 0;
  edm::Handle<reco::CaloMETCollection> METCollectionHandle;
  iEvent.getByLabel(globalMETInputTag_,METCollectionHandle);
  METCollection = METCollectionHandle.product();
   
  // pass the collection pointer of the collection that we 
  // want to use to the MuonData object.
  // This needs to be done for EVERY EVENT
  muons_.getData().globalMuonCollection = muonCollection;

  // pass the collection pointer of the collection that we 
  // want to use to the ElectronData object.
  // This needs to be done for EVERY EVENT
  electrons_.getData().globalElectronCollection = electronCollection;

  // pass the collection pointer of the collection that we 
  // want to use to the JetData object.
  // This needs to be done for EVERY EVENT
  jets_.getData().globalJetCollection = jetCollection;

  // pass the collection pointer of the collection that we 
  // want to use to the METData object.
  // This needs to be done for EVERY EVENT
  MET_.getData().globalMETCollection = METCollection;
   
  // get vector of muons
  std::vector<const reco::Muon*> tightMuons = muons_.getMuons(Muons::TightGlobalMuons,tightMuon_);
  std::vector<const reco::Muon*> looseMuons = muons_.getMuons(Muons::LooseGlobalMuons,looseMuon_);
  std::vector<const reco::Muon*> allMuons = muons_.getMuons(Muons::AllGlobalMuons,allMuon_);

  // get vector of electrons
  std::vector<const reco::Electron*> tightElectrons = electrons_.getElectrons(Electrons::TightGlobalElectrons,tightElectron_);
  std::vector<const reco::Electron*> looseElectrons = electrons_.getElectrons(Electrons::LooseGlobalElectrons,looseElectron_);

  // get vector of jets
  std::vector<const reco::CaloJet*> jets = jets_.getJets(Jets::GlobalJets,jet_);

  // get vector of MET
  std::vector<const reco::CaloMET*> metVector = MET_.getMET(MET::GlobalMET,metCut_);

  //    edm::LogVerbatim("CMS1TableMaker") << tightMuons.size() << " tight global muon(s) found!";
  //    edm::LogVerbatim("CMS1TableMaker") << looseMuons.size() << " loose global muon(s) found!";
  //    edm::LogVerbatim("CMS1TableMaker") << tightElectrons.size() << " tight global electron(s) found!";
  //    edm::LogVerbatim("CMS1TableMaker") << looseElectrons.size() << " loose global electron(s) found!";
  //    edm::LogVerbatim("CMS1TableMaker") << jets.size() << " global jet(s) found!";
  //    edm::LogVerbatim("CMS1TableMaker") << metVector.size() << " global MET found!";


//	cms1::Dump::event();
//	cms1::Dump::candidates("AllGlobalMuons", allMuons );
//	cms1::Dump::candidates("LooseGlobalElectrons", looseElectrons );
//	cms1::Dump::candidates("GlobalJets", jets );

#ifndef NOTDEF

	std::cout << "------------------------------------------------------------------------" << std::endl; 
	std::cout << "Dump of Event, " 
		<< " Ne = " << looseElectrons.size() 
		<< " Nmu = " << allMuons.size() 
		<< " Nj = " << jets.size()		 
		<< " MET = " << metVector.size()		 
	<< std::endl;	

  for ( std::vector<const reco::Electron*>::iterator i = looseElectrons.begin(), ie = looseElectrons.end();
		i != ie;
		++i ) {
			const reco::Candidate* cp = *i;
			std::cout 
				<< "Electron " 
			<< "Pt = " << cp->pt() 
			<< ", Eta = " << cp->eta() 
			<< ", Phi = " << cp->phi() 
			<< std::endl; 
	}
		
	for ( std::vector<const reco::Muon*>::iterator i = allMuons.begin(), ie = allMuons.end();
		i != ie;
		++i ) {
			const reco::Candidate* cp = *i;
			std::cout 
			<< "Muon "  
			<< "Pt = " << cp->pt() 
			<< ", Eta = " << cp->eta() 
			<< ", Phi = " << cp->phi() 
			<< std::endl; 
	}
	for ( std::vector<const reco::CaloJet*>::iterator i = jets.begin(), ie = jets.end();
		i != ie;
		++i ) {
			const reco::Candidate* cp = *i;
			std::cout 
					<< "Jet " 
					<< "Pt = " << cp->pt() 
					<< ", Eta = " << cp->eta() 
					<< ", Phi = " << cp->phi() 
					<< std::endl; 
	}
	for ( std::vector<const reco::CaloMET*>::iterator i = metVector.begin(), ie = metVector.end();
			i != ie;
			++i ) {
		const reco::Candidate* cp = *i;
		std::cout 
	   		<< "MET "  
				<< "Pt = " << cp->pt() 
				<< ", Eta = " << cp->eta() 
				<< ", Phi = " << cp->phi() 
				<< std::endl; 
	}

#endif

  // logic
  std::vector<std::pair<const reco::Electron*, const reco::Electron*> > takenEE;
  std::vector<std::pair<const reco::Electron*, const reco::Muon*> >     takenEMu;
  std::vector<std::pair<const reco::Muon*, const reco::Electron*> >     takenMuE;
  std::vector<std::pair<const reco::Muon*, const reco::Muon*> >         takenMuMu;


  // loop over tight electrons
  for ( std::vector<const reco::Electron*>::iterator tightElectron = tightElectrons.begin(),
	  electronEnd = tightElectrons.end();
	tightElectron != electronEnd;
	++tightElectron ) {
    // loop over loose electrons
    for ( std::vector<const reco::Electron*>::iterator looseElectron = looseElectrons.begin(),
	    electronEnd = looseElectrons.end();
	  looseElectron != electronEnd;
	  ++looseElectron ) {
      // check if the same electron has been selected
      if ( *tightElectron != *looseElectron ) {
	// check if pair already passed cuts
	bool passed = false;
	for ( std::vector<std::pair<const reco::Electron*, const reco::Electron*> >::iterator passedPair = takenEE.begin(),
		passedPairEnd = takenEE.end();
	      passedPair != passedPairEnd;
	      ++passedPair ) {
	  if ( (passedPair->second == *tightElectron) &&
	       (passedPair->first == *looseElectron) ) {
	    passed = true;
	    break;
	  }
	}
	// continue if pair hasn't passed cuts yet
	if ( !passed ) {
	  // check if candidate passes MET cut
	  if ( metVector.size() > 0 ) {
	    // calculate invariant mass and check for special MET cut within Z window
	    // take first MET cancicate
	    math::XYZTLorentzVector inv = (*tightElectron)->p4() + (*looseElectron)->p4();
	    if ( inv.mass() >= ZRangeMinMass_ && inv.mass() <= ZRangeMaxMass_ ) {
	      if ( metCutAroundZ_.testCandidate(**(metVector.begin())) ) {
		// passed
		takenEE.push_back(std::make_pair(*tightElectron,*looseElectron));
	      }
	    } else {
	      takenEE.push_back(std::make_pair(*tightElectron,*looseElectron));
	    }
	  }
	}
      }
    }
    // loop over loose muons
    for ( std::vector<const reco::Muon*>::iterator looseMuon = looseMuons.begin(),
	    MuonEnd = looseMuons.end();
	  looseMuon != MuonEnd;
	  ++looseMuon ) {
      // check if candidate passes MET cut
      if ( metVector.size() > 0 ) {
	takenEMu.push_back(std::make_pair(*tightElectron,*looseMuon));
      }
    }
  }

  // loop over tight muons
  for ( std::vector<const reco::Muon*>::iterator tightMuon = tightMuons.begin(),
	  muonEnd = tightMuons.end();
	tightMuon != muonEnd;
	++tightMuon ) {
    // loop over loose electrons
    for ( std::vector<const reco::Electron*>::iterator looseElectron = looseElectrons.begin(),
	    electronEnd = looseElectrons.end();
	  looseElectron != electronEnd;
	  ++looseElectron ) {
      // check if candidate passes MET cut
      if ( metVector.size() > 0 ) {
	takenMuE.push_back(std::make_pair(*tightMuon,*looseElectron));
      }
    }
    // loop over loose muons
    for ( std::vector<const reco::Muon*>::iterator looseMuon = looseMuons.begin(),
	    MuonEnd = looseMuons.end();
	  looseMuon != MuonEnd;
	  ++looseMuon ) {

      // check if the same muon has been selected
      if ( *tightMuon != *looseMuon ) {
	// check if pair already passed cuts
	bool passed = false;
	for ( std::vector<std::pair<const reco::Muon*, const reco::Muon*> >::iterator passedPair = takenMuMu.begin(),
		passedPairEnd = takenMuMu.end();
	      passedPair != passedPairEnd;
	      ++passedPair ) {
	  if ( (passedPair->second == *tightMuon) &&
	       (passedPair->first == *looseMuon) ) {
	    passed = true;
	    break;
	  }
	}
	// continue if pair hasn't passed cuts yet
	if ( !passed ) {
	  // check if candidate passes MET cut
	  if ( metVector.size() > 0 ) {
	    // calculate invariant mass and check for special MET cut within Z window
	    // take first MET cancicate
	    math::XYZTLorentzVector inv = (*tightMuon)->p4() + (*looseMuon)->p4();
	    if ( inv.mass() >= ZRangeMinMass_ && inv.mass() <= ZRangeMaxMass_ ) {
	      if ( metCutAroundZ_.testCandidate(**(metVector.begin())) ) {
		// passed
		takenMuMu.push_back(std::make_pair(*tightMuon,*looseMuon));
	      }
	    } else {
	      takenMuMu.push_back(std::make_pair(*tightMuon,*looseMuon));
	    }
	  }
	}
      }
    }
  }

  // fill counter
  if ( jets.size() == 0 ) {
    countedEE0Jets_   += takenEE.size();
    countedEMu0Jets_  += takenEMu.size();
    countedMuE0Jets_  += takenMuE.size();
    countedMuMu0Jets_ += takenMuMu.size();
  } else if ( jets.size() == 1 ) {
    countedEE1Jets_   += takenEE.size();
    countedEMu1Jets_  += takenEMu.size();
    countedMuE1Jets_  += takenMuE.size();
    countedMuMu1Jets_ += takenMuMu.size();
  } else if ( jets.size() == 2 ) {
    countedEE2Jets_   += takenEE.size();
    countedEMu2Jets_  += takenEMu.size();
    countedMuE2Jets_  += takenMuE.size();
    countedMuMu2Jets_ += takenMuMu.size();
  } else if ( jets.size() == 3 ) {
    countedEE3Jets_   += takenEE.size();
    countedEMu3Jets_  += takenEMu.size();
    countedMuE3Jets_  += takenMuE.size();
    countedMuMu3Jets_ += takenMuMu.size();
  } else if ( jets.size() >= 4 ) {
    countedEE4Jets_   += takenEE.size();
    countedEMu4Jets_  += takenEMu.size();
    countedMuE4Jets_  += takenMuE.size();
    countedMuMu4Jets_ += takenMuMu.size();
  }
     
}


void 
cms1::TableMaker::beginJob(const edm::EventSetup& setup)
{

  countedEE0Jets_ = 0;
  countedEMu0Jets_ = 0;
  countedMuE0Jets_ = 0;
  countedMuMu0Jets_ = 0;
  countedEE1Jets_ = 0;
  countedEMu1Jets_ = 0;
  countedMuE1Jets_ = 0;
  countedMuMu1Jets_ = 0;
  countedEE2Jets_ = 0;
  countedEMu2Jets_ = 0;
  countedMuE2Jets_ = 0;
  countedMuMu2Jets_ = 0;
  countedEE3Jets_ = 0;
  countedEMu3Jets_ = 0;
  countedMuE3Jets_ = 0;
  countedMuMu3Jets_ = 0;
  countedEE4Jets_ = 0;
  countedEMu4Jets_ = 0;
  countedMuE4Jets_ = 0;
  countedMuMu4Jets_ = 0;

  events_ = 0;

}

void 
cms1::TableMaker::endJob() {

  std::cout << "Muons" << countedMuMu1Jets_ << ", "  << countedMuMu2Jets_ << ", "  << countedMuMu3Jets_ << ", "  << std::endl;

  ostringstream output;

  output << "\n-------------------------------------------------------------------------\n";
  output <<   "| Events: " << setw(9) << events_ << "                                                     |\n";
  output << "-------------------------------------------------------------------------\n";
  output << "| Leptons   |    0 Jets |    1 Jet  |    2 Jets |    3 Jets | >= 4 Jets |\n";
  output << "-------------------------------------------------------------------------\n";
  output << "| EE        | " << std::setw(9) << countedEE0Jets_ << " | " 
	 << std::setw(9) << countedEE1Jets_ << " | " 
	 << std::setw(9) << countedEE2Jets_ << " | " 
	 << std::setw(9) << countedEE3Jets_ << " | " 
	 << std::setw(9) << countedEE4Jets_ << " |\n"; 
  output << "-------------------------------------------------------------------------\n";
  output << "| EMu       | " << std::setw(9) << countedEMu0Jets_ << " | " 
	 << std::setw(9) << countedEMu1Jets_ << " | " 
	 << std::setw(9) << countedEMu2Jets_ << " | " 
	 << std::setw(9) << countedEMu3Jets_ << " | " 
	 << std::setw(9) << countedEMu4Jets_ << " |\n" ;
  output << "-------------------------------------------------------------------------\n";
  output << "| MuE       | " << std::setw(9) << countedMuE0Jets_ << " | " 
	 << std::setw(9) << countedMuE1Jets_ << " | " 
	 << std::setw(9) << countedMuE2Jets_ << " | " 
	 << std::setw(9) << countedMuE3Jets_ << " | " 
	 << std::setw(9) << countedMuE4Jets_ << " |\n" ;
  output << "-------------------------------------------------------------------------\n";
  output << "| MuMu      | " << std::setw(9) << countedMuMu0Jets_ << " | " 
			     << std::setw(9) << countedMuMu1Jets_ << " | " 
	 << std::setw(9) << countedMuMu2Jets_ << " | " 
	 << std::setw(9) << countedMuMu3Jets_ << " | " 
	 << std::setw(9) << countedMuMu4Jets_ << " |\n" ;
  output << "-------------------------------------------------------------------------\n";

  edm::LogVerbatim("CMS1TableMaker") << output.str(); 

}
