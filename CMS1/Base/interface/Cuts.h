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
// $Author: dmytro $
// $Date: 2007/03/16 07:03:58 $
// $Revision: 1.4 $
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
	bool testGenParticle(const HepMC::GenParticle&) const;
	bool testGenJet(const reco::GenJet&) const;
	
	// perform logical AND of two cuts 
	void AND(const Cuts& );
	
	// Cut parameters
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
	pt_min(0.), pt_max(99999.),
	eta_min(-9999.), eta_max(9999),
	phi_min(-9999.), phi_max(9999),
	et_min(-99999.), et_max(99999),
	met_min(-99999.), met_max(99999),
	isolated(NotIsolated), truthMatchingType( NoMatching ),
	data_(0)
	{}
	
	
      private:
	bool truthMatch( const math::XYZVector& ) const;
	const EventData* data_;
     };
   
}

#endif

