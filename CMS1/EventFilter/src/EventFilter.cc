// -*- C++ -*-
//
// Package:    EventFilter
// Class:      EventFilter
// 
/**\class EventFilter EventFilter.cc 

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Lothar Bauerdick
//         Created:  Fri Jul  6 15:36:57 CDT 2007
// $Id$
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

//
// class declaration
//

class EventFilter : public edm::EDFilter {
   public:
      explicit EventFilter(const edm::ParameterSet&);
      ~EventFilter();

   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual bool filter(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      // ----------member data ---------------------------

		std::vector<unsigned int> runEventList_;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
EventFilter::EventFilter(const edm::ParameterSet& iConfig)
{
	//now do what ever initialization is needed
	runEventList_  = iConfig.getUntrackedParameter<std::vector<unsigned int> >("RunEventList");

	if (false) {
		for(std::vector<unsigned int>::const_iterator iter=runEventList_.begin();
		iter != runEventList_.end(); 
		++iter) {
			unsigned int ren = *iter;		
			unsigned int e = ren%1000000;
			unsigned int r = ren/1000000;
			std::cout << "---->> Will filter run: " << r << " event: " << e <<	std::endl;
		}
	}
}


EventFilter::~EventFilter()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
	EventFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;

	unsigned int r = iEvent.id().run();
	unsigned int e = iEvent.id().event();

	unsigned int thisRen = 1000000*r+e;
	bool result = false;

	for(std::vector<unsigned int>::const_iterator iter=runEventList_.begin();
		iter!= runEventList_.end(); 
		iter++) {
		unsigned int ren = *iter;		
		if ( ren == thisRen ) { 
			result = true;
			std::cout << "--> EventFilter - run: " << r << " event: " << e <<	" passed." << std::endl;
			break; 
		}
	}
	return result;
}

// ------------ method called once each job just before starting event loop  ------------
void 
EventFilter::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
EventFilter::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(EventFilter);
