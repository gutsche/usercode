#ifndef StreamDecider_h
#define StreamDecider_h

//
// Package:         Streaming/StreamDecider
// Class:           StreamDecider
// 
// Description:     Decide on streaming
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Tue Aug 22 21:33:57 UTC 2006
//
// $Author: gutsche $
// $Date: 2006/03/28 23:12:10 $
// $Revision: 1.1 $
//

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Handle.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "DataFormats/Common/interface/EDProduct.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

namespace cms
{
  class StreamDecider : public edm::EDProducer
  {
  public:

    explicit StreamDecider(const edm::ParameterSet& conf);

    virtual ~StreamDecider();

    virtual void produce(edm::Event& e, const edm::EventSetup& c);

    virtual unsigned int GetDecimalPlaces(double number);

  private:
    unsigned int count_;
    unsigned int count_fnal_;
    unsigned int count_cnaf_;
    unsigned int count_ral_;
    unsigned int count_in2p3_;
    unsigned int count_asgc_;
    unsigned int count_fzk_; 
    unsigned int count_pic_;

    unsigned int step_width_;
    unsigned int fnal_step_width_;
    unsigned int cnaf_step_width_;
    unsigned int ral_step_width_;
    unsigned int in2p3_step_width_;
    unsigned int asgc_step_width_;
    unsigned int fzk_step_width_;
    unsigned int pic_step_width_;

  
  };
}


#endif
