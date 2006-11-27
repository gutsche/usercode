#ifndef GutSoftSameNumberOfTracksFilter_h
#define GutSoftSameNumberOfTracksFilter_h

//
// Package:         GutSoftFilters/GutSoftSameNumberOfTracksFilter
// Class:           GutSoftSameNumberOfTracksFilter
// 
// Description:     EDFilter for same number of tracks
//                  for all specified collections
//                  found in the event
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Wed Oct 25 22:09:44 UTC 2006
//
// $Author: gutsche $
// $Date: 2006/10/30 10:28:47 $
// $Revision: 1.1 $
//

#include <string>
#include <vector>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class GutSoftSameNumberOfTracksFilter : public edm::EDFilter {
 public:
  explicit GutSoftSameNumberOfTracksFilter(const edm::ParameterSet&);
  ~GutSoftSameNumberOfTracksFilter();

 private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual bool filter(edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
      
  unsigned int numberOfTracks_;
  std::vector<std::string > trackProducerLabels_;

};

#endif
