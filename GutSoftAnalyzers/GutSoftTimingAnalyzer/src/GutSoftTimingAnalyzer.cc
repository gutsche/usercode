//
// Package:         GutSoftAnalyzers/GutSoftTimingAnalyzer
// Class:           GutSoftTimingAnalyzer
// 
// Description:     EDAnalyzer for timing informattion stored in collection
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Thu Jan 11 17:31:01 UTC 2007
//
// $Author: gutsche $
// $Date: 2007/01/11 01:51:04 $
// $Revision: 1.1 $
//

#include <string>
#include <cmath>

#include "GutSoftAnalyzers/GutSoftTimingAnalyzer/interface/GutSoftTimingAnalyzer.h"

#include "GutSoftTools/GutSoftHistogramFileService/interface/GutSoftHistogramFileService.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/HLTReco/interface/ModuleTiming.h"

#include "DataFormats/TrajectorySeed/interface/TrajectorySeed.h"
#include "DataFormats/TrajectorySeed/interface/TrajectorySeedCollection.h"
#include "DataFormats/RoadSearchCloud/interface/RoadSearchCloud.h"
#include "DataFormats/RoadSearchCloud/interface/RoadSearchCloudCollection.h"
#include "DataFormats/TrackCandidate/interface/TrackCandidate.h"
#include "DataFormats/TrackCandidate/interface/TrackCandidateCollection.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

GutSoftTimingAnalyzer::GutSoftTimingAnalyzer(const edm::ParameterSet& iConfig)
{

  // input tags
  timingInputTag_                = iConfig.getUntrackedParameter<edm::InputTag>("TimingInputTag");

  normalizationAnalyzerInputTag_ = iConfig.getUntrackedParameter<edm::InputTag>("NormalizationAnalyzerInputTag");

  seedInputTag_                  = iConfig.getUntrackedParameter<edm::InputTag>("SeedInputTag");
  cloudInputTag_                 = iConfig.getUntrackedParameter<edm::InputTag>("CloudInputTag");
  trackCandidateInputTag_        = iConfig.getUntrackedParameter<edm::InputTag>("TrackCandidateInputTag");
  trackInputTag_                 = iConfig.getUntrackedParameter<edm::InputTag>("TrackInputTag");

  normalization_                 = iConfig.getUntrackedParameter<double>("Normalization");

  time_nbins_                    = iConfig.getUntrackedParameter<unsigned int>("Time_NBins");
  time_low_                      = iConfig.getUntrackedParameter<double>("Time_LowerBorder");
  time_high_                     = iConfig.getUntrackedParameter<double>("Time_UpperBorder");

  numSeeds_nbins_                = iConfig.getUntrackedParameter<unsigned int>("NumSeeds_NBins");
  numSeeds_low_                  = iConfig.getUntrackedParameter<double>("NumSeeds_LowerBorder");
  numSeeds_high_                 = iConfig.getUntrackedParameter<double>("NumSeeds_UpperBorder");

  numClouds_nbins_               = iConfig.getUntrackedParameter<unsigned int>("NumClouds_NBins");
  numClouds_low_                 = iConfig.getUntrackedParameter<double>("NumClouds_LowerBorder");
  numClouds_high_                = iConfig.getUntrackedParameter<double>("NumClouds_UpperBorder");

  numTrackCandidates_nbins_      = iConfig.getUntrackedParameter<unsigned int>("NumTrackCandidates_NBins");
  numTrackCandidates_low_        = iConfig.getUntrackedParameter<double>("NumTrackCandidates_LowerBorder");
  numTrackCandidates_high_       = iConfig.getUntrackedParameter<double>("NumTrackCandidates_UpperBorder");

  numTracks_nbins_               = iConfig.getUntrackedParameter<unsigned int>("NumTracks_NBins");
  numTracks_low_                 = iConfig.getUntrackedParameter<double>("NumTracks_LowerBorder");
  numTracks_high_                = iConfig.getUntrackedParameter<double>("NumTracks_UpperBorder");

  // base directory for histograms
  baseDirectoryName_            = iConfig.getUntrackedParameter<std::string>("BaseDirectoryName");

  // GutSoftHistogramFactory
  histograms_                   = edm::Service<GutSoftHistogramFileService>()->getFactory();

  first_ = true;

}


GutSoftTimingAnalyzer::~GutSoftTimingAnalyzer()
{
 
}


void
GutSoftTimingAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  if ( first_ ) {
    first_ = false;
  } else {

  // set baseDirectory in GutSoftHistogramFactory
  histograms_->setBaseDirectory(baseDirectoryName_);

  // get timing collection
  const edm::EventTime *eventTime = 0;
  edm::Handle<edm::EventTime> eventTimeHandle;
  try {
    iEvent.getByLabel(timingInputTag_,eventTimeHandle);
    eventTime = eventTimeHandle.product();
  }
  catch (edm::Exception const& x) {
    if ( x.categoryCode() == edm::errors::ProductNotFound ) {
      if ( x.history().size() == 1 ) {
	edm::LogWarning("GutSoftTimingAnalyzer") << "Collection edm::EventTime with label " << timingInputTag_ << " cannot be found. Not filling histograms.";
      }
    }
  }

  // get seed collection
  const TrajectorySeedCollection *seedCollection = 0;
  if ( seedInputTag_.label() != "dummy" ) {
    try {
      edm::Handle<TrajectorySeedCollection> seedCollectionHandle;
      iEvent.getByLabel(seedInputTag_,seedCollectionHandle);
      seedCollection = seedCollectionHandle.product();
    }
    catch (edm::Exception const& x) {
      if ( x.categoryCode() == edm::errors::ProductNotFound ) {
	if ( x.history().size() == 1 ) {
	  static const TrajectorySeedCollection s_empty;
	  seedCollection = &s_empty;
	  edm::LogWarning("GutSoftTimingAnalyzer") << "Collection TrajectorySeedCollection with label " << seedInputTag_ << " cannot be found, using empty collection of same type";
	}
      }
    }
  }

  // get cloud collection
  const RoadSearchCloudCollection *cloudCollection = 0;
  if ( cloudInputTag_.label() != "dummy" ) {
    try {
      edm::Handle<RoadSearchCloudCollection> cloudCollectionHandle;
      iEvent.getByLabel(cloudInputTag_,cloudCollectionHandle);
      cloudCollection = cloudCollectionHandle.product();
    }
    catch (edm::Exception const& x) {
      if ( x.categoryCode() == edm::errors::ProductNotFound ) {
	if ( x.history().size() == 1 ) {
	  static const RoadSearchCloudCollection s_empty;
	  cloudCollection = &s_empty;
	  edm::LogWarning("GutSoftTimingAnalyzer") << "Collection RoadSearchCloudCollection with label " << cloudInputTag_ << " cannot be found, using empty collection of same type";
	}
      }
    }
  }

  // get track candidate collection
  const TrackCandidateCollection *trackCandidateCollection = 0;
  if ( trackCandidateInputTag_.label() != "dummy" ) {
    try {
      edm::Handle<TrackCandidateCollection> trackCandidateCollectionHandle;
      iEvent.getByLabel(trackCandidateInputTag_,trackCandidateCollectionHandle);
      trackCandidateCollection = trackCandidateCollectionHandle.product();
    }
    catch (edm::Exception const& x) {
      if ( x.categoryCode() == edm::errors::ProductNotFound ) {
	if ( x.history().size() == 1 ) {
	  static const TrackCandidateCollection s_empty;
	  trackCandidateCollection = &s_empty;
	  edm::LogWarning("GutSoftTimingAnalyzer") << "Collection TrackCandidateCollection with label " << trackCandidateInputTag_ << " cannot be found, using empty collection of same type";
	}
      }
    }
  }

  // get track collection
  const reco::TrackCollection *trackCollection = 0;
  if ( trackInputTag_.label() != "dummy" ) {
    try {
      edm::Handle<reco::TrackCollection> trackCollectionHandle;
      iEvent.getByLabel(trackInputTag_,trackCollectionHandle);
      trackCollection = trackCollectionHandle.product();
    }
    catch (edm::Exception const& x) {
      if ( x.categoryCode() == edm::errors::ProductNotFound ) {
	if ( x.history().size() == 1 ) {
	  static const reco::TrackCollection s_empty;
	  trackCollection = &s_empty;
	  edm::LogWarning("GutSoftTimingAnalyzer") << "Collection reco::TrackCollection with label " << trackInputTag_ << " cannot be found, using empty collection of same type";
	}
      }
    }
  }
  
  double normalization  = 0.0;
  double seed           = 0.0;
  double cloud          = 0.0;
  double trackCandidate = 0.0;
  double track          = 0.0;

  if ( eventTime != 0 ) {
    for ( unsigned int i = 0; 
	  i < eventTime->size();
	  ++i ) {
      if ( eventTime->name(i) == normalizationAnalyzerInputTag_.label() ) {
	normalization = eventTime->time(i);
      } else if ( eventTime->name(i) == seedInputTag_.label() ) {
	seed = eventTime->time(i);
      } else if ( eventTime->name(i) == cloudInputTag_.label() ) {
	cloud = eventTime->time(i);
      } else if ( eventTime->name(i) == trackCandidateInputTag_.label() ) {
	trackCandidate = eventTime->time(i);
      } else if ( eventTime->name(i) == trackInputTag_.label() ) {
	track = eventTime->time(i);
      } 
    }
  }

  // normalization
  if ( normalization != 0.0 ) {
    seed           = seed * normalization_ / normalization;
    cloud          = cloud * normalization_ / normalization;
    trackCandidate = trackCandidate * normalization_ / normalization;
    track          = track * normalization_ / normalization;
  }

  if ( seed != 0.0 ) {
    histograms_->fill("seeds",seedCollection->size(),seed);
  }

  if ( cloud != 0.0 ) {
    histograms_->fill("clouds",cloudCollection->size(),cloud);
  }

  if ( trackCandidate != 0.0 ) {
    histograms_->fill("trackCandidates",trackCandidateCollection->size(),trackCandidate);
  }

  if ( track != 0.0 ) {
    histograms_->fill("tracks",trackCollection->size(),track);
  }

  }  

}


void 
GutSoftTimingAnalyzer::beginJob(const edm::EventSetup& setup)
{

  // binning for histograms
  std::string  directory = baseDirectoryName_;

  if ( seedInputTag_.label() != "dummy" ) {
    histograms_->bookHistogram("seeds","Seeding Time vs. Number of Seeds",
			       directory,numSeeds_nbins_,numSeeds_low_,numSeeds_high_,time_nbins_,time_low_,time_high_,
			       "n_{seeds}","time [s]","Events");
  }
  if ( cloudInputTag_.label() != "dummy" ) {
    histograms_->bookHistogram("clouds","Cloud Reconstruction Time vs. Number of Clouds",
			       directory,numClouds_nbins_,numClouds_low_,numClouds_high_,time_nbins_,time_low_,time_high_,
			       "n_{clouds}","time [s]","Events");
  }
  if ( trackCandidateInputTag_.label() != "dummy" ) {
    histograms_->bookHistogram("trackCandidates","Track Candidate Reconstruction Time vs. Number of Track Candidatess",
			       directory,numTrackCandidates_nbins_,numTrackCandidates_low_,numTrackCandidates_high_,time_nbins_,time_low_,time_high_,
			       "n_{track candidates}","time [s]","Events");
  }
  if ( trackInputTag_.label() != "dummy" ) {
    histograms_->bookHistogram("tracks","Final Track Fit Time vs. Number of Tracks",
			       directory,numTracks_nbins_,numTracks_low_,numTracks_high_,time_nbins_,time_low_,time_high_,
			       "n_{tracks}","time [s]","Events");
  }
}

void 
GutSoftTimingAnalyzer::endJob() {

}
