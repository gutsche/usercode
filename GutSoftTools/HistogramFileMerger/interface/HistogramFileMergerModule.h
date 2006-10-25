#ifndef HistogramFileMergerModule_h
#define HistogramFileMergerModule_h

//
// Package:         GutSoftTools/HistogramFileMerger
// Class:           HistogramFileMergerModule
// 
// Description:     EDAnalyzer to merge histogram root files
//                  in endjob routine
//                  to be scheduled after histogram root file producers
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Wed Oct 25 20:48:20 UTC 2006
//
// $Author: gutsche $
// $Date: 2006/10/17 03:12:34 $
// $Revision: 1.1 $
//

#include <string>
#include <vector>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class HistogramFileMergerModule : public edm::EDAnalyzer {
 public:
  explicit HistogramFileMergerModule(const edm::ParameterSet&);
  ~HistogramFileMergerModule();

 private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  std::string outputFileName_;
  std::vector<std::string> inputFileNames_;

};

#endif
