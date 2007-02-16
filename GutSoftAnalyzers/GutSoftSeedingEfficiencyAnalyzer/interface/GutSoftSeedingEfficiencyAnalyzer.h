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
// $Date: 2007/01/22 01:35:09 $
// $Revision: 1.3 $
//

#include <string>
#include <vector>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include "GutSoftTools/GutSoftHistogramFactory/interface/GutSoftHistogramFactory.h"

#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"

#include "RecoTracker/RoadMapRecord/interface/Roads.h"

#include "RecoTracker/RingRecord/interface/Ring.h"

#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"
#include "SimDataFormats/TrackingHit/interface/PSimHit.h"

#include "DataFormats/DetId/interface/DetId.h"

#include "TrackingTools/RoadSearchHitAccess/interface/DetHitAccess.h"

#include "SimTracker/TrackerHitAssociation/interface/TrackerHitAssociator.h"

#include "Validation/RecoTrack/interface/TPEfficiencySelector.h"

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

  explicit GutSoftSeedingEfficiencyAnalyzer(const edm::ParameterSet& conf);
  ~GutSoftSeedingEfficiencyAnalyzer();

  bool isIncluded(std::vector<const Ring*> &rings, 
		  DetId id);
  unsigned int hitsInRings(std::vector<const Ring*> &rings, 
			   const TrackingParticle *particle);
  SimHitMap CreateMapTrueHitsToRecHits(TrackerHitAssociator &associator);
  std::vector<TrackingRecHit*> AllRecHitsOfTrackingParticle(SimHitMap &simHitMap, 
							    const TrackingParticle *trackingParticle); 

 private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  GutSoftHistogramFactory *histograms_;

  edm::InputTag trajectorySeedInputTag_;
  edm::InputTag trackingTruthInputTag_;
  edm::InputTag matchedStripRecHitsInputTag_;
  edm::InputTag rphiStripRecHitsInputTag_;
  edm::InputTag stereoStripRecHitsInputTag_;
  edm::InputTag pixelRecHitsInputTag_;
  std::string   baseDirectoryName_;

  double trackingParticleMinimumPt_;
  unsigned int trackingParticleHitsInInnerSeedRings_;
  unsigned int trackingParticleHitsInOuterSeedRings_;
  unsigned int trackingParticleHitsInSeedRings_;

  std::vector<const Ring*> innerSeedRings_;
  std::vector<const Ring*> outerSeedRings_;

  const TrackerGeometry *tracker_;
  const Roads           *roads_;

  const edm::ParameterSet conf_; 

  DetHitAccess innerSeedHitVector_;
  DetHitAccess outerSeedHitVector_;

  DetHitAccess::accessMode innerSeedHitAccessMode_;
  bool                     innerSeedHitAccessUseRPhi_;
  bool                     innerSeedHitAccessUseStereo_;

  DetHitAccess::accessMode outerSeedHitAccessMode_;
  bool                     outerSeedHitAccessUseRPhi_;
  bool                     outerSeedHitAccessUseStereo_;

  TPEfficiencySelector    trackingParticleSelector_;

};

#endif
