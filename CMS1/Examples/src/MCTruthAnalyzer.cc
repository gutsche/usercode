//
// Package:         CMS1/Examples
// Class:           MCTruthAnalyzer
// 
// Description:     working example of MCTruth
//
// Original Author: Matteo Sani
//

#include "CMS1/Examples/interface/MCTruthAnalyzer.h"
#include "CMS1/CommonTools/interface/MCTruth.h"

#include "SimTracker/TrackAssociation/interface/TrackAssociatorByHits.h"

#include <iostream>

void cms1::MCTruthAnalyzer::configure(const edm::ParameterSet& iConfig) 
{}

void cms1::MCTruthAnalyzer::processEvent(const edm::Event& iEvent) {

  MCTruth mc;
  mc.setEventData(&theData);

  // at the moment the standard algorithm uses Handle to the collections and it is not easy to pass a track as input 
  // to the function and return the associated tracks
  
  edm::Handle<reco::TrackCollection> trackCollectionH = theData.getHandle<reco::TrackCollection>("ctfWithMaterialTracks");
  
  reco::RecoToSimCollection p = mc.recoToSimByHits();
  
  for(reco::TrackCollection::size_type i=0; i<trackCollectionH->size(); ++i) {
    reco::TrackRef track(trackCollectionH, i);
    try {
      std::vector<std::pair<TrackingParticleRef, double> > tp = p[track];

      std::cout << "Reco Track " << std::setw(2) << track.index() << " pT: "  << std::setw(6) << track->pt() 
                <<  " matched to " << tp.size() << " MC Tracks" << std::endl;
      for (std::vector<std::pair<TrackingParticleRef, double> >::const_iterator it = tp.begin(); it != tp.end(); ++it) {
        TrackingParticleRef tpr = it->first;
        double assocChi2 = it->second;
        std::cout << "\t\tMCTrack " << std::setw(2) << tpr.index() << " pT: " << std::setw(6) << tpr->pt() << 
          " NShared: " << assocChi2 << std::endl;
      }
    } catch (edm::Exception event) {
      std::cout << "->   Track " << std::setw(2) << track.index() << " pT: " 
                << std::setprecision(2) << std::setw(6) << track->pt() 
                <<  " matched to 0  MC Tracks" << std::endl;
    }
  }
}

