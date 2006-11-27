//
// Package:         GutSoftAnalyzers/GutSoftTrackCandidateAnalyzer
// Class:           GutSoftTrackCandidateAnalyzer
// 
// Description:     EDAnalyzer for TrackCandidates
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Wed Oct 18 01:05:12 UTC 2006
//
// $Author: gutsche $
// $Date: 2006/10/25 02:07:30 $
// $Revision: 1.2 $
//

#include <string>

#include "GutSoftAnalyzers/GutSoftTrackCandidateAnalyzer/interface/GutSoftTrackCandidateAnalyzer.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/TrackCandidate/interface/TrackCandidate.h"
#include "DataFormats/TrackCandidate/interface/TrackCandidateCollection.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/SiStripDetId/interface/StripSubdetector.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"

GutSoftTrackCandidateAnalyzer::GutSoftTrackCandidateAnalyzer(const edm::ParameterSet& iConfig)
{

  outputFileName_               = iConfig.getUntrackedParameter<std::string>("OutputFileName");
  trackCandidateProducerLabel_  = iConfig.getUntrackedParameter<std::string>("TrackCandidateProducerLabel");
  baseDirectoryName_            = iConfig.getUntrackedParameter<std::string>("BaseDirectoryName");

}


GutSoftTrackCandidateAnalyzer::~GutSoftTrackCandidateAnalyzer()
{

}


void
GutSoftTrackCandidateAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  const TrackCandidateCollection *trackCandidateCollection = 0;
  try {
    edm::Handle<TrackCandidateCollection> trackCandidateCollectionHandle;
    iEvent.getByLabel(trackCandidateProducerLabel_,trackCandidateCollectionHandle);
    trackCandidateCollection = trackCandidateCollectionHandle.product();
  }
  catch (edm::Exception const& x) {
    if ( x.categoryCode() == edm::errors::ProductNotFound ) {
      if ( x.history().size() == 1 ) {
	trackCandidateCollection = new TrackCandidateCollection();
	edm::LogWarning("GutSoftTrackCandidateAnalyzer") << "Collection TrackCandidateCollection with label " << trackCandidateProducerLabel_ << " cannot be found, using empty collection of same type";
      }
    }
  }

  // track candidates
  histograms_->fill("nTrackCandidates",trackCandidateCollection->size());
  for ( TrackCandidateCollection::const_iterator candidate = trackCandidateCollection->begin();
	candidate != trackCandidateCollection->end();
	++candidate ) {

    // loop over hits in tracks, count
    unsigned int nHit      = 0;
    unsigned int nStripHit = 0;
    unsigned int nPixelHit = 0;
    unsigned int nTIBHit   = 0;
    unsigned int nTOBHit   = 0;
    unsigned int nTIDHit   = 0;
    unsigned int nTECHit   = 0;
    unsigned int nPXBHit   = 0;
    unsigned int nPXFHit   = 0;

    for ( TrackCandidate::RecHitContainer::const_iterator recHit = candidate->recHits().first;
	  recHit != candidate->recHits().second;
	  ++recHit ) {
      ++nHit;
      DetId id(recHit->geographicalId());
      
      if ( (unsigned int)id.subdetId() == StripSubdetector::TIB ) {
	++nStripHit;
	++nTIBHit;
      } else if ( (unsigned int)id.subdetId() == StripSubdetector::TOB ) {
	++nStripHit;
	++nTOBHit;
      } else if ( (unsigned int)id.subdetId() == StripSubdetector::TID ) {
	++nStripHit;
	++nTIDHit;
      } else if ( (unsigned int)id.subdetId() == StripSubdetector::TEC ) {
	++nStripHit;
	++nTECHit;
      } else if ( (unsigned int)id.subdetId() == PixelSubdetector::PixelBarrel ) {
	++nPixelHit;
	++nPXBHit;
      } else if ( (unsigned int)id.subdetId() == PixelSubdetector::PixelEndcap ) {
	++nPixelHit;
	++nPXFHit;
      }
    }
    
    histograms_->fill("nHitPerTrackCandidateVsEta",nHit,0.);
    histograms_->fill("nStripHitPerTrackCandidateVsEta",nStripHit,0.);
    histograms_->fill("nPixelHitPerTrackCandidateVsEta",nPixelHit,0.);
    histograms_->fill("nTIBHitPerTrackCandidateVsEta",nTIBHit,0.);
    histograms_->fill("nTOBHitPerTrackCandidateVsEta",nTOBHit,0.);
    histograms_->fill("nTIDHitPerTrackCandidateVsEta",nTIDHit,0.);
    histograms_->fill("nTECHitPerTrackCandidateVsEta",nTECHit,0.);
    histograms_->fill("nPXBHitPerTrackCandidateVsEta",nPXBHit,0.);
    histograms_->fill("nPXFHitPerTrackCandidateVsEta",nPXFHit,0.);

  }

}


void 
GutSoftTrackCandidateAnalyzer::beginJob(const edm::EventSetup&)
{
  // GutSoftHistogramFactory
  histograms_ = new GutSoftHistogramFactory(outputFileName_);

  // binning for histograms
  unsigned int nTrackCandidates_nbins    = 100000;
  unsigned int nTrackCandidates_low      = 0;
  unsigned int nTrackCandidates_high     = 100000;
  std::string  nTrackCandidatesDirectory = baseDirectoryName_;

  unsigned int eta_nbins = 30;
  double       eta_low   = -3.;
  double       eta_high  =  3.;

  unsigned int nhit_nbins = 31;
  double       nhit_low   = -0.5;
  double       nhit_high  = 30.5;

  // book histogram
  histograms_->bookHistogram("nTrackCandidates","Number of track candidates per event",
			     nTrackCandidatesDirectory,nTrackCandidates_nbins,nTrackCandidates_low,nTrackCandidates_high,
			     "n_{TrackCandidates}","Events");
  histograms_->bookHistogram("nHitPerTrackCandidateVsEta","Number of hits per track candidate vs. #eta",
			     nTrackCandidatesDirectory,nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,
			     "n_{Hit}","#eta","Events");
  histograms_->bookHistogram("nStripHitPerTrackCandidateVsEta","Number of strip hits per track candidate vs. #eta",
			     nTrackCandidatesDirectory,nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,
			     "n_{Hit}","#eta","Events");
  histograms_->bookHistogram("nPixelHitPerTrackCandidateVsEta","Number of pixel hits per track candidate vs. #eta",
			     nTrackCandidatesDirectory,nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,
			     "n_{Hit}","#eta","Events");
  histograms_->bookHistogram("nTIBHitPerTrackCandidateVsEta","Number of TIB hits per track candidate vs. #eta",
			     nTrackCandidatesDirectory,nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,
			     "n_{Hit}","#eta","Events");
  histograms_->bookHistogram("nTOBHitPerTrackCandidateVsEta","Number of TOB hits per track candidate vs. #eta",
			     nTrackCandidatesDirectory,nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,
			     "n_{Hit}","#eta","Events");
  histograms_->bookHistogram("nTIDHitPerTrackCandidateVsEta","Number of TID hits per track candidate vs. #eta",
			     nTrackCandidatesDirectory,nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,
			     "n_{Hit}","#eta","Events");
  histograms_->bookHistogram("nTECHitPerTrackCandidateVsEta","Number of TEC hits per track candidate vs. #eta",
			     nTrackCandidatesDirectory,nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,
			     "n_{Hit}","#eta","Events");
  histograms_->bookHistogram("nPXBHitPerTrackCandidateVsEta","Number of PXB hits per track candidate vs. #eta",
			     nTrackCandidatesDirectory,nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,
			     "n_{Hit}","#eta","Events");
  histograms_->bookHistogram("nPXFHitPerTrackCandidateVsEta","Number of PXF hits per track candidate vs. #eta",
			     nTrackCandidatesDirectory,nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,
			     "n_{Hit}","#eta","Events");
}

void 
GutSoftTrackCandidateAnalyzer::endJob() {

  // delete GutSoftHistogramFactory, histogram file is written out and can be handled in module endJob functions of the following modules
  if (histograms_) {
    delete histograms_;
    histograms_ = 0;
  }

}
