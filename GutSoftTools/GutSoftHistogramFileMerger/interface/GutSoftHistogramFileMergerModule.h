#ifndef GutSoftHistogramFileMergerModule_h
#define GutSoftHistogramFileMergerModule_h

//
// Package:         GutSoftTools/GutSoftHistogramFileMerger
// Class:           GutSoftHistogramFileMergerModule
// 
// Description:     EDAnalyzer to merge histogram root files
//                  in endjob routine
//                  to be scheduled after histogram root file producers
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Wed Oct 25 20:48:20 UTC 2006
//
// $Author: gutsche $
// $Date: 2006/10/25 22:01:37 $
// $Revision: 1.2 $
//

#include <string>
#include <vector>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class GutSoftHistogramFileMergerModule : public edm::EDAnalyzer {
 public:
  explicit GutSoftHistogramFileMergerModule(const edm::ParameterSet&);
  ~GutSoftHistogramFileMergerModule();

 private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  std::string outputFileName_;
  std::vector<std::string> inputFileNames_;
  bool deleteInputFiles_;

};

#endif
