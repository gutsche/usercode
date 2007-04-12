//
// Package:         CMS1/TableMaker
// Class:           TableMaker

// 
// Description:     EDAnalyzer filling table for di-lepton analysis dependent on number of jets
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Tue Feb 20 23:00:01 UTC 2007
//
// $Author: latb $
// $Date: 2007/03/28 22:45:07 $
// $Revision: 1.7 $
//

#include <vector>
#include <utility>
#include <sstream>
#include <iomanip>

#include "CMS1/TableMaker/interface/TableMakerFW.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"


cms1::TableMakerFW::~TableMakerFW()
{}

cms1::TableMakerFW::TableMakerFW(const edm::ParameterSet& iConfig)
{
   theDataFW = &theData;
   configure(iConfig); 
}

void
cms1::TableMakerFW::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   // process default stuff (load data in the EventData)
   BaseAnalyzerFW::analyze( iEvent, iSetup );
	     
   // get MC Event
   edm::Handle<edm::HepMCProduct> mcCollectionHandle;
   iEvent.getByLabel("VtxSmeared", mcCollectionHandle);
   const HepMC::GenEvent* genEvent = mcCollectionHandle->GetEvent();
   std::vector<HepMC::GenParticle> mcParticle;
   for (HepMC::GenEvent::particle_const_iterator p = genEvent->particles_begin(); p != genEvent->particles_end(); ++p)
     mcParticle.push_back(**p);
   theData.mcInfo = &mcParticle;
   
   // run user code
   processEvent();
}
