#ifndef RoadSearchTrackValidation_h
#define RoadSearchTrackValidation_h

//
// Package:         RoadSearchValidation/RoadSearchTrackValidation
// Class:           RoadSearchTrackValidation
// 
// Description:     fills track validation histrograms and stores them in ROOT file
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Fri Sep  1 15:39:57 UTC 2006
//
// $Author: gutsche $
// $Date: 2006/09/05 21:58:43 $
// $Revision: 1.2 $
//

#include <string>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "GutSoftTools/HistogramFactory/interface/HistogramFactory.h"

class RoadSearchTrackValidation : public edm::EDAnalyzer {
 public:
  explicit RoadSearchTrackValidation(const edm::ParameterSet&);
  ~RoadSearchTrackValidation();

 private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  HistogramFactory histograms_;

  std::string rsSeedProducerLabel_;
  std::string rsRawCloudProducerLabel_;
  std::string rsCleanCloudProducerLabel_;
  std::string rsTrackCandidateProducerLabel_;
  std::string rsTrackProducerLabel_;

  std::string ckfSeedProducerLabel_;
  std::string ckfTrackCandidateProducerLabel_;
  std::string ckfTrackProducerLabel_;

  unsigned int sameNumberOfTracks;

};

#endif
