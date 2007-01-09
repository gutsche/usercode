//
// Package:         GutSoftAnalyzers/GutSoftTrackingEfficiencyAnalyzer
// Class:           GutSoftTrackingEfficiencyAnalyzer
// 
// Description:     EDAnalyzer for tracks
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Wed Oct 18 01:25:17 UTC 2006
//
// $Author: gutsche $
// $Date: 2006/11/29 01:44:38 $
// $Revision: 1.3 $
//

#include <string>
#include <cmath>

#include "GutSoftAnalyzers/GutSoftTrackingEfficiencyAnalyzer/interface/GutSoftTrackingEfficiencyAnalyzer.h"

#include "GutSoftTools/GutSoftHistogramFileService/interface/GutSoftHistogramFileService.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticleFwd.h"

#include "SimTracker/Records/interface/TrackAssociatorRecord.h"

GutSoftTrackingEfficiencyAnalyzer::GutSoftTrackingEfficiencyAnalyzer(const edm::ParameterSet& iConfig) :
  trackSelectorForEfficiencies_(iConfig.getParameter<edm::ParameterSet>("TrackCutsForEfficiencies")),
  trackSelectorForPurity_(iConfig.getParameter<edm::ParameterSet>("TrackCutsForPurity")),
  trackingParticleSelectorForEfficiencies_(iConfig.getParameter<edm::ParameterSet>("TrackingParticleCutsForEfficiencies")),
  trackingParticleSelectorForPurity_(iConfig.getParameter<edm::ParameterSet>("TrackingParticleCutsForPurity"))
{

  // input tags
  trackInputTag_                = iConfig.getUntrackedParameter<edm::InputTag>("TrackInputTag");
  trackingTruthInputTag_        = iConfig.getUntrackedParameter<edm::InputTag>("TrackingTruthInputTag");

  // base directory for histograms
  baseDirectoryName_            = iConfig.getUntrackedParameter<std::string>("BaseDirectoryName");

  // associator
  associatorName_               = iConfig.getUntrackedParameter<std::string>("Associator");
  associator_                   = 0;

  // GutSoftHistogramFactory
  histograms_                   = edm::Service<GutSoftHistogramFileService>()->getFactory();

}


GutSoftTrackingEfficiencyAnalyzer::~GutSoftTrackingEfficiencyAnalyzer()
{
 
}


void
GutSoftTrackingEfficiencyAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  // set baseDirectory in GutSoftHistogramFactory
  histograms_->setBaseDirectory(baseDirectoryName_);

  // get tracking particle collection for true information
  edm::Handle<TrackingParticleCollection>  trackingParticleCollectionHandle;
  iEvent.getByLabel(trackingTruthInputTag_,trackingParticleCollectionHandle);
  const TrackingParticleCollection *trackingParticleCollection = trackingParticleCollectionHandle.product();

  // get track collection
  const reco::TrackCollection *trackCollection = 0;
  edm::Handle<reco::TrackCollection> trackCollectionHandle;
  try {
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

  // association maps
  reco::RecoToSimCollection recoToSimCollection = associator_->associateRecoToSim(trackCollectionHandle,
										  trackingParticleCollectionHandle,
										  &iEvent);

  reco::SimToRecoCollection simToRecoCollection = associator_->associateSimToReco(trackCollectionHandle,
										  trackingParticleCollectionHandle,
										  &iEvent);

  // fill reconstructed histograms
  for ( reco::TrackCollection::size_type trackNumber = 0;
	trackNumber < trackCollection->size();
	++trackNumber ) {
    reco::TrackRef trackRef(trackCollectionHandle, trackNumber);
    std::vector<std::pair<TrackingParticleRef, double> > associatedTrackingParticles;

    if ( trackSelectorForEfficiencies_(*trackRef) ) {
      histograms_->fill("eff_eta_rec",trackRef->momentum().eta());
      histograms_->fill("eff_pt_rec",trackRef->pt());

      try {
	associatedTrackingParticles = recoToSimCollection[trackRef];
      } catch (cms::Exception e) {
      }

      if (associatedTrackingParticles.size()!=0) {
	// ATTENTION: take only first match
	TrackingParticleRef trackingParticleRef = associatedTrackingParticles.begin()->first;
	if ( trackingParticleSelectorForEfficiencies_( *trackingParticleRef ) ) {
	  histograms_->fill("eff_eta_recNtrue",trackingParticleRef->momentum().eta());
	  histograms_->fill("eff_pt_recNtrue",std::sqrt(trackingParticleRef->momentum().perp2()));
	}
      }
    }

    if ( trackSelectorForPurity_(*trackRef) ) {
      histograms_->fill("pur_eta_rec",trackRef->momentum().eta());
      histograms_->fill("pur_pt_rec",trackRef->pt());

      try {
	associatedTrackingParticles = recoToSimCollection[trackRef];
      } catch (cms::Exception e) {
      }

      if (associatedTrackingParticles.size()!=0) {
	// ATTENTION: take only first match
	TrackingParticleRef trackingParticleRef = associatedTrackingParticles.begin()->first;
	if ( trackingParticleSelectorForPurity_( *trackingParticleRef ) ) {
	  histograms_->fill("pur_eta_recNtrue",trackingParticleRef->momentum().eta());
	  histograms_->fill("pur_pt_recNtrue",std::sqrt(trackingParticleRef->momentum().perp2()));
	}
      }
    }
  }

  // fill true histograms
  for ( TrackingParticleCollection::size_type trackingParticleNumber = 0;
	trackingParticleNumber < trackingParticleCollection->size();
	++trackingParticleNumber ) {
    TrackingParticleRef trackingParticleRef(trackingParticleCollectionHandle, trackingParticleNumber);
    std::vector<std::pair<reco::TrackRef, double> > associatedTracks;

    if ( trackingParticleSelectorForEfficiencies_(*trackingParticleRef) ) {
      histograms_->fill("eff_eta_true",trackingParticleRef->momentum().eta());
      histograms_->fill("eff_pt_true",std::sqrt(trackingParticleRef->momentum().perp2()));

      try {
	associatedTracks = simToRecoCollection[trackingParticleRef];
      } catch (cms::Exception e) {
      }

      if (associatedTracks.size()!=0) {
	// ATTENTION: take only first match
	reco::TrackRef trackRef = associatedTracks.begin()->first;
	if ( trackSelectorForEfficiencies_( *trackRef ) ) {
	  histograms_->fill("eff_eta_trueNrec",trackRef->momentum().eta());
	  histograms_->fill("eff_pt_trueNrec",trackRef->pt());
	}
      }
    }

    if ( trackingParticleSelectorForPurity_(*trackingParticleRef) ) {
      histograms_->fill("pur_eta_true",trackingParticleRef->momentum().eta());
      histograms_->fill("pur_pt_true",std::sqrt(trackingParticleRef->momentum().perp2()));

      try {
	associatedTracks = simToRecoCollection[trackingParticleRef];
      } catch (cms::Exception e) {
      }

      if (associatedTracks.size()!=0) {
	// ATTENTION: take only first match
	reco::TrackRef trackRef = associatedTracks.begin()->first;
	if ( trackSelectorForPurity_( *trackRef ) ) {
	  histograms_->fill("pur_eta_trueNrec",trackRef->momentum().eta());
	  histograms_->fill("pur_pt_trueNrec",trackRef->pt());
	}
      }
    }
  }
}


void 
GutSoftTrackingEfficiencyAnalyzer::beginJob(const edm::EventSetup& setup)
{

  // binning for histograms
  std::string  directory = baseDirectoryName_;

  unsigned int pt_nbins = 160;
  double       pt_low   = 0.;
  double       pt_high  = 20.;

  unsigned int eta_nbins = 30;
  double       eta_low   = -3.;
  double       eta_high  =  3.;

  histograms_->bookHistogram("eff_pt_true","Track p_{T} (GeV)",
			     directory,pt_nbins,pt_low,pt_high,
			     "p_{T} [GeV]","Events");
  histograms_->bookHistogram("eff_eta_true","Track #eta",
			     directory,eta_nbins,eta_low,eta_high,
			     "#eta","Events");
  histograms_->bookHistogram("eff_pt_rec","Track p_{T} (GeV)",
			     directory,pt_nbins,pt_low,pt_high,
			     "p_{T} [GeV]","Events");
  histograms_->bookHistogram("eff_eta_rec","Track #eta",
			     directory,eta_nbins,eta_low,eta_high,
			     "#eta","Events");
  histograms_->bookHistogram("eff_pt_trueNrec","Track p_{T} (GeV)",
			     directory,pt_nbins,pt_low,pt_high,
			     "p_{T} [GeV]","Events");
  histograms_->bookHistogram("eff_eta_trueNrec","Track #eta",
			     directory,eta_nbins,eta_low,eta_high,
			     "#eta","Events");
  histograms_->bookHistogram("eff_pt_recNtrue","Track p_{T} (GeV)",
			     directory,pt_nbins,pt_low,pt_high,
			     "p_{T} [GeV]","Events");
  histograms_->bookHistogram("eff_eta_recNtrue","Track #eta",
			     directory,eta_nbins,eta_low,eta_high,
			     "#eta","Events");

  histograms_->bookHistogram("pur_pt_true","Track p_{T} (GeV)",
			     directory,pt_nbins,pt_low,pt_high,
			     "p_{T} [GeV]","Events");
  histograms_->bookHistogram("pur_eta_true","Track #eta",
			     directory,eta_nbins,eta_low,eta_high,
			     "#eta","Events");
  histograms_->bookHistogram("pur_pt_rec","Track p_{T} (GeV)",
			     directory,pt_nbins,pt_low,pt_high,
			     "p_{T} [GeV]","Events");
  histograms_->bookHistogram("pur_eta_rec","Track #eta",
			     directory,eta_nbins,eta_low,eta_high,
			     "#eta","Events");
  histograms_->bookHistogram("pur_pt_trueNrec","Track p_{T} (GeV)",
			     directory,pt_nbins,pt_low,pt_high,
			     "p_{T} [GeV]","Events");
  histograms_->bookHistogram("pur_eta_trueNrec","Track #eta",
			     directory,eta_nbins,eta_low,eta_high,
			     "#eta","Events");
  histograms_->bookHistogram("pur_pt_recNtrue","Track p_{T} (GeV)",
			     directory,pt_nbins,pt_low,pt_high,
			     "p_{T} [GeV]","Events");
  histograms_->bookHistogram("pur_eta_recNtrue","Track #eta",
			     directory,eta_nbins,eta_low,eta_high,
			     "#eta","Events");

  // get associator
  edm::ESHandle<TrackAssociatorBase> associatorHandle;
  setup.get<TrackAssociatorRecord>().get(associatorName_,associatorHandle);
  associator_ = associatorHandle.product();

}

void 
GutSoftTrackingEfficiencyAnalyzer::endJob() {

}
