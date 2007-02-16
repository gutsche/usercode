#ifndef GutSoftHepMCAnalyzer_h
#define GutSoftHepMCAnalyzer_h

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
// $Date: 2007/01/11 01:51:04 $
// $Revision: 1.1 $
//

#include <string>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include "GutSoftTools/GutSoftHistogramFactory/interface/GutSoftHistogramFactory.h"


class GutSoftHepMCAnalyzer : public edm::EDAnalyzer {
 public:
  explicit GutSoftHepMCAnalyzer(const edm::ParameterSet&);
  ~GutSoftHepMCAnalyzer();

 private:
  virtual void beginJob(const edm::EventSetup& setup) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  GutSoftHistogramFactory *histograms_;

  edm::InputTag hepMCInputTag_;

  std::string baseDirectoryName_;

  double minpt_;
  double maxpt_;
  double mineta_;
  double maxeta_;
  double minphi_;
  double maxphi_;
  int status_; 
  int processID_;    

};

#endif
