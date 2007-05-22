#include "CMS1/CommonTools/interface/MCTruth.h"

#include "CMS1/Muons/interface/Muons.h"
#include "CMS1/Electrons/interface/Electrons.h"
#include "CMS1/Jets/interface/Jets.h"

#include "DataFormats/TrackReco/interface/TrackFwd.h"

const reco::Candidate* cms1::MCTruth::matchGenToCand(const reco::GenJet& genJet, Cuts cuts) const {

  const reco::Candidate* output = 0;
  double dRmin = 0.1;

  std::vector<const reco::Candidate*> cand;
  std::vector<const reco::Candidate*>::const_iterator itCand;
  
  cms1::Jets j;
  cand = j.getJets(Jets::JetsWithoutElectrons, cuts);

  for(itCand=cand.begin(); itCand!=cand.end(); ++itCand) {

    const math::XYZVector v1(genJet.momentum().x(), genJet.momentum().y(), genJet.momentum().z());

    double dR = ROOT::Math::VectorUtil::DeltaR(v1, (*itCand)->p4());
    if (dR < dRmin) {
      dRmin = dR;
      output = *itCand;
    }
  }

  return output;
}

const reco::GenJet* cms1::MCTruth::matchCandToGenJet(const reco::Candidate& jet, Cuts cuts) const { 
  
  const reco::GenJet* output = 0;
  double dRmin = 0.1;
  
  MCInfo mcInfo;
  mcInfo.setEventData(data_);
  std::vector<const reco::GenJet*> genJets = mcInfo.getJetInfo(cuts);
  std::vector<const reco::GenJet*>::const_iterator itJet;

  for(itJet=genJets.begin(); itJet!=genJets.end(); ++itJet) {

    const math::XYZVector v1((*itJet)->momentum().x(), (*itJet)->momentum().y(), (*itJet)->momentum().z());

    double dR = ROOT::Math::VectorUtil::DeltaR(v1, jet.p4());
    if (dR < dRmin) {
      dRmin = dR;
      output = *itJet;
    }
  }

  return output;
}

const HepMC::GenParticle* cms1::MCTruth::matchCandToGen(const reco::Candidate& cand, MCInfo::ParticleType type, Cuts cuts) const {

  const HepMC::GenParticle* output = 0;
  double dRmin = 0.1;
  
  MCInfo mcInfo;
  mcInfo.setEventData(data_);
  std::vector<const HepMC::GenParticle*> genParticles = mcInfo.getMCInfo(type, cuts);
  std::vector<const HepMC::GenParticle*>::const_iterator itPart;
  for(itPart=genParticles.begin(); itPart!=genParticles.end(); ++itPart) {

    const math::XYZVector v1((*itPart)->momentum().x(), (*itPart)->momentum().y(), (*itPart)->momentum().z());

    double dR = ROOT::Math::VectorUtil::DeltaR(v1,cand.p4());
    if (dR < dRmin) {
      dRmin = dR;
      output = *itPart;
    }
  }

  return output;
}

const HepMC::GenParticle* cms1::MCTruth::matchP4ToGen(const math::XYZTLorentzVector& v, MCInfo::ParticleType type, Cuts cuts) const {

  const HepMC::GenParticle* output = 0;
  double dRmin = 0.1;
  
  MCInfo mcInfo;
  mcInfo.setEventData(data_);
  std::vector<const HepMC::GenParticle*> genParticles = mcInfo.getMCInfo(type, cuts);

  std::vector<const HepMC::GenParticle*>::const_iterator itPart;

  for(itPart=genParticles.begin(); itPart!=genParticles.end(); ++itPart) {

    const math::XYZVector v1((*itPart)->momentum().x(), (*itPart)->momentum().y(), (*itPart)->momentum().z());

    double dR = ROOT::Math::VectorUtil::DeltaR(v1, v);
    if (dR < dRmin) {
      dRmin = dR;
      output = *itPart;
    }
  }

  return output;
}

const reco::Candidate* cms1::MCTruth::matchGenToCand(const HepMC::GenParticle& p, MCTruth::CandidateType type, Cuts cuts) const {

  const reco::Candidate* output = 0;
  double dRmin = 0.1;

  std::vector<const reco::Candidate*> cand;
  std::vector<const reco::Candidate*>::const_iterator itCand;
  
  //Fill cand with selected Candidate Collection

  switch (type) {
  case Muons:
    {
      cms1::Muons m;
      cand = m.getMuons(Muons::AllGlobalMuons, cuts);
    }
    break;
  case Electrons:
    {
      cms1::Electrons e;
      cand = e.getElectrons(Electrons::AllElectrons, cuts);
    }
    break;
  default: 
    {
      std::cout << "Something wrong: no valid collection selected" << std::endl;
    }
    break;
  } 

  for(itCand=cand.begin(); itCand!=cand.end(); ++itCand) {

    const math::XYZVector v1(p.momentum().x(), p.momentum().y(), p.momentum().z());

    double dR = ROOT::Math::VectorUtil::DeltaR(v1, (*itCand)->p4());
    if (dR < dRmin) {
      dRmin = dR;
      output = *itCand;
    }
  }

  return output;
}

reco::RecoToSimCollection cms1::MCTruth::recoToSimByHits() {

  // need to be fixed compile but doesn't work of course
  // access to EventSetup ???

  /*
  edm::ESHandle<MagneticField> theMF;
  setup.get<IdealMagneticFieldRecord>().get(theMF);
  
  edm::ESHandle<TrackAssociatorBase> theChiAssociator;
  setup.get<TrackAssociatorRecord>().get("TrackAssociatorByChi2",theChiAssociator);
  associatorByChi2 = (TrackAssociatorBase *) theChiAssociator.product();
  
  edm::ESHandle<TrackAssociatorBase> theHitsAssociator;
  setup.get<TrackAssociatorRecord>().get("TrackAssociatorByHits",theHitsAssociator);
  associatorByHits = (TrackAssociatorBase *) theHitsAssociator.product();
  */
  TrackAssociatorBase* associatorByHits;
  edm::Handle<reco::TrackCollection> trackCollectionH = data_->getHandle<reco::TrackCollection>("ctfWithMaterialTracks");
  edm::Handle<TrackingParticleCollection>  TPCollectionH = data_->getHandle<TrackingParticleCollection>("trackingtruth", "TrackTruth");

  reco::RecoToSimCollection p = associatorByHits->associateRecoToSim(trackCollectionH, TPCollectionH, data_->iEvent);

  return p;
}  

reco::SimToRecoCollection cms1::MCTruth::simToRecoByHits() {
  
  TrackAssociatorBase* associatorByHits;
  edm::Handle<reco::TrackCollection> trackCollectionH = data_->getHandle<reco::TrackCollection>("ctfWithMaterialTracks");
  edm::Handle<TrackingParticleCollection>  TPCollectionH = data_->getHandle<TrackingParticleCollection>("trackingtruth", "TrackTruth");

  reco::SimToRecoCollection p = associatorByHits->associateSimToReco(trackCollectionH, TPCollectionH, data_->iEvent);

  return p;
}

reco::RecoToSimCollection cms1::MCTruth::recoToSimByChi2() {

  TrackAssociatorBase* associatorByChi2;
  edm::Handle<reco::TrackCollection> trackCollectionH = data_->getHandle<reco::TrackCollection>("ctfWithMaterialTracks");
  edm::Handle<TrackingParticleCollection>  TPCollectionH = data_->getHandle<TrackingParticleCollection>("trackingtruth", "TrackTruth");

  reco::RecoToSimCollection p = associatorByChi2->associateRecoToSim(trackCollectionH, TPCollectionH, data_->iEvent);

  return p;
}  

reco::SimToRecoCollection cms1::MCTruth::simToRecoByChi2() {
  
  TrackAssociatorBase* associatorByChi2;
  edm::Handle<reco::TrackCollection> trackCollectionH = data_->getHandle<reco::TrackCollection>("ctfWithMaterialTracks");
  edm::Handle<TrackingParticleCollection>  TPCollectionH = data_->getHandle<TrackingParticleCollection>("trackingtruth", "TrackTruth");

  reco::SimToRecoCollection p = associatorByChi2->associateSimToReco(trackCollectionH, TPCollectionH, data_->iEvent);

  return p;
}
