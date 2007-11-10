// -*- C++ -*-
//
/*

 Description: check collection size
  
*/
//
// Original Author:  Dmytro Kovalskyi
//         Created:  Sat Nov 10 02:44:47 CST 2007
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
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Math/interface/LorentzVector.h"

class CollectionSizeFilter : public edm::EDFilter {
   public:
      explicit CollectionSizeFilter(const edm::ParameterSet&);
      ~CollectionSizeFilter();

   private:
      virtual bool filter(edm::Event&, const edm::EventSetup&);
   edm::InputTag tag;
   int min_size;
   int max_size;
};

CollectionSizeFilter::CollectionSizeFilter(const edm::ParameterSet& iConfig)
{
   tag = iConfig.getUntrackedParameter<edm::InputTag>("collection");
   min_size = iConfig.getUntrackedParameter<int>("min_size");
   max_size = iConfig.getUntrackedParameter<int>("max_size");
}


CollectionSizeFilter::~CollectionSizeFilter()
{
}

bool
CollectionSizeFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   if ( max_size < 0 ) return false;
   if ( min_size <= 0 ) return true;
   int size = -1;
   
   try {
      edm::Handle<edm::View<reco::Candidate> > data;
      iEvent.getByLabel(tag, data);
      size = data->size();
   } catch (...) {
      size = -1;
   }
   
   if ( size > -1 ) return size >= min_size && size <= max_size;
   
   try {
      edm::Handle<std::vector<int> > data;
      iEvent.getByLabel(tag, data);
      size = data->size();
   } catch (...) {
      size = -1;
   }
   
   if ( size > -1 ) return size >= min_size && size <= max_size;
   
   try {
      edm::Handle<std::vector<float> > data;
      iEvent.getByLabel(tag, data);
      size = data->size();
   } catch (...) {
      size = -1;
   }
   
   if ( size > -1 ) return size >= min_size && size <= max_size;
   
   try {
      edm::Handle<std::vector<math::XYZTLorentzVector> > data;
      iEvent.getByLabel(tag, data);
      size = data->size();
   } catch (...) {
      size = -1;
   }
   
   if ( size > -1 ) return size >= min_size && size <= max_size;

   return false;
}

//define this as a plug-in
DEFINE_FWK_MODULE(CollectionSizeFilter);
