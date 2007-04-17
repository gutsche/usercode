#ifndef CMS1EventData_h
#define CMS1EventData_h
//
// Package:         CMS1/Base
// Class:           EventData
// 
// Description:     container of event information (collections, objects etc)
//
// Original Author: Dmytro Kovalskyi
//
// $Author: dmytro $
// $Date: 2007/04/12 19:35:08 $
// $Revision: 1.4 $
//

#include "CLHEP/HepMC/GenParticle.h"
// #include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"

namespace cms1 {
   struct EventData 
     {
	EventData():mcInfo(0){}
	
	// event data
	const edm::Event* iEvent;
	
	// ------------ DERIVED STUFF ----------------
	
	// generator info 
	std::vector<HepMC::GenParticle>  mcInfo;
	
	// ------------ METHODS ----------------------
	template <class T> const T* getData(const std::string label, const std::string instance = "") const
	  {
	     edm::Handle<T> t;
	     iEvent->getByLabel(label,instance,t);
	     return t.product();
	  }
     };
   
}
#endif
