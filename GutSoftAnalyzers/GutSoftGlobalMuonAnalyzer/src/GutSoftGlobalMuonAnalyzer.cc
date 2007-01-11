//
// Package:         GutSoftAnalyzers/GutSoftGlobalMuonAnalyzer
// Class:           GutSoftGlobalMuonAnalyzer
// 
// Description:     EDAnalyzer for global muons
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Thu Jan 11 01:09:09 UTC 2007
//
// $Author: gutsche $
// $Date: 2007/01/09 21:10:59 $
// $Revision: 1.2 $
//

#include <string>
#include <cmath>

#include "GutSoftAnalyzers/GutSoftGlobalMuonAnalyzer/interface/GutSoftGlobalMuonAnalyzer.h"

#include "GutSoftTools/GutSoftHistogramFileService/interface/GutSoftHistogramFileService.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"

GutSoftGlobalMuonAnalyzer::GutSoftGlobalMuonAnalyzer(const edm::ParameterSet& iConfig)
{

  // input tags
  muonInputTag_                = iConfig.getUntrackedParameter<edm::InputTag>("MuonInputTag");

  // base directory for histograms
  baseDirectoryName_            = iConfig.getUntrackedParameter<std::string>("BaseDirectoryName");

  // GutSoftHistogramFactory
  histograms_                   = edm::Service<GutSoftHistogramFileService>()->getFactory();

}


GutSoftGlobalMuonAnalyzer::~GutSoftGlobalMuonAnalyzer()
{
 
}


void
GutSoftGlobalMuonAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  // set baseDirectory in GutSoftHistogramFactory
  histograms_->setBaseDirectory(baseDirectoryName_);

  // get muon collection
  const reco::MuonCollection *muonCollection = 0;
  edm::Handle<reco::MuonCollection> muonCollectionHandle;
  try {
    iEvent.getByLabel(muonInputTag_,muonCollectionHandle);
    muonCollection = muonCollectionHandle.product();
  }
  catch (edm::Exception const& x) {
    if ( x.categoryCode() == edm::errors::ProductNotFound ) {
      if ( x.history().size() == 1 ) {
	static const reco::MuonCollection s_empty;
	muonCollection = &s_empty;
	edm::LogWarning("GutSoftGlobalMuonAnalyzer") << "Collection reco::MuonCollection with label " << muonInputTag_ << " cannot be found, using empty collection of same type";
      }
    }
  }

  for ( reco::MuonCollection::const_iterator muon = muonCollection->begin();
	muon != muonCollection->end();
	++muon ) {
    histograms_->fill("eta",muon->eta());
    histograms_->fill("pt",muon->pt());
  }

}


void 
GutSoftGlobalMuonAnalyzer::beginJob(const edm::EventSetup& setup)
{

  // binning for histograms
  std::string  directory = baseDirectoryName_;

  unsigned int pt_nbins = 400;
  double       pt_low   = 0.;
  double       pt_high  = 200.;

  unsigned int eta_nbins = 30;
  double       eta_low   = -3.;
  double       eta_high  =  3.;

  histograms_->bookHistogram("pt","Global Muon p_{T} (GeV)",
			     directory,pt_nbins,pt_low,pt_high,
			     "p_{T} [GeV]","Events");
  histograms_->bookHistogram("eta","Global Muon #eta",
			     directory,eta_nbins,eta_low,eta_high,
			     "#eta","Events");
}

void 
GutSoftGlobalMuonAnalyzer::endJob() {

}
