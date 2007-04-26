//
// Package:         CMS1/MCInfo
// Class:           testMCInfo
//

#include "testMCInfo.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
//#include "FWCore/MessageLogger/interface/MessageLogger.h"


#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/EgammaCandidates/interface/Electron.h"

#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"

cms1::testMCInfo::testMCInfo(const edm::ParameterSet& iConfig) {

  hepMC_ = iConfig.getUntrackedParameter<edm::InputTag>("MCInputTag");
  genJetInputTag_ = iConfig.getUntrackedParameter<edm::InputTag>("GenJetInputTag");

  // some cuts
  elCut_.pt_min = iConfig.getUntrackedParameter<double>("ElPtMinCut");
  muCut_.pt_min = iConfig.getUntrackedParameter<double>("MuPtMinCut");
  tauCut_.pt_max = iConfig.getUntrackedParameter<double>("TauPtMinCut");
  jetCut_.et_min = iConfig.getUntrackedParameter<double>("JetEtMinCut");
}

cms1::testMCInfo::~testMCInfo()
{}

void
cms1::testMCInfo::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

  // get MC Event
  edm::Handle<edm::HepMCProduct> mcCollectionHandle;
  iEvent.getByLabel(hepMC_, mcCollectionHandle);
  const HepMC::GenEvent* genEvent = mcCollectionHandle->GetEvent();
  std::vector<HepMC::GenParticle> mcParticle;
  for (HepMC::GenEvent::particle_const_iterator p = genEvent->particles_begin(); p != genEvent->particles_end(); ++p)
    mcParticle.push_back(**p);
 
  // get gen jet 
  const reco::GenJetCollection *jetCollection = 0;
  edm::Handle<reco::GenJetCollection> jetCollectionHandle;
  iEvent.getByLabel(genJetInputTag_,jetCollectionHandle);
  jetCollection = jetCollectionHandle.product();
   
  // pass the collection pointer of the collection that we 
  // want to use.
  mcInfo_.getData().mcInfo = &mcParticle;
  mcInfo_.getData().jetInfo = jetCollection;
  
  // dump the MC information of the event without cut
  mcInfo_.dumpLeptons();
  mcInfo_.dumpJets();

  // dump the MC information of the event with cut
  mcInfo_.dumpLeptons(elCut_, muCut_, tauCut_);
  mcInfo_.dumpJets(jetCut_);
}

void cms1::testMCInfo::beginJob(const edm::EventSetup& setup) 
{}

void cms1::testMCInfo::endJob() 
{}
