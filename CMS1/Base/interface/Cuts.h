#ifndef CMS1BaseCuts_h
#define CMS1BaseCuts_h

//
// Package:         CMS1/Base
// Class:           Cuts
// 
// Description:     selectors for standard classes
//
// Original Author: Dmytro Kovalskyi
//
// $Author: gutsche $
// $Date: 2007/01/29 18:15:28 $
// $Revision: 1.1 $
//
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/Candidate/interface/Candidate.h"

namespace cms1 {
   class Cuts
     {
      public:
	bool testTrack(const reco::Track& ) const;
	bool testCandidate(const reco::Candidate & ) const;
	
	double pt_min;
	double pt_max;
	double eta_min;
	double eta_max;
	double phi_min;
	double phi_max;
	double et_min;
	double et_max;
	double met_min;
	double met_max;
	
	// default values
	Cuts():
	pt_min(0.), pt_max(99999.),
	eta_min(-9999.), eta_max(100),
	phi_min(-9999.), phi_max(9999),
	et_min(-99999.), et_max(99999),
	met_min(-99999.), met_max(99999)
	  {}
     };
   
}

#endif
