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
// $Date: 2007/01/22 01:35:09 $
// $Revision: 1.5 $
//

#include <string>

#include "GutSoftAnalyzers/GutSoftTrackCandidateAnalyzer/interface/GutSoftTrackCandidateAnalyzer.h"

#include "GutSoftTools/GutSoftHistogramFileService/interface/GutSoftHistogramFileService.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/TrackCandidate/interface/TrackCandidate.h"
#include "DataFormats/TrackCandidate/interface/TrackCandidateCollection.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/SiStripDetId/interface/StripSubdetector.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"

#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"

#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"

#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"

GutSoftTrackCandidateAnalyzer::GutSoftTrackCandidateAnalyzer(const edm::ParameterSet& iConfig)
{

  trackCandidateInputTag_  = iConfig.getUntrackedParameter<edm::InputTag>("TrackCandidateInputTag");
  baseDirectoryName_            = iConfig.getUntrackedParameter<std::string>("BaseDirectoryName");

  // GutSoftHistogramFactory
  histograms_ = edm::Service<GutSoftHistogramFileService>()->getFactory();

}


GutSoftTrackCandidateAnalyzer::~GutSoftTrackCandidateAnalyzer()
{

}


void
GutSoftTrackCandidateAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  // set baseDirectory in GutSoftHistogramFactory
  histograms_->setBaseDirectory(baseDirectoryName_);

  const TrackCandidateCollection *trackCandidateCollection = 0;
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
	edm::LogWarning("GutSoftTrackCandidateAnalyzer") << "Collection TrackCandidateCollection with label " << trackCandidateInputTag_ << " cannot be found, using empty collection of same type";
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

    const TrackingRecHit *outerHit = 0;
    for ( TrackCandidate::RecHitContainer::const_iterator recHit = candidate->recHits().first;
	  recHit != candidate->recHits().second;
	  ++recHit ) {
      ++nHit;
      outerHit = &(*recHit);
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
    
    // take eta of last seed hit
    double eta  = trackerGeometry_->idToDet(outerHit->geographicalId())->surface().toGlobal(outerHit->localPosition()).eta();

    histograms_->fill("nHitPerTrackCandidateVsEta",eta,nHit);
    histograms_->fill("nStripHitPerTrackCandidateVsEta",eta,nStripHit);
    histograms_->fill("nPixelHitPerTrackCandidateVsEta",eta,nPixelHit);
    histograms_->fill("nTIBHitPerTrackCandidateVsEta",eta,nTIBHit);
    histograms_->fill("nTOBHitPerTrackCandidateVsEta",eta,nTOBHit);
    histograms_->fill("nTIDHitPerTrackCandidateVsEta",eta,nTIDHit);
    histograms_->fill("nTECHitPerTrackCandidateVsEta",eta,nTECHit);
    histograms_->fill("nPXBHitPerTrackCandidateVsEta",eta,nPXBHit);
    histograms_->fill("nPXFHitPerTrackCandidateVsEta",eta,nPXFHit);

  }

}


void 
GutSoftTrackCandidateAnalyzer::beginJob(const edm::EventSetup& es)
{

  // get tracker geometry
  edm::ESHandle<TrackerGeometry> trackerHandle;
  es.get<TrackerDigiGeometryRecord>().get(trackerHandle);
  trackerGeometry_ = trackerHandle.product();

  // binning for histograms
  unsigned int nTrackCandidates_nbins    = 100000;
  unsigned int nTrackCandidates_low      = 0;
  unsigned int nTrackCandidates_high     = 100000;
  std::string  nTrackCandidatesDirectory = baseDirectoryName_;

  unsigned int eta_nbins = 60;
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
			     nTrackCandidatesDirectory,eta_nbins,eta_low,eta_high,nhit_nbins,nhit_low,nhit_high,
			     "n_{Hit}","#eta","Events");
  histograms_->bookHistogram("nStripHitPerTrackCandidateVsEta","Number of strip hits per track candidate vs. #eta",
			     nTrackCandidatesDirectory,eta_nbins,eta_low,eta_high,nhit_nbins,nhit_low,nhit_high,
			     "n_{Hit}","#eta","Events");
  histograms_->bookHistogram("nPixelHitPerTrackCandidateVsEta","Number of pixel hits per track candidate vs. #eta",
			     nTrackCandidatesDirectory,eta_nbins,eta_low,eta_high,nhit_nbins,nhit_low,nhit_high,
			     "n_{Hit}","#eta","Events");
  histograms_->bookHistogram("nTIBHitPerTrackCandidateVsEta","Number of TIB hits per track candidate vs. #eta",
			     nTrackCandidatesDirectory,eta_nbins,eta_low,eta_high,nhit_nbins,nhit_low,nhit_high,
			     "n_{Hit}","#eta","Events");
  histograms_->bookHistogram("nTOBHitPerTrackCandidateVsEta","Number of TOB hits per track candidate vs. #eta",
			     nTrackCandidatesDirectory,eta_nbins,eta_low,eta_high,nhit_nbins,nhit_low,nhit_high,
			     "n_{Hit}","#eta","Events");
  histograms_->bookHistogram("nTIDHitPerTrackCandidateVsEta","Number of TID hits per track candidate vs. #eta",
			     nTrackCandidatesDirectory,eta_nbins,eta_low,eta_high,nhit_nbins,nhit_low,nhit_high,
			     "n_{Hit}","#eta","Events");
  histograms_->bookHistogram("nTECHitPerTrackCandidateVsEta","Number of TEC hits per track candidate vs. #eta",
			     nTrackCandidatesDirectory,eta_nbins,eta_low,eta_high,nhit_nbins,nhit_low,nhit_high,
			     "n_{Hit}","#eta","Events");
  histograms_->bookHistogram("nPXBHitPerTrackCandidateVsEta","Number of PXB hits per track candidate vs. #eta",
			     nTrackCandidatesDirectory,eta_nbins,eta_low,eta_high,nhit_nbins,nhit_low,nhit_high,
			     "n_{Hit}","#eta","Events");
  histograms_->bookHistogram("nPXFHitPerTrackCandidateVsEta","Number of PXF hits per track candidate vs. #eta",
			     nTrackCandidatesDirectory,eta_nbins,eta_low,eta_high,nhit_nbins,nhit_low,nhit_high,
			     "n_{Hit}","#eta","Events");
}

void 
GutSoftTrackCandidateAnalyzer::endJob() {

}
