//
// Package:         CMS1/Base
// Class:           BaseAnalyzer
// 
// Description:     abstract base class for TnS analysis
//
// Original Author: Dmytro Kovalskyi
//
// $Author: dmytro $
// $Date: 2007/04/12 19:37:15 $
// $Revision: 1.2 $
//
#include "CMS1/BaseAnalyzer/interface/BaseAnalyzer.h"
#include "FWCore/Framework/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"

void cms1::BaseAnalyzer::configure(const edm::ParameterSet& iConfig)
{
   // let black boxes know where to read data from
   theMuons.setEventData     ( &theData );
   theElectrons.setEventData ( &theData );
   theJets.setEventData      ( &theData );
   theMET.setEventData       ( &theData );
}

void cms1::BaseAnalyzer::processEvent(const edm::Event& iEvent)
{
   theData.iEvent = &iEvent;
   
   // get MC Event
   edm::Handle<edm::HepMCProduct> mcCollectionHandle;
   iEvent.getByLabel("VtxSmeared", mcCollectionHandle);
   const HepMC::GenEvent* genEvent = mcCollectionHandle->GetEvent();
   theData.mcInfo.clear();
   for (HepMC::GenEvent::particle_const_iterator p = genEvent->particles_begin(); p != genEvent->particles_end(); ++p)
     theData.mcInfo.push_back(**p);
}
