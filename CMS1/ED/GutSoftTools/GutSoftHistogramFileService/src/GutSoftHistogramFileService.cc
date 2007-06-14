//
// Package:         GutSoftTools/GutSoftHistogramFileService
// Class:           GutSoftHistogramFileService
// 
// Description:     service to provide HistogramFactory
//                  for all modules
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Tue Nov 28 00:08:15 UTC 2006
//
// $Author: gutsche $
// $Date: 2007/01/09 21:45:23 $
// $Revision: 1.2 $
//

#include "GutSoftTools/GutSoftHistogramFileService/interface/GutSoftHistogramFileService.h"

#include "FWCore/ServiceRegistry/interface/ServiceMaker.h"

GutSoftHistogramFileService::GutSoftHistogramFileService(const edm::ParameterSet& iConfig,
							 const edm::ActivityRegistry& iActvity)
{

  // get OutputFileName parameter
  outputFileName_ = iConfig.getUntrackedParameter<std::string>("OutputFileName");

  // create GutSoftHistogramFactory
  factory_ = new GutSoftHistogramFactory(outputFileName_);

}


GutSoftHistogramFileService::~GutSoftHistogramFileService()
{
 
  if ( factory_ ) {
    delete factory_;
    factory_ = 0;
  }

}


//define this as a plug-in
DEFINE_FWK_SERVICE(GutSoftHistogramFileService);
