//
// Package:         GutSoftAnalyzers/GutSoftTrackerRecHitAnalyzer
// Class:           GutSoftTrackerRecHitAnalyzer
// 
// Description:     EDAnalyzer for TrackerRecHit information
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Thu Feb 15 21:09:04 UTC 2007
//
// $Author: gutsche $
// $Date: 2007/05/25 00:02:59 $
// $Revision: 1.6 $
//

#include <string>
#include <cmath>
#include <sstream>
#include <algorithm>

#include "GutSoftAnalyzers/GutSoftTrackerRecHitAnalyzer/interface/GutSoftTrackerRecHitAnalyzer.h"

#include "GutSoftTools/GutSoftHistogramFileService/interface/GutSoftHistogramFileService.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/TrackerRecHit2D/interface/SiStripMatchedRecHit2DCollection.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit2DCollection.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHitCollection.h"

GutSoftTrackerRecHitAnalyzer::GutSoftTrackerRecHitAnalyzer(const edm::ParameterSet& iConfig)
{

  // input tags
  matchedStripRecHitsInputTag_ = iConfig.getParameter<edm::InputTag>("MatchedStripRecHitInputTag");
  rphiStripRecHitsInputTag_    = iConfig.getParameter<edm::InputTag>("RphiStripRecHitInputTag");
  stereoStripRecHitsInputTag_  = iConfig.getParameter<edm::InputTag>("StereoStripRecHitInputTag");
  pixelRecHitsInputTag_        = iConfig.getParameter<edm::InputTag>("PixelRecHitInputTag");

  // base directory for histograms
  baseDirectoryName_            = iConfig.getUntrackedParameter<std::string>("BaseDirectoryName");

  // GutSoftHistogramFactory
  histograms_                   = edm::Service<GutSoftHistogramFileService>()->getFactory();

}


GutSoftTrackerRecHitAnalyzer::~GutSoftTrackerRecHitAnalyzer()
{
 
}


void
GutSoftTrackerRecHitAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  // set baseDirectory in GutSoftHistogramFactory
  histograms_->setBaseDirectory(baseDirectoryName_);

  // get Inputs
  edm::Handle<SiStripRecHit2DCollection> rphirecHitHandle;
  iEvent.getByLabel(rphiStripRecHitsInputTag_ ,rphirecHitHandle);
  const SiStripRecHit2DCollection *rphiRecHitCollection = rphirecHitHandle.product();
  edm::Handle<SiStripRecHit2DCollection> stereorecHitHandle;
  iEvent.getByLabel(stereoStripRecHitsInputTag_ ,stereorecHitHandle);
  const SiStripRecHit2DCollection *stereoRecHitCollection = stereorecHitHandle.product();
  edm::Handle<SiStripMatchedRecHit2DCollection> matchedrecHitHandle;
  iEvent.getByLabel(matchedStripRecHitsInputTag_, matchedrecHitHandle);
  const SiStripMatchedRecHit2DCollection *matchedRecHitCollection = matchedrecHitHandle.product();
  edm::Handle<SiPixelRecHitCollection> pixelrecHitHandle;
  iEvent.getByLabel(pixelRecHitsInputTag_, pixelrecHitHandle);
  const SiPixelRecHitCollection *pixelRecHitCollection = pixelrecHitHandle.product();

  histograms_->fill("nPixelRecHits",pixelRecHitCollection->size());
  histograms_->fill("nRphiStripRecHits",rphiRecHitCollection->size());
  histograms_->fill("nStereoStripRecHits",stereoRecHitCollection->size());
  histograms_->fill("nMatchedStripRecHits",matchedRecHitCollection->size());
  
  edm::LogInfo("GutSoftTrackerRecHitAnalyzer") << "Number of Pixel RecHits: " << pixelRecHitCollection->size();
  edm::LogInfo("GutSoftTrackerRecHitAnalyzer") << "Number of Rphi Strip RecHits: " << rphiRecHitCollection->size();
  edm::LogInfo("GutSoftTrackerRecHitAnalyzer") << "Number of Stereo Strip RecHits: " << stereoRecHitCollection->size();
  edm::LogInfo("GutSoftTrackerRecHitAnalyzer") << "Number of Matched Strip RecHits: " << matchedRecHitCollection->size();

}

void 
GutSoftTrackerRecHitAnalyzer::beginJob(const edm::EventSetup& es)
{

  // binning for histograms
  std::string  directory = baseDirectoryName_;

  unsigned int n_nbins = 100000;
  double       n_low   = 0.;
  double       n_high  = 100000.;

  histograms_->bookHistogram("nPixelRecHits","Number of Pixel RecHits",
			     directory,n_nbins,n_low,n_high,
			     "n_{PixelRecHits}","Events");
  histograms_->bookHistogram("nRphiStripRecHits","Number of rphi Strip RecHits",
			     directory,n_nbins,n_low,n_high,
			     "n_{RphiStripRecHits}","Events");
  histograms_->bookHistogram("nStereoStripRecHits","Number of stereo Strip RecHits",
			     directory,n_nbins,n_low,n_high,
			     "n_{StereoStripRecHits}","Events");
  histograms_->bookHistogram("nMatchedStripRecHits","Number of matched Strip RecHits",
			     directory,n_nbins,n_low,n_high,
			     "n_{MatchedStripRecHits}","Events");

}

void 
GutSoftTrackerRecHitAnalyzer::endJob() {

}
