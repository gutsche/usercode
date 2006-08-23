//
// Package:         Streaming/StreamDecider
// Class:           StreamDecider
// 
// Description:     according to the fraction of events individual sites should receive, assign streaming decisions
//
//                  only fractions up to three decimal places (10.3 %)
//
//                  first calculate minimal integer representation of site fractions, use sum as step_width
//                  start looping over events, set stream decision event after event
//                  site 1 gets events till minimal integer representation of site is fulfilled
//                  followed by site 2, site 3
//                  restart with site 1 when step_width number of events have been processed
//                  
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Tue Aug 22 21:33:57 UTC 2006
//
// $Author: gutsche $
// $Date: 2006/03/28 23:12:10 $
// $Revision: 1.1 $
//

#include <exception>
#include <cmath>
#include <sstream>

#include "Streaming/StreamDecider/interface/StreamDecider.h"

#include "DataFormats/StreamDecision/interface/StreamDecisionCollection.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

namespace cms
{

  StreamDecider::StreamDecider(edm::ParameterSet const& conf) 
  {
    produces<StreamDecisionCollection>();

    // initialize
    count_       = 0;
    count_fnal_  = 0;
    count_cnaf_  = 0;
    count_ral_   = 0;
    count_in2p3_ = 0;
    count_asgc_  = 0;
    count_fzk_   = 0;
    count_pic_   = 0;
    double fnal_fraction  = conf.getParameter<double>("fnal");
    double cnaf_fraction  = conf.getParameter<double>("cnaf");
    double ral_fraction   = conf.getParameter<double>("ral");
    double in2p3_fraction = conf.getParameter<double>("in2p3");
    double asgc_fraction  = conf.getParameter<double>("asgc");
    double fzk_fraction   = conf.getParameter<double>("fzk");
    double pic_fraction   = conf.getParameter<double>("pic");

    // check if fractions add up to 1
    double sum = fnal_fraction + 
      cnaf_fraction + 
      ral_fraction + 
      in2p3_fraction + 
      asgc_fraction + 
      fzk_fraction +
      pic_fraction;
    if ( sum !=  1.0 ) {
      edm::LogError("Streaming") << "Fractions do not add up to 1.0, fnal: " << fnal_fraction
				 << " cnaf: " << cnaf_fraction
				 << " ral: " << ral_fraction
				 << " in2p3: " << in2p3_fraction
				 << " asgc: " << asgc_fraction
				 << " fzk: " << fzk_fraction
				 << " pic: " << pic_fraction
				 << " sum: " << sum;
      throw std::exception();
    }

    // determine step_width
    //
    // calculate minimum of integer representation from fractions
    // 
    unsigned int decimals = 0;

    // fnal
    unsigned int fnal_decimals = GetDecimalPlaces(fnal_fraction);
    if ( fnal_decimals > decimals ) decimals = fnal_decimals;

    // cnaf
    unsigned int cnaf_decimals = GetDecimalPlaces(cnaf_fraction);
    if ( cnaf_decimals > decimals ) decimals = cnaf_decimals;

    // ral
    unsigned int ral_decimals = GetDecimalPlaces(ral_fraction);
    if ( ral_decimals > decimals ) decimals = ral_decimals;

    // in2p3
    unsigned int in2p3_decimals = GetDecimalPlaces(in2p3_fraction);
    if ( in2p3_decimals > decimals ) decimals = in2p3_decimals;

    // asgc
    unsigned int asgc_decimals = GetDecimalPlaces(asgc_fraction);
    if ( asgc_decimals > decimals ) decimals = asgc_decimals;

    // fzk
    unsigned int fzk_decimals = GetDecimalPlaces(fzk_fraction);
    if ( fzk_decimals > decimals ) decimals = fzk_decimals;

    // pic
    unsigned int pic_decimals = GetDecimalPlaces(pic_fraction);
    if ( pic_decimals > decimals ) decimals = pic_decimals;

    // calculate
    fnal_step_width_ = int(fnal_fraction*std::pow(10.0,int(decimals)));
    cnaf_step_width_ = int(cnaf_fraction*std::pow(10.0,int(decimals)));
    ral_step_width_ = int(ral_fraction*std::pow(10.0,int(decimals)));
    in2p3_step_width_ = int(in2p3_fraction*std::pow(10.0,int(decimals)));
    asgc_step_width_ = int(asgc_fraction*std::pow(10.0,int(decimals)));
    fzk_step_width_ = int(fzk_fraction*std::pow(10.0,int(decimals)));
    pic_step_width_ = int(pic_fraction*std::pow(10.0,int(decimals)));

    step_width_ = fnal_step_width_ +
      cnaf_step_width_ +
      ral_step_width_ +
      in2p3_step_width_ +
      asgc_step_width_ +
      fzk_step_width_ +
      pic_step_width_;

    edm::LogInfo("Streaming") << "Streaming Configuration:\n"
			      << "FNAL : fraction: "  << fnal_fraction << " step width: " << fnal_step_width_ << "\n"
			      << "CNAF : fraction: "  << cnaf_fraction << " step width: " << cnaf_step_width_ << "\n"
			      << "RAL  : fraction: "  << ral_fraction << " step width: " << ral_step_width_ << "\n"
			      << "IN2P3: fraction: "  << in2p3_fraction << " step width: " << in2p3_step_width_ << "\n"
			      << "ASGC : fraction: "  << asgc_fraction << " step width: " << asgc_step_width_ << "\n"
			      << "FZK  : fraction: "  << fzk_fraction << " step width: " << fzk_step_width_ << "\n"
			      << "PIC  : fraction: "  << pic_fraction << " step width: " << pic_step_width_;
    


  }


  // Virtual destructor needed.
  StreamDecider::~StreamDecider() { }  

  // Functions that gets called by framework every event
  void StreamDecider::produce(edm::Event& e, const edm::EventSetup& es)
  {
    // create empty output collection
    std::auto_ptr<StreamDecisionCollection> output(new StreamDecisionCollection);

    // count events
    ++count_;

    // create StreamDecision objects and fill appropriate flag and increase counter
    StreamDecision decision;

    if ( count_fnal_ < fnal_step_width_ ) {
      decision.SetFNAL(true);
      ++count_fnal_;
    } else if ( count_cnaf_ < cnaf_step_width_ ) {
      decision.SetCNAF(true);
      ++count_cnaf_;
    } else if ( count_ral_ < ral_step_width_ ) {
      decision.SetRAL(true);
      ++count_ral_;
    } else if ( count_in2p3_ < in2p3_step_width_ ) {
      decision.SetIN2P3(true);
      ++count_in2p3_;
    } else if ( count_asgc_ < asgc_step_width_ ) {
      decision.SetASGC(true);
      ++count_asgc_;
    } else if ( count_fzk_ < fzk_step_width_ ) {
      decision.SetFZK(true);
      ++count_fzk_;
    } else if ( count_pic_ < pic_step_width_ ) {
      decision.SetPIC(true);
      ++count_pic_;
    } else {
      edm::LogError("Streamin") << "Could not set decision flag for any site. Reset didn't work";
      throw std::exception();
    }

    if ( count_ == step_width_ ) {
      count_       = 0;
      count_fnal_  = 0;
      count_cnaf_  = 0;
      count_ral_   = 0;
      count_in2p3_ = 0;
      count_asgc_  = 0;
      count_fzk_   = 0;
      count_pic_   = 0;
    }

    output->push_back(decision);

    // Step D: write output to file
    e.put(output);

  }

  unsigned int StreamDecider::GetDecimalPlaces(double number) {
    std::stringstream stream;
    stream << number;
    return (stream.str().length() - stream.str().find(".") - 1);
  }



}
