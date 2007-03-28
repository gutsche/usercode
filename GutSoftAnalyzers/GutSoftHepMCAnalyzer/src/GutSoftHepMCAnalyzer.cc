//
// Package:         GutSoftAnalyzers/GutSoftHepMCAnalyzer
// Class:           GutSoftHepMCAnalyzer
// 
// Description:     EDAnalyzer for HepMC information
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Thu Feb 15 21:09:04 UTC 2007
//
// $Author: gutsche $
// $Date: 2007/02/16 00:46:11 $
// $Revision: 1.1 $
//

#include <string>
#include <cmath>

#include "GutSoftAnalyzers/GutSoftHepMCAnalyzer/interface/GutSoftHepMCAnalyzer.h"

#include "GutSoftTools/GutSoftHistogramFileService/interface/GutSoftHistogramFileService.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"

GutSoftHepMCAnalyzer::GutSoftHepMCAnalyzer(const edm::ParameterSet& iConfig)
{

  // input tags
  hepMCInputTag_                = iConfig.getUntrackedParameter<edm::InputTag>("HepMCInputTag");

  // base directory for histograms
  baseDirectoryName_            = iConfig.getUntrackedParameter<std::string>("BaseDirectoryName");

  // GutSoftHistogramFactory
  histograms_                   = edm::Service<GutSoftHistogramFileService>()->getFactory();

  minpt_     = iConfig.getUntrackedParameter<double>("MinPt", 0.);
  maxpt_     = iConfig.getUntrackedParameter<double>("MaxPt", 10000.);
  mineta_    = iConfig.getUntrackedParameter<double>("MinEta", -10.);
  maxeta_    = iConfig.getUntrackedParameter<double>("MaxEta", 10.);
  minphi_    = iConfig.getUntrackedParameter<double>("MinPhi", -3.5);
  maxphi_    = iConfig.getUntrackedParameter<double>("MaxPhi", 3.5);
  status_    = iConfig.getUntrackedParameter<int>("Status", 0);
  processID_ = iConfig.getUntrackedParameter<int>("ProcessID", 0);

}


GutSoftHepMCAnalyzer::~GutSoftHepMCAnalyzer()
{
 
}


void
GutSoftHepMCAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  // set baseDirectory in GutSoftHistogramFactory
  histograms_->setBaseDirectory(baseDirectoryName_);

  // get HepMC product
  edm::Handle<edm::HepMCProduct> hepMCProductHandle;
  iEvent.getByLabel(hepMCInputTag_, hepMCProductHandle);
  const HepMC::GenEvent *genEvent = hepMCProductHandle->GetEvent();
    
  if(processID_ == 0 || processID_ == genEvent->signal_process_id()) {
    
    for ( HepMC::GenEvent::particle_const_iterator p = genEvent->particles_begin(),
	    pEnd = genEvent->particles_end();
	  p != pEnd; 
	  ++p ) {

      if ( (*p)->momentum().perp() >= minpt_ 
	   && (*p)->momentum().perp() <= maxpt_
	   && (*p)->momentum().eta() >= mineta_
	   && (*p)->momentum().eta() <= maxeta_ 
	   && (*p)->momentum().phi() >= minphi_
	   && (*p)->momentum().phi() <= maxphi_ 
	   && (*p)->status() == status_ ) {
	histograms_->fill("eta",(*p)->momentum().eta());
      }
    }
  }
}

void 
GutSoftHepMCAnalyzer::beginJob(const edm::EventSetup& setup)
{

  // binning for histograms
  std::string  directory = baseDirectoryName_;

  unsigned int eta_nbins = 60;
  double       eta_low   = -3.;
  double       eta_high  =  3.;

  histograms_->bookHistogram("eta","HepMC #eta",
			     directory,eta_nbins,eta_low,eta_high,
			     "#eta","Events");
}

void 
GutSoftHepMCAnalyzer::endJob() {

}
