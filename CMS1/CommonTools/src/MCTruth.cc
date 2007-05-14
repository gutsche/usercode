#include "CMS1/CommonTools/interface/MCTruth.h"

#include "CMS1/Muons/interface/Muons.h"

const HepMC::GenParticle* cms1::MCTruth::matchCandToGen(const reco::Candidate& cand, MCInfo::ParticleType type, Cuts cuts) const {

  const HepMC::GenParticle* output = 0;
  double dRmin = 0.1;
  
  MCInfo mcInfo;
  mcInfo.setEventData(data_);
  std::vector<const HepMC::GenParticle*> genParticles = mcInfo.getMCInfo(type, cuts);

  std::vector<const HepMC::GenParticle*>::const_iterator itPart;

  for(itPart=genParticles.begin(); itPart!=genParticles.end(); ++itPart) {

    const math::XYZVector v1((*itPart)->momentum().x(), (*itPart)->momentum().y(), (*itPart)->momentum().z());
    const math::XYZVector v2(cand.px(), cand.py(), cand.pz());

    double dR = ROOT::Math::VectorUtil::DeltaR(v1, v2);
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
