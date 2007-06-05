#ifndef CMS1_BaseStreamer_h
#define CMS1_BaseStreamer_h
// Description:     streamer classes that break composite classes in basic types and know how to extract information
//
// Original Author: Dmytro Kovalskyi
//
// $Author: dmytro $
// $Date: 2007/05/22 07:12:39 $
// $Revision: 1.1 $
//
#include "CMS1/Base/interface/UserData.h"
#include "CMS1/Base/interface/EventData.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "CLHEP/HepMC/GenParticle.h"
#include "DataFormats/JetReco/interface/GenJet.h"

#include <string>
namespace cms1 {
   struct StreamerArguments
     {
	const reco::Candidate*    candidate;
	const HepMC::GenParticle* genParticle;
	const reco::GenJet*       genJet;
	const reco::Track*        track;
        float                     jetcorrection;

	StreamerArguments():
	  candidate(0),genParticle(0),genJet(0),track(0),jetcorrection(0)
          {}
     };
   
   class BaseStreamer
     {
      public:
	const std::vector<std::string>& getIntNames()     { return intNames_; }
	const std::vector<std::string>& getFloatNames()   { return floatNames_; }
	const std::vector<std::string>& getP4Names()      { return p4Names_; }
	int                             getInt( int i )   { return ints_.empty()? 0 : ints_[i]; }
	float                           getFloat( int i ) { return floats_.empty()? 0 : floats_[i]; }
	LorentzVector                   getP4( int i )    { return p4s_.empty()? LorentzVector(0,0,0,0) : p4s_[i]; }
	     
	// template <class T> void         fill( const T* ) = 0;
	
      protected:
	std::vector<std::string>       intNames_;
	std::vector<std::string>       floatNames_;
	std::vector<std::string>       p4Names_;
	std::vector<int>               ints_;
	std::vector<float>             floats_;
	std::vector<LorentzVector>     p4s_;
     };
}

#endif
