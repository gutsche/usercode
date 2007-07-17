//
// Package:         GutSoftAnalyzers/GutSoftTrackerDigiAnalyzer
// Class:           GutSoftTrackerDigiAnalyzer
// 
// Description:     EDAnalyzer for TrackerDigi information
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

#include "GutSoftAnalyzers/GutSoftTrackerDigiAnalyzer/interface/GutSoftTrackerDigiAnalyzer.h"

#include "GutSoftTools/GutSoftHistogramFileService/interface/GutSoftHistogramFileService.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/SiPixelDigi/interface/PixelDigi.h"
#include "DataFormats/SiStripDigi/interface/SiStripDigi.h"

GutSoftTrackerDigiAnalyzer::GutSoftTrackerDigiAnalyzer(const edm::ParameterSet& iConfig)
{

  // input tags
  pixelDigiInputTag_        = iConfig.getUntrackedParameter<edm::InputTag>("PixelDigiInputTag");
  stripDigiInputTag_        = iConfig.getUntrackedParameter<edm::InputTag>("StripDigiInputTag");

  // base directory for histograms
  baseDirectoryName_            = iConfig.getUntrackedParameter<std::string>("BaseDirectoryName");

  // GutSoftHistogramFactory
  histograms_                   = edm::Service<GutSoftHistogramFileService>()->getFactory();

}


GutSoftTrackerDigiAnalyzer::~GutSoftTrackerDigiAnalyzer()
{
 
}


void
GutSoftTrackerDigiAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  // set baseDirectory in GutSoftHistogramFactory
  histograms_->setBaseDirectory(baseDirectoryName_);

  // get pixel digi collection
  edm::Handle< edm::DetSetVector<PixelDigi> > pixelDigiHandle;
  iEvent.getByLabel( pixelDigiInputTag_, pixelDigiHandle);
  const edm::DetSetVector<PixelDigi>* pixelDigiCollection = pixelDigiHandle.product();

  // get strip digi collection
  edm::Handle< edm::DetSetVector<SiStripDigi> > stripDigiHandle;
  iEvent.getByLabel( stripDigiInputTag_, stripDigiHandle);
  const edm::DetSetVector<SiStripDigi>* stripDigiCollection = stripDigiHandle.product();

  // determine number of pixelDigis
  unsigned int pixelCounter = 0;
  for ( edm::DetSetVector<PixelDigi>::const_iterator pixelDetSetVectorEntry = pixelDigiCollection->begin(),
	  pixelDetSetVectorEntryEnd = pixelDigiCollection->end();
	pixelDetSetVectorEntry != pixelDetSetVectorEntryEnd;
	++pixelDetSetVectorEntry ) {
    pixelCounter += pixelDetSetVectorEntry->size();
  }

  // determine number of stripDigis
  unsigned int stripCounter = 0;
  for ( edm::DetSetVector<SiStripDigi>::const_iterator stripDetSetVectorEntry = stripDigiCollection->begin(),
	  stripDetSetVectorEntryEnd = stripDigiCollection->end();
	stripDetSetVectorEntry != stripDetSetVectorEntryEnd;
	++stripDetSetVectorEntry ) {
    stripCounter += stripDetSetVectorEntry->size();
  }

  histograms_->fill("nPixelDigis",pixelCounter);
  histograms_->fill("nStripDigis",stripCounter);
  
  edm::LogInfo("GutSoftTrackerDigiAnalyzer") << "Number of Pixel Digis: " << pixelCounter;
  edm::LogInfo("GutSoftTrackerDigiAnalyzer") << "Number of Strip Digis: " << stripCounter;

}

void 
GutSoftTrackerDigiAnalyzer::beginJob(const edm::EventSetup& es)
{

  // binning for histograms
  std::string  directory = baseDirectoryName_;

  unsigned int n_nbins = 100000;
  double       n_low   = 0.;
  double       n_high  = 100000.;

  histograms_->bookHistogram("nPixelDigis","Number of Pixel Digis",
			     directory,n_nbins,n_low,n_high,
			     "n_{PixelDigis}","Events");

  histograms_->bookHistogram("nStripDigis","Number of Strip Digis",
			     directory,n_nbins,n_low,n_high,
			     "n_{StripDigis}","Events");
}

void 
GutSoftTrackerDigiAnalyzer::endJob() {

}
