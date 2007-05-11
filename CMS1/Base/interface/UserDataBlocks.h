#ifndef CMS1_UserDataBlocks_h
#define CMS1_UserDataBlocks_h
// Description:     blocks that define user data structure of various objects
//
// Original Author: Dmytro Kovalskyi
//
// $Author: dmytro $
// $Date: 2007/04/17 04:43:13 $
// $Revision: 1.5 $
//
#include "CMS1/Base/interface/UserData.h"
#include "CMS1/Base/interface/EventData.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include <string>
namespace cms1 {
   struct BaseUserBlock
     {
	// register new block constituents according to their types
	virtual void registerBlock(EventData& event, const std::string& name_prefix, const std::string& alias_prefix="") = 0;
	
	// check whether the block can be used
	virtual bool usable() = 0;

	template <class T> void addEntry(std::vector<UserData<T>* >& container, 
					 UserData<T>*& var,
					 const std::string& name,
					 const std::string& name_prefix,
					 const std::string& alias_prefix="")
	  {
	     container.push_back( new UserData<T>(name, name_prefix, alias_prefix) );
	     var = container.back();
	  }
     };
   
	
   struct TrackUserBlock: public BaseUserBlock
     {
	TrackUserBlock(): vP4(0),vD0(0),vZ0(0),vSiHits(0)
	  {}

	void registerBlock(EventData& event, const std::string& name_prefix, const std::string& alias_prefix="")
	  {
	     addEntry(event.vp4UserData,    vP4,     "_p4",   name_prefix, alias_prefix);
	     addEntry(event.vfloatUserData, vD0,     "_d0",   name_prefix, alias_prefix);
	     addEntry(event.vfloatUserData, vZ0,     "_z0",   name_prefix, alias_prefix);
	     addEntry(event.vintUserData,   vSiHits, "_hits", name_prefix, alias_prefix);
	  }
	void fill(const reco::Candidate& candidate){
	   if ( ! usable() ) return;
	   vP4->get()->push_back( candidate.p4() );
	   vD0->get()->push_back( -999. );
	   vZ0->get()->push_back( -999. );
	   vSiHits->get()->push_back( -999 );
	}
	void fill(const reco::Track& track){
	   if ( ! usable() ) return;
	   // use massless track hypothesis, not that big deal at high energy pp colider
	   // add 1e-6 to momentum, to avoid rounding errors leading to imaginary mass
	   vP4->get()->push_back( LorentzVector( track.px(), track.py(), track.pz(), track.p()+1e-6 ) );
	   vD0->get()->push_back( track.d0() );
	   vZ0->get()->push_back( track.dz() );
	   vSiHits->get()->push_back( track.numberOfValidHits() );
	}
	bool usable()
	  {
	     if (vP4 == 0||vD0==0||vZ0==0||vSiHits==0){
		std::cout << "ERROR: attempt to fill data for non-registered TrackUserBlock" << std::endl;
		return false;
	     }
	     return true;
	  }
	UserDataVP4*         vP4;
	UserDataVFloat*      vD0;
	UserDataVFloat*      vZ0;
	UserDataVInt*        vSiHits;
     };
   
   struct JetUserBlock: public BaseUserBlock
     {
	JetUserBlock(): vP4(0),vEmFraction(0)
	  {}

	void registerBlock(EventData& event, const std::string& name_prefix, const std::string& alias_prefix="")
	  {
	     addEntry(event.vp4UserData,    vP4,         "_p4",   name_prefix, alias_prefix);
	     addEntry(event.vfloatUserData, vEmFraction, "_emFraction",   name_prefix, alias_prefix);
	  }
	void fill(const reco::CaloJet& jet){
	   if ( ! usable() ) return;
	   vP4->get()->push_back( jet.p4() );
	   vEmFraction->get()->push_back( jet.emEnergyFraction() );
	}
	bool usable()
	  {
	     if (vP4 == 0||vEmFraction==0){
		std::cout << "ERROR: attempt to fill data for non-registered JetUserBlock" << std::endl;
		return false;
	     }
	     return true;
	  }
	UserDataVP4*         vP4;
	UserDataVFloat*      vEmFraction;
     };
/*   struct EnergyUserBlock: public BaseUserBlock
     {
	void registerBlock(EventData& event, const std::string& name_prefix, const std::string& alias_prefix="")
	  {
	     addEntry(event.vp4UserData,    vEmEnergy,     "_emEnergy",   name_prefix, alias_prefix);
	     addEntry(event.vfloatUserData, vHadEnergy,    "_hadEnergy",  name_prefix, alias_prefix);
	     addEntry(event.vfloatUserData, vHoEnergy,     "_hoEnergy",   name_prefix, alias_prefix);
	  }
	UserDataVFloat*               vEmEnergy;
	UserDataVFloat*               vHadEnergy;
	UserDataVFloat*               vHoEnergy;
     };
 */
}

#endif
