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
// $Date: 2007/01/22 01:35:07 $
// $Revision: 1.5 $
//

#include <string>

#include "GutSoftAnalyzers/GutSoftTrackAnalyzer/interface/GutSoftTrackAnalyzer.h"

#include "GutSoftTools/GutSoftHistogramFileService/interface/GutSoftHistogramFileService.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/SiStripDetId/interface/StripSubdetector.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"

GutSoftTrackAnalyzer::GutSoftTrackAnalyzer(const edm::ParameterSet& iConfig)
{

  trackInputTag_ = iConfig.getUntrackedParameter<edm::InputTag>("TrackInputTag");
  baseDirectoryName_  = iConfig.getUntrackedParameter<std::string>("BaseDirectoryName");

  // GutSoftHistogramFactory
  histograms_ = edm::Service<GutSoftHistogramFileService>()->getFactory();

}


GutSoftTrackAnalyzer::~GutSoftTrackAnalyzer()
{
 
}


void
GutSoftTrackAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  // set baseDirectory in GutSoftHistogramFactory
  histograms_->setBaseDirectory(baseDirectoryName_);

  const reco::TrackCollection *trackCollection = 0;
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
	edm::LogWarning("GutSoftTrackAnalyzer") << "Collection reco::TrackCollection with label " << trackInputTag_ << " cannot be found, using empty collection of same type";
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
    histograms_->fill("qoverp",track->qoverp());
    histograms_->fill("lambda",track->lambda());
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
    
    histograms_->fill("nHitPerTrackVsEta",track->momentum().eta(),nHit);
    histograms_->fill("nStripHitPerTrackVsEta",track->momentum().eta(),nStripHit);
    histograms_->fill("nPixelHitPerTrackVsEta",track->momentum().eta(),nPixelHit);
    histograms_->fill("nTIBHitPerTrackVsEta",track->momentum().eta(),nTIBHit);
    histograms_->fill("nTOBHitPerTrackVsEta",track->momentum().eta(),nTOBHit);
    histograms_->fill("nTIDHitPerTrackVsEta",track->momentum().eta(),nTIDHit);
    histograms_->fill("nTECHitPerTrackVsEta",track->momentum().eta(),nTECHit);
    histograms_->fill("nPXBHitPerTrackVsEta",track->momentum().eta(),nPXBHit);
    histograms_->fill("nPXFHitPerTrackVsEta",track->momentum().eta(),nPXFHit);
    histograms_->fill("nHitPerTrackVsPhi",track->momentum().phi(),nHit);

  }

}


void 
GutSoftTrackAnalyzer::beginJob(const edm::EventSetup&)
{

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

  unsigned int qoverp_nbins = 100;
  double       qoverp_low   = -0.25;
  double       qoverp_high  =  0.25;

  unsigned int lambda_nbins = 100;
  double       lambda_low   = -5.;
  double       lambda_high  = 5.;

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
  histograms_->bookHistogram("qoverp","Q/p",
			     nTrackDirectory,qoverp_nbins,qoverp_low,qoverp_high,
			     "Q/p [1/GeV]","");
  histograms_->bookHistogram("lambda","lambda",
			     nTrackDirectory,lambda_nbins,lambda_low,lambda_high,
			     "lambda","Events");
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
			     nTrackDirectory,eta_nbins,eta_low,eta_high,nhit_nbins,nhit_low,nhit_high,
			     "n_{Hit}","#eta","Events");
  histograms_->bookHistogram("nHitPerTrackCandidateVsEta","Number of hits per track candidate vs. #eta",
			     nTrackDirectory,eta_nbins,eta_low,eta_high,nhit_nbins,nhit_low,nhit_high,
			     "n_{Hit}","#eta","Events");
  histograms_->bookHistogram("nHitPerTrackVsPhi","Number of hits per track vs. #phi",
			     nTrackDirectory,phi_nbins,phi_low,phi_high,nhit_nbins,nhit_low,nhit_high,
			     "n_{Hit}","#phi","Events");

  histograms_->bookHistogram("nStripHitPerTrackVsEta","Number of strip hits per track vs. #eta",
			     nTrackDirectory,eta_nbins,eta_low,eta_high,nhit_nbins,nhit_low,nhit_high,
			     "n_{Hit}","#eta","Events");
  histograms_->bookHistogram("nPixelHitPerTrackVsEta","Number of pixel hits per track vs. #eta",
			     nTrackDirectory,eta_nbins,eta_low,eta_high,nhit_nbins,nhit_low,nhit_high,
			     "n_{Hit}","#eta","Events");
  histograms_->bookHistogram("nTIBHitPerTrackVsEta","Number of TIB hits per track vs. #eta",
			     nTrackDirectory,eta_nbins,eta_low,eta_high,nhit_nbins,nhit_low,nhit_high,
			     "n_{Hit}","#eta","Events");
  histograms_->bookHistogram("nTOBHitPerTrackVsEta","Number of TOB hits per track vs. #eta",
			     nTrackDirectory,eta_nbins,eta_low,eta_high,nhit_nbins,nhit_low,nhit_high,
			     "n_{Hit}","#eta","Events");
  histograms_->bookHistogram("nTIDHitPerTrackVsEta","Number of TID hits per track vs. #eta",
			     nTrackDirectory,eta_nbins,eta_low,eta_high,nhit_nbins,nhit_low,nhit_high,
			     "n_{Hit}","#eta","Events");
  histograms_->bookHistogram("nTECHitPerTrackVsEta","Number of TEC hits per track vs. #eta",
			     nTrackDirectory,eta_nbins,eta_low,eta_high,nhit_nbins,nhit_low,nhit_high,
			     "n_{Hit}","#eta","Events");
  histograms_->bookHistogram("nPXBHitPerTrackVsEta","Number of PXB hits per track vs. #eta",
			     nTrackDirectory,eta_nbins,eta_low,eta_high,nhit_nbins,nhit_low,nhit_high,
			     "n_{Hit}","#eta","Events");
  histograms_->bookHistogram("nPXFHitPerTrackVsEta","Number of PXF hits per track vs. #eta",
			     nTrackDirectory,eta_nbins,eta_low,eta_high,nhit_nbins,nhit_low,nhit_high,
			     "n_{Hit}","#eta","Events");
}

void 
GutSoftTrackAnalyzer::endJob() {

}
