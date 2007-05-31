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
// $Author: mangano $
// $Date: 2007/05/21 16:51:30 $
// $Revision: 1.6 $
//
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "CLHEP/HepMC/GenParticle.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"
#include "CMS1/Base/interface/EventData.h"

namespace cms1 {
   class Cuts
     {
      public:
	enum TruthMatchingType { NoMatching, Electron, Muon, Tau };
	enum IsolationType { NotIsolated = 0, Isolated = 1 };
	     
	bool testTrack(const reco::Track&) const;
	bool testCandidate(const reco::Candidate &) const;
	bool testGenParticle(const HepMC::GenParticle*) const;
	bool testGenJet(const reco::GenJet&) const;
	
	// perform logical AND of two cuts 
	void AND(const Cuts& );

	// REPLACE current cuts with cuts from a new object
	void REPLACE(const Cuts& );

	//Print cuts out
	void PrintCuts();
	
	// Cut parameters
	double Default_Parameter_Value;
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
	IsolationType isolated;
	TruthMatchingType truthMatchingType; 
	
	// verify that a jet doesn't look like electron. False if 
	// an electron was found near it.
	static bool testJetForElectrons(const reco::Candidate&,
					const std::vector<const reco::Candidate*> &);
	  
	// track isolation is computed based on a generic collection of tracks,
 	// which is refined for this task inside the function.
	static double trackRelIsolation(const math::XYZVector momentum, 
					const math::XYZPoint vertex,
					const std::vector<reco::Track>* tracks = 0, 
					double dRConeMax = 0.3, double dRConeMin = 0.1, 
					double tkVtxDMax = 0.1,
					double vtxDiffDMax = 9999., double vtxDiffZMax = 9999., 
					double ptMin = 1.5, bool debug = false);
	
	// this class can only read event data
	void setEventData( const EventData* data ){ data_ = data; }
	const EventData* getEventData(){ return data_; }
	
	///////////////////// INTERNAL STUFF ////////////////////////
	// default values
	
	
	Cuts():
	  Default_Parameter_Value(99999.0),
	  pt_min(0.0), pt_max(Default_Parameter_Value),
	  eta_min(-1*Default_Parameter_Value), eta_max(Default_Parameter_Value),
	  phi_min(-1*Default_Parameter_Value), phi_max(Default_Parameter_Value),
	  et_min(-1*Default_Parameter_Value), et_max(Default_Parameter_Value),
	  met_min(-1*Default_Parameter_Value), met_max(Default_Parameter_Value),
	  isolated(NotIsolated), truthMatchingType( NoMatching ),
	  data_(0)
	{}
	
	
      private:
	bool truthMatch( const math::XYZVector& ) const;
	const EventData* data_;
	
     };
   
}

#endif

