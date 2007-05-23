#ifndef MCTruth_h
#define MCTruth_h

//
// Package:         CMS1/CommonTools
// Class:           MCTruth
// 
// Description:     MCTruth informations
//
// Original Author: Matteo Sani
//

#include "DataFormats/Candidate/interface/Candidate.h"
#include "CLHEP/HepMC/GenParticle.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "CMS1/Base/interface/EventData.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "CMS1/MCInfo/interface/MCInfo.h"
#include "SimTracker/TrackAssociation/interface/TrackAssociatorByHits.h"
#include "SimTracker/TrackAssociation/interface/TrackAssociatorByChi2.h"
#include "SimDataFormats/Track/interface/SimTrack.h"

#include "DataFormats/Math/interface/LorentzVector.h"
#include <Math/VectorUtil.h>
#include <Math/Point3D.h>

namespace cms1 {
   class MCTruth {
   public:
     MCTruth();
     virtual ~MCTruth() {}
     
     const HepMC::GenParticle* matchP4ToGen(const math::XYZTLorentzVector& v, MCInfo::ParticleType type=MCInfo::All, Cuts cuts=Cuts()) const;

     const HepMC::GenParticle* matchCandToGen(const reco::Candidate&, MCInfo::ParticleType type=MCInfo::All, Cuts cuts=Cuts()) const;
     const reco::GenJet* matchCandToGenJet(const reco::Candidate& jet, Cuts cuts=Cuts()) const; 

     const reco::Candidate* matchGenToCand(const HepMC::GenParticle&, std::vector<const reco::Candidate*> cand) const; 
     const reco::Candidate* matchGenToCand(const reco::GenJet&, std::vector<const reco::Candidate*> cand) const;

     reco::RecoToSimCollection recoToSimByHits();
     reco::SimToRecoCollection simToRecoByHits();
     //reco::RecoToSimCollection recoToSimByChi2();
     //reco::SimToRecoCollection simToRecoByChi2();

     void setEventData(EventData* data){ data_ = data; }
     const EventData* getEventData(){ return data_; }
	
   private:
     EventData* data_;
     edm::ParameterSet conf_;
   };
}

#endif

