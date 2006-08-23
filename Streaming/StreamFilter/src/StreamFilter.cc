//
// Package:         Streaming/StreamFilter
// Class:           StreamFilter
// 
// Description:     Filter on streaming decision
//                  valid modes are: fnal, cnaf, ral, in2p3, asgc, fzk, pic
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Tue Aug 22 21:33:57 UTC 2006
//
// $Author: gutsche $
// $Date: 2006/03/28 23:12:10 $
// $Revision: 1.1 $
//

#include <exception>

#include "Streaming/StreamFilter/interface/StreamFilter.h"

#include "FWCore/Framework/interface/Handle.h"

#include "DataFormats/StreamDecision/interface/StreamDecision.h"
#include "DataFormats/StreamDecision/interface/StreamDecisionCollection.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

namespace cms
{
  StreamFilter::StreamFilter(const edm::ParameterSet& iConfig)
  {
    mode_ = iConfig.getParameter<std::string>("Mode");
    src_  = iConfig.getParameter<std::string>("DecisionMaker");

    if ( (mode_ != "fnal") &&
	 (mode_ != "cnaf") &&
	 (mode_ != "ral") &&
	 (mode_ != "in2p3") &&
	 (mode_ != "asgc") &&
	 (mode_ != "fzk") &&
	 (mode_ != "pic") ) {
      edm::LogError("Streaming") << "Unsupported mode: " << mode_ << ". Please use fnal, cnaf, ral, in2p3, asgc, fzk, pic";
      throw std::exception();
    }


  }

  StreamFilter::~StreamFilter()
  {

  }

  bool
  StreamFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
  {
    bool filter_decision = false;

    edm::Handle<StreamDecisionCollection> decisions;
    iEvent.getByLabel(src_,decisions);

    // take first decisioc collection entry
    const StreamDecision decision = *(decisions.product()->begin());

    if ( mode_ == "fnal" ) {
      if ( decision.GetFNAL() ) {
	filter_decision = true;
      }
    } else if ( mode_ == "cnaf" ) {
      if ( decision.GetCNAF() ) {
	filter_decision = true;
      }
    } else if ( mode_ == "ral" ) {
      if ( decision.GetRAL() ) {
	filter_decision = true;
      }
    } else if ( mode_ == "in2p3" ) {
      if ( decision.GetIN2P3() ) {
	filter_decision = true;
      }
    } else if ( mode_ == "asgc" ) {
      if ( decision.GetASGC() ) {
	filter_decision = true;
      }
    } else if ( mode_ == "fzk" ) {
      if ( decision.GetFZK() ) {
	filter_decision = true;
      }
    } else if ( mode_ == "pic" ) {
      if ( decision.GetPIC() ) {
	filter_decision = true;
      }
    }    

    if ( filter_decision) {
      LogDebug("Streaming") << "Filter in mode: " << mode_ << " decided true.";
    }
    return filter_decision;
  }

  void 
  StreamFilter::beginJob(const edm::EventSetup&)
  {
  }

  void 
  StreamFilter::endJob() {
  }
}
