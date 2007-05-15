#include "CMS1/CommonTools/interface/MCTruth.h"

#include "CMS1/Muons/interface/Muons.h"
#include "CMS1/Electrons/interface/Electrons.h"

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

// jet association still missing

/*
reco::RecoToSimCollection cms1::MCTruth::associationByHits(const reco::Track& track) {

  //Handle<reco::TrackCollection> trackCollectionH;
  //event.getByLabel("ctfWithMaterialTracks",trackCollectionH);
  const  reco::TrackCollection tC;
  tC.push_back(track);
  
  Handle<SimTrackContainer> simTrackCollection;
  event.getByLabel("g4SimHits", simTrackCollection);
//  event.getByLabel("SimG4Object", simTrackCollection);
  const SimTrackContainer simTC = *(simTrackCollection.product());
  
  Handle<SimVertexContainer> simVertexCollection;
  event.getByLabel("g4SimHits", simVertexCollection);
//  event.getByLabel("SimG4Object", simVertexCollection);
  const SimVertexContainer simVC = *(simVertexCollection.product());

  edm::Handle<TrackingParticleCollection>  TPCollectionH ;
  event.getByLabel("trackingParticles",TPCollectionH);
  const TrackingParticleCollection tPC   = *(TPCollectionH.product());

  reco::RecoToSimCollection p = associatorByHits->associateRecoToSim(trackCollectionH, TPCollectionH, &event);

  for(TrackCollection::size_type i=0; i<tC.size(); ++i) {
    TrackRef track(trackCollectionH, i);
    try{ 
      std::vector<std::pair<TrackingParticleRef, double> > tp = p[track];
      cout << "Reco Track " << setw(2) << track.index() << " pT: "  << setw(6) << track->pt() 
           <<  " matched to " << tp.size() << " MC Tracks" << std::endl;
      for (std::vector<std::pair<TrackingParticleRef, double> >::const_iterator it = tp.begin(); 
           it != tp.end(); ++it) {
        TrackingParticleRef tpr = it->first;
        double assocChi2 = it->second;
        cout << "\t\tMCTrack " << setw(2) << tpr.index() << " pT: " << setw(6) << tpr->pt() << 
          " NShared: " << assocChi2 << endl;
      }
    } catch (Exception event) {
      cout << "->   Track " << setw(2) << track.index() << " pT: " 
           << setprecision(2) << setw(6) << track->pt() 
           <<  " matched to 0  MC Tracks" << endl;
    }
  }
  
  return p;
}  

cout << "-- Associator by chi2 --" << endl;  
  p = associatorByChi2->associateRecoToSim (trackCollectionH,TPCollectionH,&event );
  for(TrackCollection::size_type i=0; i<tC.size(); ++i) {
    TrackRef track(trackCollectionH, i);
    try{ 
      std::vector<std::pair<TrackingParticleRef, double> > tp = p[track];
      cout << "Reco Track " << setw(2) << track.index() << " pT: "  << setw(6) << track->pt() 
	   <<  " matched to " << tp.size() << " MC Tracks" << std::endl;
      for (std::vector<std::pair<TrackingParticleRef, double> >::const_iterator it = tp.begin(); 
	   it != tp.end(); ++it) {
	TrackingParticleRef tpr = it->first;
	double assocChi2 = it->second;
	cout << "\t\tMCTrack " << setw(2) << tpr.index() << " pT: " << setw(6) << tpr->pt() << 
	  " chi2: " << assocChi2 << endl;
      }
    } catch (Exception event) {
      cout << "->   Track " << setw(2) << track.index() << " pT: " 
	   << setprecision(2) << setw(6) << track->pt() 
	   <<  " matched to 0  MC Tracks" << endl;
    }
  }
  //SIMTORECO
  cout << "                      ****************** Sim To Reco ****************** " << endl;
  cout << "-- Associator by hits --" << endl;  
  reco::SimToRecoCollection q = 
    associatorByHits->associateSimToReco(trackCollectionH,TPCollectionH,&event );
  for(SimTrackContainer::size_type i=0; i<simTC.size(); ++i){
    TrackingParticleRef tp (TPCollectionH,i);
    try{ 
      std::vector<std::pair<TrackRef, double> > trackV = q[tp];
	cout << "Sim Track " << setw(2) << tp.index() << " pT: "  << setw(6) << tp->pt() 
	     <<  " matched to " << trackV.size() << " reco::Tracks" << std::endl;
	for (std::vector<std::pair<TrackRef,double> >::const_iterator it=trackV.begin(); it != trackV.end(); ++it) {
	  TrackRef tr = it->first;
	  double assocChi2 = it->second;
	  cout << "\t\treco::Track " << setw(2) << tr.index() << " pT: " << setw(6) << tr->pt() << 
	    " NShared: " << assocChi2 << endl;
	}
    } catch (Exception event) {
      cout << "->   TrackingParticle " << setw(2) << tp.index() << " pT: " 
	   <<setprecision(2)<<setw(6)<<tp->pt() 
	   <<  " matched to 0  reco::Tracks" << endl;
    }
  }
  cout << "-- Associator by chi2 --" << endl;  
  q = associatorByChi2->associateSimToReco(trackCollectionH,TPCollectionH,&event );
  for(SimTrackContainer::size_type i=0; i<simTC.size(); ++i){
    TrackingParticleRef tp (TPCollectionH,i);
    try{ 
      std::vector<std::pair<TrackRef, double> > trackV = q[tp];
      cout << "Sim Track " << setw(2) << tp.index() << " pT: "  << setw(6) << tp->pt() 
	   <<  " matched to " << trackV.size() << " reco::Tracks" << std::endl;
      for (std::vector<std::pair<TrackRef,double> >::const_iterator it=trackV.begin(); it != trackV.end(); ++it) {
	TrackRef tr = it->first;
	double assocChi2 = it->second;
	cout << "\t\treco::Track " << setw(2) << tr.index() << " pT: " << setw(6) << tr->pt() << 
	  " chi2: " << assocChi2 << endl;
      }
    } catch (Exception event) {
      cout << "->   TrackingParticle " << setw(2) << tp.index() << " pT: " 
	   <<setprecision(2)<<setw(6)<<tp->pt() 
	   <<  " matched to 0  reco::Tracks" << endl;
    }
  }
*/
