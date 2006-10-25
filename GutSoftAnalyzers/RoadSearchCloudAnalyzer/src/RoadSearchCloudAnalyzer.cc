//
// Package:         GutSoftAnalyzers/RoadSearchCloudAnalyzer
// Class:           RoadSearchCloudAnalyzer
// 
// Description:     EDAnalyzer of RoadSearchClouds
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Tue Oct 17 02:41:12 UTC 2006
//
// $Author: gutsche $
// $Date: 2006/10/17 03:12:33 $
// $Revision: 1.1 $
//

#include <string>

#include "GutSoftAnalyzers/RoadSearchCloudAnalyzer/interface/RoadSearchCloudAnalyzer.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/RoadSearchCloud/interface/RoadSearchCloud.h"
#include "DataFormats/RoadSearchCloud/interface/RoadSearchCloudCollection.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/SiStripDetId/interface/StripSubdetector.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"

RoadSearchCloudAnalyzer::RoadSearchCloudAnalyzer(const edm::ParameterSet& iConfig)
{

  outputFileName_               = iConfig.getUntrackedParameter<std::string>("OutputFileName");
  roadSearchCloudProducerLabel_ = iConfig.getUntrackedParameter<std::string>("RoadSearchCloudProducerLabel");
  baseDirectoryName_            = iConfig.getUntrackedParameter<std::string>("BaseDirectoryName");

}


RoadSearchCloudAnalyzer::~RoadSearchCloudAnalyzer()
{
 
  if ( histograms_ ) {
    delete histograms_;
    histograms_ = 0;
  }
 
}


void
RoadSearchCloudAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  const RoadSearchCloudCollection *roadSearchCloudCollection = 0;
  try {
    edm::Handle<RoadSearchCloudCollection> roadSearchCloudCollectionHandle;
    iEvent.getByLabel(roadSearchCloudProducerLabel_,roadSearchCloudCollectionHandle);
    roadSearchCloudCollection = roadSearchCloudCollectionHandle.product();
  }
  catch (edm::Exception const& x) {
    if ( x.categoryCode() == edm::errors::ProductNotFound ) {
      if ( x.history().size() == 1 ) {
	roadSearchCloudCollection = new RoadSearchCloudCollection();
	edm::LogWarning("RoadSearchCloudAnalyzer") << "Collection RoadSearchCloudCollection with label " << roadSearchCloudProducerLabel_ << " cannot be found, using empty collection of same type";
      }
    }
  }

  // clouds
  histograms_->fill("nCloud",roadSearchCloudCollection->size());
  for ( RoadSearchCloudCollection::const_iterator cloud = roadSearchCloudCollection->begin();
	cloud != roadSearchCloudCollection->end();
	++cloud ) {

    // loop over hits in clouds
    unsigned int nHit      = 0;
    unsigned int nStripHit = 0;
    unsigned int nPixelHit = 0;
    unsigned int nTIBHit   = 0;
    unsigned int nTOBHit   = 0;
    unsigned int nTIDHit   = 0;
    unsigned int nTECHit   = 0;
    unsigned int nPXBHit   = 0;
    unsigned int nPXFHit   = 0;
    
    for ( RoadSearchCloud::RecHitOwnVector::const_iterator recHit = cloud->begin_hits();
	  recHit != cloud->end_hits();
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
    
    histograms_->fill("nHitPerCloudVsEta",nHit,0.);
    histograms_->fill("nStripHitPerCloudVsEta",nStripHit,0.);
    histograms_->fill("nPixelHitPerCloudVsEta",nPixelHit,0.);
    histograms_->fill("nTIBHitPerCloudVsEta",nTIBHit,0.);
    histograms_->fill("nTOBHitPerCloudVsEta",nTOBHit,0.);
    histograms_->fill("nTIDHitPerCloudVsEta",nTIDHit,0.);
    histograms_->fill("nTECHitPerCloudVsEta",nTECHit,0.);
    histograms_->fill("nPXBHitPerCloudVsEta",nPXBHit,0.);
    histograms_->fill("nPXFHitPerCloudVsEta",nPXFHit,0.);

  }

}


void 
RoadSearchCloudAnalyzer::beginJob(const edm::EventSetup&)
{

  // HistogramFactory
  histograms_ = new HistogramFactory(outputFileName_);

  // binning for histograms
  unsigned int nCloud_nbins    = 100000;
  unsigned int nCloud_low      = 0;
  unsigned int nCloud_high     = 100000;
  std::string  nCloudDirectory = baseDirectoryName_;

  unsigned int nhit_nbins      = 31;
  double       nhit_low        = -0.5;
  double       nhit_high       = 30.5;

  unsigned int eta_nbins       = 30;
  double       eta_low         = -3.;
  double       eta_high        =  3.;

  // book histogram
  histograms_->bookHistogram("nCloud","Number of cloud per event",
			     nCloudDirectory,nCloud_nbins,nCloud_low,nCloud_high,
			     "n_{Cloud}","Events");
  histograms_->bookHistogram("nHitPerCloudVsEta","Number of hits per cloud vs. #eta",
			     nCloudDirectory,nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,
			     "n_{Hit}","#eta","Events");
  histograms_->bookHistogram("nStripHitPerCloudVsEta","Number of strip hits per cloud vs. #eta",
			     nCloudDirectory,nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,
			     "n_{Hit}","#eta","Events");
  histograms_->bookHistogram("nPixelHitPerCloudVsEta","Number of pixel hits per cloud vs. #eta",
			     nCloudDirectory,nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,
			     "n_{Hit}","#eta","Events");
  histograms_->bookHistogram("nTIBHitPerCloudVsEta","Number of TIB hits per cloud vs. #eta",
			     nCloudDirectory,nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,
			     "n_{Hit}","#eta","Events");
  histograms_->bookHistogram("nTOBHitPerCloudVsEta","Number of TOB hits per cloud vs. #eta",
			     nCloudDirectory,nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,
			     "n_{Hit}","#eta","Events");
  histograms_->bookHistogram("nTIDHitPerCloudVsEta","Number of TID hits per cloud vs. #eta",
			     nCloudDirectory,nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,
			     "n_{Hit}","#eta","Events");
  histograms_->bookHistogram("nTECHitPerCloudVsEta","Number of TEC hits per cloud vs. #eta",
			     nCloudDirectory,nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,
			     "n_{Hit}","#eta","Events");
  histograms_->bookHistogram("nPXBHitPerCloudVsEta","Number of PXB hits per cloud vs. #eta",
			     nCloudDirectory,nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,
			     "n_{Hit}","#eta","Events");
  histograms_->bookHistogram("nPXFHitPerCloudVsEta","Number of PXF hits per cloud vs. #eta",
			     nCloudDirectory,nhit_nbins,nhit_low,nhit_high,eta_nbins,eta_low,eta_high,
			     "n_{Hit}","#eta","Events");

}

void 
RoadSearchCloudAnalyzer::endJob() {

  // delete HistogramFactory, histogram file is written out and can be handled in module endJob functions of the following modules
  if ( histograms_ ) {
    delete histograms_;
    histograms_ = 0;
  }

}
