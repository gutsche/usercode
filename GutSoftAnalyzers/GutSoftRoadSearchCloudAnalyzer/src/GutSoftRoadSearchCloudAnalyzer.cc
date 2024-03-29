//
// Package:         GutSoftAnalyzers/GutSoftRoadSearchCloudAnalyzer
// Class:           GutSoftRoadSearchCloudAnalyzer
// 
// Description:     EDAnalyzer of RoadSearchClouds
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Tue Oct 17 02:41:12 UTC 2006
//
// $Author: gutsche $
// $Date: 2007/03/28 20:16:15 $
// $Revision: 1.8 $
//

#include <string>

#include "GutSoftAnalyzers/GutSoftRoadSearchCloudAnalyzer/interface/GutSoftRoadSearchCloudAnalyzer.h"

#include "GutSoftTools/GutSoftHistogramFileService/interface/GutSoftHistogramFileService.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/RoadSearchCloud/interface/RoadSearchCloud.h"
#include "DataFormats/RoadSearchCloud/interface/RoadSearchCloudCollection.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/SiStripDetId/interface/StripSubdetector.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"

#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"

#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"

#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"

GutSoftRoadSearchCloudAnalyzer::GutSoftRoadSearchCloudAnalyzer(const edm::ParameterSet& iConfig)
{

  roadSearchCloudInputTag_      = iConfig.getUntrackedParameter<edm::InputTag>("RoadSearchCloudInputTag");
  baseDirectoryName_            = iConfig.getUntrackedParameter<std::string>("BaseDirectoryName");

  // GutSoftHistogramFactory
  histograms_ = edm::Service<GutSoftHistogramFileService>()->getFactory();

}


GutSoftRoadSearchCloudAnalyzer::~GutSoftRoadSearchCloudAnalyzer()
{
 
}


void
GutSoftRoadSearchCloudAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  // set baseDirectory in GutSoftHistogramFactory
  histograms_->setBaseDirectory(baseDirectoryName_);

  const RoadSearchCloudCollection *roadSearchCloudCollection = 0;
  try {
    edm::Handle<RoadSearchCloudCollection> roadSearchCloudCollectionHandle;
    iEvent.getByLabel(roadSearchCloudInputTag_,roadSearchCloudCollectionHandle);
    roadSearchCloudCollection = roadSearchCloudCollectionHandle.product();
  }
  catch (edm::Exception const& x) {
    if ( x.categoryCode() == edm::errors::ProductNotFound ) {
      if ( x.history().size() == 1 ) {
	static const RoadSearchCloudCollection s_empty;
	roadSearchCloudCollection = &s_empty;
	edm::LogWarning("GutSoftRoadSearchCloudAnalyzer") << "Collection RoadSearchCloudCollection with label " << roadSearchCloudInputTag_ << " cannot be found, using empty collection of same type";
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
    
    for ( RoadSearchCloud::RecHitVector::const_iterator recHit = cloud->begin_hits();
	  recHit != cloud->end_hits();
	  ++recHit ) {
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
    
    // take eta of last cloud hit
    const TrackingRecHit *outerSeedHit = *(--cloud->end_hits());
    double eta = trackerGeometry_->idToDet(outerSeedHit->geographicalId())->surface().toGlobal(outerSeedHit->localPosition()).eta();

    histograms_->fill("nHitPerCloudVsEta",eta,nHit);
    histograms_->fill("nStripHitPerCloudVsEta",eta,nStripHit);
    histograms_->fill("nPixelHitPerCloudVsEta",eta,nPixelHit);
    histograms_->fill("nTIBHitPerCloudVsEta",eta,nTIBHit);
    histograms_->fill("nTOBHitPerCloudVsEta",eta,nTOBHit);
    histograms_->fill("nTIDHitPerCloudVsEta",eta,nTIDHit);
    histograms_->fill("nTECHitPerCloudVsEta",eta,nTECHit);
    histograms_->fill("nPXBHitPerCloudVsEta",eta,nPXBHit);
    histograms_->fill("nPXFHitPerCloudVsEta",eta,nPXFHit);

    histograms_->fill("nHits",cloud->size());

  }

}


void 
GutSoftRoadSearchCloudAnalyzer::beginJob(const edm::EventSetup& es)
{

  // get tracker geometry
  edm::ESHandle<TrackerGeometry> trackerHandle;
  es.get<TrackerDigiGeometryRecord>().get(trackerHandle);
  trackerGeometry_ = trackerHandle.product();

  std::string  nCloudDirectory = baseDirectoryName_;

  // binning for histograms
  unsigned int nCloud_nbins    = 100000;
  unsigned int nCloud_low      = 0;
  unsigned int nCloud_high     = 100000;

  unsigned int nhit_nbins      = 31;
  double       nhit_low        = -0.5;
  double       nhit_high       = 30.5;

  unsigned int eta_nbins       = 30;
  double       eta_low         = -3.;
  double       eta_high        =  3.;

  unsigned int nHits_nbins     = 1000;
  double       nHits_low       = 0.;
  double       nHits_high      = 1000.;


  // book histogram
  histograms_->bookHistogram("nCloud","Number of cloud per event",
			     nCloudDirectory,nCloud_nbins,nCloud_low,nCloud_high,
			     "n_{Cloud}","Events");
  histograms_->bookHistogram("nHitPerCloudVsEta","Number of hits per cloud vs. #eta",
			     nCloudDirectory,eta_nbins,eta_low,eta_high,nhit_nbins,nhit_low,nhit_high,
			     "#eta","n_{Hit}","Events");
  histograms_->bookHistogram("nStripHitPerCloudVsEta","Number of strip hits per cloud vs. #eta",
			     nCloudDirectory,eta_nbins,eta_low,eta_high,nhit_nbins,nhit_low,nhit_high,
			     "#eta","n_{Hit}","Events");
  histograms_->bookHistogram("nPixelHitPerCloudVsEta","Number of pixel hits per cloud vs. #eta",
			     nCloudDirectory,eta_nbins,eta_low,eta_high,nhit_nbins,nhit_low,nhit_high,
			     "#eta","n_{Hit}","Events");
  histograms_->bookHistogram("nTIBHitPerCloudVsEta","Number of TIB hits per cloud vs. #eta",
			     nCloudDirectory,eta_nbins,eta_low,eta_high,nhit_nbins,nhit_low,nhit_high,
			     "#eta","n_{Hit}","Events");
  histograms_->bookHistogram("nTOBHitPerCloudVsEta","Number of TOB hits per cloud vs. #eta",
			     nCloudDirectory,eta_nbins,eta_low,eta_high,nhit_nbins,nhit_low,nhit_high,
			     "#eta","n_{Hit}","Events");
  histograms_->bookHistogram("nTIDHitPerCloudVsEta","Number of TID hits per cloud vs. #eta",
			     nCloudDirectory,eta_nbins,eta_low,eta_high,nhit_nbins,nhit_low,nhit_high,
			     "#eta","n_{Hit}","Events");
  histograms_->bookHistogram("nTECHitPerCloudVsEta","Number of TEC hits per cloud vs. #eta",
			     nCloudDirectory,eta_nbins,eta_low,eta_high,nhit_nbins,nhit_low,nhit_high,
			     "#eta","n_{Hit}","Events");
  histograms_->bookHistogram("nPXBHitPerCloudVsEta","Number of PXB hits per cloud vs. #eta",
			     nCloudDirectory,eta_nbins,eta_low,eta_high,nhit_nbins,nhit_low,nhit_high,
			     "#eta","n_{Hit}","Events");
  histograms_->bookHistogram("nPXFHitPerCloudVsEta","Number of PXF hits per cloud vs. #eta",
			     nCloudDirectory,eta_nbins,eta_low,eta_high,nhit_nbins,nhit_low,nhit_high,
			     "#eta","n_{Hit}","Events");
  histograms_->bookHistogram("nHits","Number of hits per cloud",
			     nCloudDirectory,nHits_nbins,nHits_low,nHits_high,
			     "n_{Hits}","Events");
}

void 
GutSoftRoadSearchCloudAnalyzer::endJob() {

}
