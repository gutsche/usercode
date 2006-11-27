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

#include "GutSoftFilters/GutSoftSameNumberOfTracksFilter/interface/GutSoftSameNumberOfTracksFilter.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

GutSoftSameNumberOfTracksFilter::GutSoftSameNumberOfTracksFilter(const edm::ParameterSet& iConfig)
{

  numberOfTracks_      = iConfig.getUntrackedParameter<unsigned int>("NumberOfTracks");
  trackProducerLabels_ = iConfig.getUntrackedParameter<std::vector<std::string> >("TrackProducerLabels");

}


GutSoftSameNumberOfTracksFilter::~GutSoftSameNumberOfTracksFilter()
{
 
}

bool
GutSoftSameNumberOfTracksFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  // return value
  bool result = false;
  
  std::vector<const reco::TrackCollection*> trackCollections;
  for ( std::vector<std::string>::const_iterator trackProducerLabel = trackProducerLabels_.begin();
	trackProducerLabel != trackProducerLabels_.end();
	++trackProducerLabel ) {
    const reco::TrackCollection *trackCollection = 0;
    try {
      edm::Handle<reco::TrackCollection> trackCollectionHandle;
      iEvent.getByLabel(*trackProducerLabel,trackCollectionHandle);
      trackCollection = trackCollectionHandle.product();
    }
    catch (edm::Exception const& x) {
      if ( x.categoryCode() == edm::errors::ProductNotFound ) {
	if ( x.history().size() == 1 ) {
	  edm::LogWarning("GutSoftSameNumberOfTracksFilterFilter") << "Collection reco::TrackCollection with label " << *trackProducerLabel << " cannot be found.";
	}
      }
    }
    if ( trackCollection ) {
      trackCollections.push_back(trackCollection);
    }
  }

  unsigned int counter = 0;
  for ( std::vector<const reco::TrackCollection*>::const_iterator trackCollection = trackCollections.begin();
	trackCollection != trackCollections.end();
	++trackCollection ) {
    if ( (*trackCollection)->size() == numberOfTracks_ ) {
      ++counter;
    }
  }

  if ( counter == trackCollections.size() ) {
    result = true;
  }

  return result;
}

void 
GutSoftSameNumberOfTracksFilter::beginJob(const edm::EventSetup&)
{
}

void 
GutSoftSameNumberOfTracksFilter::endJob() {
}
