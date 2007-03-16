//
// Package:         CMS1/TableMaker
// Class:           TableMaker

// 
// Description:     EDAnalyzer filling table for di-lepton analysis dependent on number of jets
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Tue Feb 20 23:00:01 UTC 2007
//
// $Author: dmytro $
// $Date: 2007/03/10 02:25:40 $
// $Revision: 1.5 $
//

#include <vector>
#include <utility>
#include <sstream>
#include <iomanip>

#include "CMS1/TableMaker/interface/TableMakerFW.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"


cms1::TableMakerFW::~TableMakerFW()
{}

cms1::TableMakerFW::TableMakerFW(const edm::ParameterSet& iConfig)
{

  // input tags
  globalTrackInputTag_    = iConfig.getUntrackedParameter<edm::InputTag>("GlobalTrackInputTag");
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

  fileTag 					= iConfig.getUntrackedParameter<std::string>("fileTag");

}


void
cms1::TableMakerFW::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // get MC Event
  edm::Handle<edm::HepMCProduct> mcCollectionHandle;
  iEvent.getByLabel("VtxSmeared", mcCollectionHandle);
  const HepMC::GenEvent* genEvent = mcCollectionHandle->GetEvent();
  std::vector<HepMC::GenParticle> mcParticle;
  for (HepMC::GenEvent::particle_const_iterator p = genEvent->particles_begin(); p != genEvent->particles_end(); ++p)
    mcParticle.push_back(**p);
 
  // get track collection from the event
  edm::Handle<reco::TrackCollection> trackCollectionHandle;
  iEvent.getByLabel(globalTrackInputTag_, trackCollectionHandle);
  data_.tracks = trackCollectionHandle.product();
  
  // get muon collection from the event
  const reco::MuonCollection *muonCollection = 0;
  edm::Handle<reco::MuonCollection> muonCollectionHandle;
  iEvent.getByLabel(globalMuonInputTag_,muonCollectionHandle);
  muonCollection = muonCollectionHandle.product();

  // get electron collection from the event
  const reco::SiStripElectronCollection *electronCollection = 0;
  edm::Handle<reco::SiStripElectronCollection> electronCollectionHandle;
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
  data_.globalMuonCollection = muonCollection;

  // pass the collection pointer of the collection that we 
  // want to use to the ElectronData object.
  // This needs to be done for EVERY EVENT
  data_.siStripElectrons = electronCollection;
  data_.mcInfo = &mcParticle;

  // pass the collection pointer of the collection that we 
  // want to use to the JetData object.
  // This needs to be done for EVERY EVENT
  data_.defaultJets = jetCollection;

  // pass the collection pointer of the collection that we 
  // want to use to the METData object.
  // This needs to be done for EVERY EVENT
  data_.metCollection = METCollection;

  TableMaker::analyze();
}


void 
cms1::TableMakerFW::beginJob(const edm::EventSetup& setup)
{
   TableMaker::beginJob();
}

void 
cms1::TableMakerFW::endJob() {
     
   TableMaker::endJob();
}
