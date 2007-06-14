#ifndef src_GutSoftHistogramFileService_h
#define src_GutSoftHistogramFileService_h

//
// Package:         GutSoftTools/GutSoftHistogramFileService
// Class:           GutSoftHistogramFileService
// 
// Description:     service to provide HistogramFactory
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Tue Nov 28 00:08:15 UTC 2006
//
// $Author: gutsche $
// $Date: 2006/11/28 20:34:48 $
// $Revision: 1.1 $
//

#include <memory>
#include <string>

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/ActivityRegistry.h"

#include "GutSoftTools/GutSoftHistogramFactory/interface/GutSoftHistogramFactory.h"

class GutSoftHistogramFileService {
 public:
  
  GutSoftHistogramFileService(const edm::ParameterSet&,
			      const edm::ActivityRegistry&);
  ~GutSoftHistogramFileService();
  
  inline GutSoftHistogramFactory* getFactory() { return factory_;}

 private:
  
  GutSoftHistogramFactory *factory_;

  std::string outputFileName_;

};

#endif

