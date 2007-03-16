//
// Package:         GutSoftAnalyzers/GutSoftTrackAnalyzer
// Class:           GutSoftTrackAnalyzer
// 
// Description:     EDAnalyzer for tracks
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Wed Oct 18 01:25:17 UTC 2006
//
// $Author: gutsche $
// $Date: 2006/10/25 02:07:30 $
// $Revision: 1.2 $
//

#include <string>

#include "GutSoftAnalyzers/GutSoftTrackAnalyzer/interface/GutSoftTrackAnalyzer.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/SiStripDetId/interface/StripSubdetector.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"

GutSoftTrackAnalyzer::GutSoftTrackAnalyzer(const edm::ParameterSet& iConfig)
{

  outputFileName_     = iConfig.getUntrackedParameter<std::string>("OutputFileName");
  trackProducerLabel_ = iConfig.getUntrackedParameter<std::string>("TrackProducerLabel");
  baseDirectoryName_  = iConfig.getUntrackedParameter<std::string>("BaseDirectoryName");

}


GutSoftTrackAnalyzer::~GutSoftTrackAnalyzer()
{
 
}


void
GutSoftTrackAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  const reco::TrackCollection *trackCollection = 0;
  try {
    edm::Handle<reco::TrackCollection> trackCollectionHandle;
    iEvent.getByLabel(trackProducerLabel_,trackCollectionHandle);
    trackCollection = trackCollectionHandle.product();
  }
  catch (edm::Exception const& x) {
    if ( x.categoryCode() == edm::errors::ProductNotFound ) {
      if ( x.history().size() == 1 ) {
	trackCollection = new reco::TrackCollection();
	edm::LogWarning("GutSoftTrackAnalyzer") << "Collection reco::TrackCollection with label " << trackProducerLabel_ << " cannot be found, using empty collection of same type";
      }
    }
  }

  // tracks
  histograms_->fill("nTracks",trackCollection->size());
  for ( reco::TrackCollection::const_iterator track = trackCollection->begin();
	track != trackCollection->end();
	++track ) {
    histograms_->fill("chi2",track->normalizedChi2());
    histograms_->fill("pt",track->pt());
    histograms_->fill("nhit",track->found());
    histograms_->fill("eta",track->momentum().eta());
    histograms_->fill("phi",track->momentum().phi());
    histograms_->fill("transCurv",track->transverseCurvature());
    histograms_->fill("phiZero",track->phi0());
    histograms_->fill("theta",track->theta());
    histograms_->fill("dZero",track->d0());
    histograms_->fill("dZ",track->dz());

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

    for ( trackingRecHit_iterator recHit = track->recHitsBegin();
	  recHit != track->recHitsEnd();
	  ++recHit ) {
      if ( (*recHit)->isValid() ) {
	++nHit;
	DetId id((*recHit)->geographicalId());

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
    }
    
    histograms_->fill("nHitPerTrackVsEta",nHit,track->momentum().eta());
    histograms_->fill("nStripHitPerTrackVsEta",nStripHit,track->momentum().eta());
    histograms_->fill("nPixelHitPerTrackVsEta",nPixelHit,track->momentum().eta());
    histograms_->fill("nTIBHitPerTrackVsEta",nTIBHit,track->momentum().eta());
    histograms_->fill("nTOBHitPerTrackVsEta",nTOBHit,track->momentum().eta());
    histograms_->fill("nTIDHitPerTrackVsEta",nTIDHit,track->momentum().eta());
    histograms_->fill("nTECHitPerTrackVsEta",nTECHit,track->momentum().eta());
    histograms_->fill("nPXBHitPerTrackVsEta",nPXBHit,track->momentum().eta());
    histograms_->fill("nPXFHitPerTrackVsEta",nPXFHit,track->momentum().eta());
    histograms_->fill("nHitPerTrackVsPhi",nHit,track->momentum().phi());

  }

}


void 
GutSoftTrackAnalyzer::beginJob(const edm::EventSetup&)
{

  // GutSoftHistogramFactory
  histograms_ = new GutSoftHistogramFactory(outputFileName_);

  // binning for histograms
  std::string  nTrackDirectory = baseDirectoryName_;

  unsigned int chi2_nbins = 50;
  double       chi2_low   = 0.;
  double       chi2_high  = 50.;
  
  unsigned int pt_nbins = 160;
  double       pt_low   = 0.;
  double       pt_high  = 20.;

  unsigned int eta_nbins = 30;
  double       eta_low   = -3.;
  double       eta_high  =  3.;

  unsigned int phi_nbins = 36;
  double       phi_low   = -3.1416;
  double       phi_high  = 3.1416;

  unsigned int transCurv_nbins = 100;
  double       transCurv_low   = -0.25;
  double       transCurv_high  =  0.25;

  unsigned int phiZero_nbins = 100;
  double       phiZero_low   = -5.;
  double       phiZero_high  = 5.;

  unsigned int theta_nbins = 40;
  double       theta_low   = 0.;
  double       theta_high  = 4.;

  unsigned int dZero_nbins = 120;
  double       dZero_low   = -0.1;
  double       dZero_high  = 0.1;

  unsigned int dZ_nbins = 400;
  double       dZ_low   = -100.;
  double       dZ_high  = 100.;

  unsigned int nTracks_nbins = 100000;
  unsigned int nTracks_low   = 0;
  unsigned int nTracks_high  = 100000;

  unsigned int nhit_nbins = 31;
  double       nhit_low   = -0.5;
  double       nhit_high  = 30.5;

  // book histogram
  histograms_->bookHistogram("chi2","Track reduced chi^{2}",
			     nTrackDirectory,chi2_nbins,chi2_low,chi2_high,
			     "#chi^2","Events");
  histograms_->bookHistogram("pt","Track p_{T} (GeV)",
			     nTrackDirectory,pt_nbins,pt_low,pt_high,
			     "p_{T} [GeV]","Events");
  histograms_->bookHistogram("eta","Track #eta",
			     nTrackDirectory,eta_nbins,eta_low,eta_high,
			     "#eta","Events");
  histograms_->bookHistogram("phi","Track #phi",
			     nTrackDirectory,phi_nbins,phi_low,phi_high,
			     "#phi","Events");
  histograms_->bookHistogram("transCurv","Transverse curvature",
			     nTrackDirectory,transCurv_nbins,transCurv_low,transCurv_high,
			     "transCurv","");
  histograms_->bookHistogram("phiZero","phiZero",
			     nTrackDirectory,phiZero_nbins,phiZero_low,phiZero_high,
			     "#phi_{0}","Events");
  histograms_->bookHistogram("theta","Theta",
			     nTrackDirectory,theta_nbins,theta_low,theta_high,
			     "#theta","Events");
  histograms_->bookHistogram("dZero",",dZero",
			     nTrackDirectory,dZero_nbins,dZero_low,dZero_high,
			     "d_{0}","Events");
  histograms_->bookHistogram("dZ","dZ",
			     nTrackDirectory,dZ_nbins,dZ_low,dZ_high,
			     "d_{Z}","Events");
  histograms_->bookHistogram("nhit","Number of hits per track",
			     nTrackDirectory,nhit_nbins,nhit_low,nhit_high,
			     "n_{hit}","Events");

  histograms_->bookHistogram("nTracks","Number of tracks per event",
			     nTrackDirectory,nTracks_nbins,nTracks_low,nTracks_high,
			     "n_{Tracks}","Events");

  histograms_->bookHistogram("nHitPerTrackVsEta","Number of hits per track vs. #eta",
			     nTrackDirectory,nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,
			     "n_{Hit}","#eta","Events");
  histograms_->bookHistogram("nHitPerTrackCandidateVsEta","Number of hits per track candidate vs. #eta",
			     nTrackDirectory,nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,
			     "n_{Hit}","#eta","Events");
  histograms_->bookHistogram("nHitPerTrackVsPhi","Number of hits per track vs. #phi",
			     nTrackDirectory,nhit_nbins,nhit_low,nhit_high,phi_nbins,phi_low,phi_high,
			     "n_{Hit}","#phi","Events");

  histograms_->bookHistogram("nStripHitPerTrackVsEta","Number of strip hits per track vs. #eta",
			     nTrackDirectory,nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,
			     "n_{Hit}","#eta","Events");
  histograms_->bookHistogram("nPixelHitPerTrackVsEta","Number of pixel hits per track vs. #eta",
			     nTrackDirectory,nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,
			     "n_{Hit}","#eta","Events");
  histograms_->bookHistogram("nTIBHitPerTrackVsEta","Number of TIB hits per track vs. #eta",
			     nTrackDirectory,nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,
			     "n_{Hit}","#eta","Events");
  histograms_->bookHistogram("nTOBHitPerTrackVsEta","Number of TOB hits per track vs. #eta",
			     nTrackDirectory,nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,
			     "n_{Hit}","#eta","Events");
  histograms_->bookHistogram("nTIDHitPerTrackVsEta","Number of TID hits per track vs. #eta",
			     nTrackDirectory,nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,
			     "n_{Hit}","#eta","Events");
  histograms_->bookHistogram("nTECHitPerTrackVsEta","Number of TEC hits per track vs. #eta",
			     nTrackDirectory,nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,
			     "n_{Hit}","#eta","Events");
  histograms_->bookHistogram("nPXBHitPerTrackVsEta","Number of PXB hits per track vs. #eta",
			     nTrackDirectory,nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,
			     "n_{Hit}","#eta","Events");
  histograms_->bookHistogram("nPXFHitPerTrackVsEta","Number of PXF hits per track vs. #eta",
			     nTrackDirectory,nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,
			     "n_{Hit}","#eta","Events");
}

void 
GutSoftTrackAnalyzer::endJob() {

  // delete GutSoftHistogramFactory, histogram file is written out and can be handled in module endJob functions of the following modules
  if (histograms_) {
    delete histograms_;
    histograms_ = 0;
  }

}
