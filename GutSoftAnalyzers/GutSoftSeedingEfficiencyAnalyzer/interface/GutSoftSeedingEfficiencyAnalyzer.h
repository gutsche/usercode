#ifndef GutSoftSeedingEfficiencyAnalyzer_h
#define GutSoftSeedingEfficiencyAnalyzer_h

//
// Package:         GutSoftAnalyzers/GutSoftSeedingEfficiencyAnalyzer
// Class:           GutSoftSeedingEfficiencyAnalyzer
// 
// Description:     EDAnalyzer for seeding efficiency studies
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Thu Feb  8 19:03:24 UTC 2007
//
// $Author: gutsche $
// $Date: 2007/05/25 00:03:00 $
// $Revision: 1.3 $
//

#include <string>
#include <vector>
#include <map>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include "GutSoftTools/GutSoftHistogramFactory/interface/GutSoftHistogramFactory.h"

#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"

#include "RecoTracker/RoadMapRecord/interface/Roads.h"
#include "RecoTracker/RingRecord/interface/Rings.h"

#include "RecoTracker/RingRecord/interface/Ring.h"

#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"
#include "SimDataFormats/TrackingHit/interface/PSimHit.h"

#include "DataFormats/DetId/interface/DetId.h"

#include "TrackingTools/RoadSearchHitAccess/interface/DetHitAccess.h"

#include "SimTracker/TrackerHitAssociation/interface/TrackerHitAssociator.h"

#include "Validation/RecoTrack/interface/TPEfficiencySelector.h"

namespace HepMC {
  class GenParticle;
}

class GutSoftSeedingEfficiencyAnalyzer : public edm::EDAnalyzer {
  
public:
  
  struct ltsimhit
  {
    bool operator()(const PSimHit &h1, const PSimHit &h2) const
    {
      bool result = false;
      if ( h1.detUnitId() < h2.detUnitId() ) {
	result = true;
      }

      return result;
    }
  };


  typedef std::map<PSimHit,std::vector<TrackingRecHit*>,ltsimhit> SimHitMap;
  typedef std::vector<std::vector<int> > MissingBarCodes;

  explicit GutSoftSeedingEfficiencyAnalyzer(const edm::ParameterSet& conf);
  ~GutSoftSeedingEfficiencyAnalyzer();

  bool isIncluded(std::vector<const Ring*> &rings, 
		  DetId id);
  unsigned int hitsInRings(std::vector<const Ring*> &rings, 
			   const TrackingParticle *particle);

  std::vector<int>
  GenParticleBarCodeFromRecHit(TrackerHitAssociator &associator,
			       const edm::SimTrackContainer *simTracks,
			       const TrackingRecHit* hit);

 private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  GutSoftHistogramFactory *histograms_;

  edm::InputTag roadSearchSeedInputTag_;
  edm::InputTag trackingTruthInputTag_;
  edm::InputTag simTracksInputTag_;
  std::string   baseDirectoryName_;

  unsigned int trackingParticleHitsInInnerSeedRings_;
  unsigned int trackingParticleHitsInOuterSeedRings_;
  unsigned int trackingParticleHitsInSeedRings_;

  std::map<double,HepMC::GenParticle> trueParticles_;

  std::vector<const Ring*> innerSeedRings_;
  std::vector<const Ring*> outerSeedRings_;

  const TrackerGeometry *tracker_;
  const Roads           *roads_;
  const Rings           *rings_;

  const edm::ParameterSet conf_; 

  TPEfficiencySelector     trackingParticleSelector_;

  std::string              roadsLabel_;
  std::string              ringsLabel_;

  MissingBarCodes     missingBarCodes_;
  std::vector<int>    runs_;
  std::vector<int>    events_;
  std::vector<double> percentages_;

  MissingBarCodes     missingRSBarCodes_;
  std::vector<int>    runsRS_;
  std::vector<int>    eventsRS_;
  std::vector<double> percentagesRS_;

};

#endif
