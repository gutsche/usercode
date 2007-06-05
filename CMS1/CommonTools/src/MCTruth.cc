#include "CMS1/CommonTools/interface/MCTruth.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include <vector>
#include <string>
#include <iostream>

cms1::MCTruth::MCTruth() {

  std::vector<std::string> trackerContainers;
  trackerContainers.push_back("TrackerHitsTIBLowTof");
  trackerContainers.push_back("TrackerHitsTIBHighTof");
  trackerContainers.push_back("TrackerHitsTIDLowTof");
  trackerContainers.push_back("TrackerHitsTIDHighTof");
  trackerContainers.push_back("TrackerHitsTOBLowTof");
  trackerContainers.push_back("TrackerHitsTOBHighTof");
  trackerContainers.push_back("TrackerHitsTECLowTof");
  trackerContainers.push_back("TrackerHitsTECHighTof");
  trackerContainers.push_back("TrackerHitsPixelBarrelLowTof");
  trackerContainers.push_back("TrackerHitsPixelBarrelHighTof");
  trackerContainers.push_back("TrackerHitsPixelEndcapLowTof");
  trackerContainers.push_back("TrackerHitsPixelEndcapHighTof");

  conf_.addParameter("MinHitFraction", (double)0.5);
  conf_.addParameter("associatePixel",(bool)true);
  conf_.addParameter("associateStrip", (bool)true);
  conf_.addParameter("associateRecoTracks", (bool)true);
  conf_.addParameter("ROUList", (std::vector<std::string>)trackerContainers);
  
  data_ = 0;
}

const reco::Candidate* cms1::MCTruth::matchGenToCand(const reco::GenJet& genJet, std::vector<const reco::Candidate*> cand) const {

  const reco::Candidate* output = 0;
  double dRmin = 0.25;

  std::vector<const reco::Candidate*>::const_iterator itCand;
  
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

const reco::GenJet* cms1::MCTruth::matchCandToGenJet(const reco::Candidate& jet,  Cuts cuts) const { 
  
  const reco::GenJet* output = 0;
  double dRmin = 0.25;
  
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

const reco::Candidate* cms1::MCTruth::matchGenToCand(const HepMC::GenParticle& p, std::vector<const reco::Candidate*> cand) const {

  const reco::Candidate* output = 0;
  double dRmin = 0.1;

  std::vector<const reco::Candidate*>::const_iterator itCand;
  
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

  TrackAssociatorByHits* associatorByHits = new TrackAssociatorByHits(conf_);
  edm::Handle<reco::TrackCollection> trackCollectionH = data_->getHandle<reco::TrackCollection>("ctfWithMaterialTracks");
  edm::Handle<TrackingParticleCollection>  TPCollectionH = data_->getHandle<TrackingParticleCollection>("trackingtruth", "TrackTruth");

  reco::RecoToSimCollection p = associatorByHits->associateRecoToSim(trackCollectionH, TPCollectionH, data_->iEvent);

  return p;
}  

reco::SimToRecoCollection cms1::MCTruth::simToRecoByHits() {
  
  TrackAssociatorByHits* associatorByHits = new TrackAssociatorByHits(conf_);
  edm::Handle<reco::TrackCollection> trackCollectionH = data_->getHandle<reco::TrackCollection>("ctfWithMaterialTracks");
  edm::Handle<TrackingParticleCollection>  TPCollectionH = data_->getHandle<TrackingParticleCollection>("trackingtruth", "TrackTruth");

  reco::SimToRecoCollection p = associatorByHits->associateSimToReco(trackCollectionH, TPCollectionH, data_->iEvent);

  return p;
}
/*
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
*/
