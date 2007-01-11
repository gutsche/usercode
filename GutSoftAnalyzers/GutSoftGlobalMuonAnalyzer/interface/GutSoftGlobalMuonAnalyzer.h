#ifndef GutSoftGlobalMuonAnalyzer_h
#define GutSoftGlobalMuonAnalyzer_h

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

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "GutSoftTools/GutSoftHistogramFactory/interface/GutSoftHistogramFactory.h"

#include "FWCore/ParameterSet/interface/InputTag.h"

class GutSoftGlobalMuonAnalyzer : public edm::EDAnalyzer {
 public:
  explicit GutSoftGlobalMuonAnalyzer(const edm::ParameterSet&);
  ~GutSoftGlobalMuonAnalyzer();

 private:
  virtual void beginJob(const edm::EventSetup& setup) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  GutSoftHistogramFactory *histograms_;

  edm::InputTag muonInputTag_;

  std::string baseDirectoryName_;

};

#endif
