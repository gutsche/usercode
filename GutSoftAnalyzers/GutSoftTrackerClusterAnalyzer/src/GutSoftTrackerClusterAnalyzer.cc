//
// Package:         GutSoftAnalyzers/GutSoftTrackerClusterAnalyzer
// Class:           GutSoftTrackerClusterAnalyzer
// 
// Description:     EDAnalyzer for TrackerCluster information
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

#include "GutSoftAnalyzers/GutSoftTrackerClusterAnalyzer/interface/GutSoftTrackerClusterAnalyzer.h"

#include "GutSoftTools/GutSoftHistogramFileService/interface/GutSoftHistogramFileService.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/SiPixelCluster/interface/SiPixelCluster.h"
#include "DataFormats/SiStripCluster/interface/SiStripCluster.h"

GutSoftTrackerClusterAnalyzer::GutSoftTrackerClusterAnalyzer(const edm::ParameterSet& iConfig)
{

  // input tags
  pixelClusterInputTag_        = iConfig.getUntrackedParameter<edm::InputTag>("PixelClusterInputTag");
  stripClusterInputTag_        = iConfig.getUntrackedParameter<edm::InputTag>("StripClusterInputTag");

  // base directory for histograms
  baseDirectoryName_            = iConfig.getUntrackedParameter<std::string>("BaseDirectoryName");

  // GutSoftHistogramFactory
  histograms_                   = edm::Service<GutSoftHistogramFileService>()->getFactory();

}


GutSoftTrackerClusterAnalyzer::~GutSoftTrackerClusterAnalyzer()
{
 
}


void
GutSoftTrackerClusterAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  // set baseDirectory in GutSoftHistogramFactory
  histograms_->setBaseDirectory(baseDirectoryName_);

  // get pixel cluster collection
  edm::Handle< edm::DetSetVector<SiPixelCluster> > pixelClusterHandle;
  iEvent.getByLabel( pixelClusterInputTag_, pixelClusterHandle);
  const edm::DetSetVector<SiPixelCluster>* pixelClusterCollection = pixelClusterHandle.product();

  // get strip cluster collection
  edm::Handle< edm::DetSetVector<SiStripCluster> > stripClusterHandle;
  iEvent.getByLabel( stripClusterInputTag_, stripClusterHandle);
  const edm::DetSetVector<SiStripCluster>* stripClusterCollection = stripClusterHandle.product();

  // determine number of pixelClusters
  unsigned int pixelCounter = 0;
  for ( edm::DetSetVector<SiPixelCluster>::const_iterator pixelDetSetVectorEntry = pixelClusterCollection->begin(),
	  pixelDetSetVectorEntryEnd = pixelClusterCollection->end();
	pixelDetSetVectorEntry != pixelDetSetVectorEntryEnd;
	++pixelDetSetVectorEntry ) {
    pixelCounter += pixelDetSetVectorEntry->size();
  }

  // determine number of stripClusters
  unsigned int stripCounter = 0;
  for ( edm::DetSetVector<SiStripCluster>::const_iterator stripDetSetVectorEntry = stripClusterCollection->begin(),
	  stripDetSetVectorEntryEnd = stripClusterCollection->end();
	stripDetSetVectorEntry != stripDetSetVectorEntryEnd;
	++stripDetSetVectorEntry ) {
    stripCounter += stripDetSetVectorEntry->size();
  }

  histograms_->fill("nPixelClusters",pixelCounter);
  histograms_->fill("nStripClusters",stripCounter);
  
  edm::LogInfo("GutSoftTrackerClusterAnalyzer") << "Number of Pixel Clusters: " << pixelCounter;
  edm::LogInfo("GutSoftTrackerClusterAnalyzer") << "Number of Strip Clusters: " << stripCounter;

}

void 
GutSoftTrackerClusterAnalyzer::beginJob(const edm::EventSetup& es)
{

  // binning for histograms
  std::string  directory = baseDirectoryName_;

  unsigned int n_nbins = 100000;
  double       n_low   = 0.;
  double       n_high  = 100000.;

  histograms_->bookHistogram("nPixelClusters","Number of Pixel Clusters",
			     directory,n_nbins,n_low,n_high,
			     "n_{PixelClusters}","Events");

  histograms_->bookHistogram("nStripClusters","Number of Strip Clusters",
			     directory,n_nbins,n_low,n_high,
			     "n_{StripClusters}","Events");
}

void 
GutSoftTrackerClusterAnalyzer::endJob() {

}
