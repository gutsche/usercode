//
// Package:         GutSoftAnalyzers/GutSoftTrackingEventDisplay
// Class:           GutSoftTrackingEventDisplay
// 
// Description:     event display for tracking
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Tue Apr  3 21:47:43 UTC 2007
//
// $Author: gutsche $
// $Date: 2007/03/28 13:34:17 $
// $Revision: 1.2 $
//

#include "GutSoftAnalyzers/GutSoftTrackingEventDisplay/interface/GutSoftTrackingEventDisplay.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/Framework/interface/ESHandle.h"

#include "RecoTracker/RingRecord/interface/RingRecord.h"
#include "RecoTracker/RoadMapRecord/interface/RoadMapRecord.h"

#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticleFwd.h"

#include "SimDataFormats/TrackingHit/interface/PSimHit.h"

#include "TrackingTools/RoadSearchHitAccess/interface/RoadSearchDetIdHelper.h"

#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"

#include "DataFormats/DetId/interface/DetId.h"

#include "TGraph.h"

#include "DataFormats/TrajectorySeed/interface/TrajectorySeed.h"
#include "DataFormats/TrajectorySeed/interface/TrajectorySeedCollection.h"

GutSoftTrackingEventDisplay::GutSoftTrackingEventDisplay(const edm::ParameterSet& iConfig)
{

  trackingTruthInputTag_  = iConfig.getUntrackedParameter<edm::InputTag>("TrackingTruthInputTag");
  trajectorySeedInputTag_ = iConfig.getUntrackedParameter<edm::InputTag>("TrajectorySeedInputTag");

  ringLabel_              = iConfig.getUntrackedParameter<std::string>("RingLabel");
  roadLabel_              = iConfig.getUntrackedParameter<std::string>("RoadLabel");

  outputFileNameBegin_    = iConfig.getUntrackedParameter<std::string>("OutputFileName") +  ".ps[";
  outputFileName_         = iConfig.getUntrackedParameter<std::string>("OutputFileName") +  ".ps";
  outputFileNameEnd_      = iConfig.getUntrackedParameter<std::string>("OutputFileName") +  ".ps]";

  labelRings_             = iConfig.getUntrackedParameter<bool>("LabelRings");

}


GutSoftTrackingEventDisplay::~GutSoftTrackingEventDisplay()
{
 
}


void
GutSoftTrackingEventDisplay::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  // get tracking particle collection for true information
  edm::Handle<TrackingParticleCollection>  trackingParticleCollectionHandle;
  iEvent.getByLabel(trackingTruthInputTag_,trackingParticleCollectionHandle);
  const TrackingParticleCollection *trackingParticleCollection = trackingParticleCollectionHandle.product();
    
  // get seed collection
  const TrajectorySeedCollection *seedCollection = 0;
  try {
    edm::Handle<TrajectorySeedCollection> seedCollectionHandle;
    iEvent.getByLabel(trajectorySeedInputTag_,seedCollectionHandle);
    seedCollection = seedCollectionHandle.product();
  }
  catch (edm::Exception const& x) {
    if ( x.categoryCode() == edm::errors::ProductNotFound ) {
      if ( x.history().size() == 1 ) {
        static const TrajectorySeedCollection s_empty;
        seedCollection = &s_empty;
        edm::LogWarning("GutSoftSeedingEfficiencyAnalyzer") << "Collection TrajectorySeedCollection with label " << trajectorySeedInputTag_ << " cannot be found, using empty collection of same type";
      }
    }
  }

  // get tracker geometry
  edm::ESHandle<TrackerGeometry> trackerHandle;
  iSetup.get<TrackerDigiGeometryRecord>().get(trackerHandle);
  tracker_ = trackerHandle.product();

  // arrays and vector for graphs
  double x[100];
  double y[100];
  std::vector<TGraph*> particles;

  // draw run and event number
  xy_pad_left_->cd();
  std::ostringstream run_event;
  run_event << "run: " << iEvent.id().run() << " event: " << iEvent.id().event();
  TPaveLabel label(0.,0.9,1.,1.,run_event.str().c_str(),"brNDC");
  label.Draw();
  
  // draw xy
  xy_pad_right_->cd();
  xy_histo_->Draw();
  xy_histo_axis_right_->Draw("SAME");
  xy_histo_axis_top_->Draw("SAME");
  xy_histo_x_line_->Draw("SAME");
  xy_histo_y_line_->Draw("SAME");

  // draw tracking particles
  for ( TrackingParticleCollection::const_iterator trackingParticle = trackingParticleCollection->begin(),
	  trackingParticleEnd = trackingParticleCollection->end();
        trackingParticle != trackingParticleEnd;
        ++trackingParticle ) {
    if ( trackingParticle->charge() != 0 ) {
      if ( std::sqrt(trackingParticle->momentum().perp2()) >= 0.9 ) {

	unsigned int counter = 0;
	for ( std::vector<PSimHit>::const_iterator simHit = trackingParticle->pSimHit_begin(),
		simHitEnd = trackingParticle->pSimHit_end();
	      simHit != simHitEnd;
	      ++simHit ) {
	  GlobalPoint globalPosition = tracker_->idToDet(DetId(simHit->detUnitId()))->surface().toGlobal(simHit->localPosition());
	  x[counter] = globalPosition.x();
	  y[counter] = globalPosition.y();
	  ++counter;
	} // loop over sim hits
	TGraph *graph = new TGraph(counter,x,y);
	graph->Draw("C*");
	particles.push_back(graph);
      }
    }
  } // loop over tracking particles

  // draw seeds
  for ( TrajectorySeedCollection::const_iterator seed = seedCollection->begin(),
          seedEnd = seedCollection->end();
	seed != seedEnd;
	++seed ) {
    unsigned int counter = 0;
    for ( edm::OwnVector<TrackingRecHit>::const_iterator seedHit = seed->recHits().first,
	    seedHitEnd = seed->recHits().second;
	  seedHit != seedHitEnd;
	  ++seedHit ) {
      GlobalPoint globalPosition = tracker_->idToDet(seedHit->geographicalId())->surface().toGlobal(seedHit->localPosition());
      x[counter] = globalPosition.x();
      y[counter] = globalPosition.y();
      ++counter;
    }
    TGraph *graph = new TGraph(counter,x,y);
    graph->SetLineColor(2);
    graph->SetLineWidth(1);
    graph->SetMarkerColor(2);
    graph->Draw("*");
    particles.push_back(graph);
  }

  // draw rz
  rz_pad_->cd();
  rz_histo_->Draw();
  rz_histo_axis_right_->Draw("SAME");
  rz_histo_axis_top_->Draw("SAME");

  // draw rings
  for ( std::vector<TPaveLabel*>::iterator ringLabel = allRings_.begin();
	ringLabel != allRings_.end();
	++ringLabel ) {
    (*ringLabel)->Draw("SAME");
  }

  // draw seed rings
  for ( std::vector<TPaveLabel*>::iterator ringLabel = innerSeedRings_.begin();
	ringLabel != innerSeedRings_.end();
	++ringLabel ) {
    (*ringLabel)->Draw("SAME");
  }
  for ( std::vector<TPaveLabel*>::iterator ringLabel = outerSeedRings_.begin();
	ringLabel != outerSeedRings_.end();
	++ringLabel ) {
    (*ringLabel)->Draw("SAME");
  }

  for ( TrackingParticleCollection::const_iterator trackingParticle = trackingParticleCollection->begin(),
	  trackingParticleEnd = trackingParticleCollection->end();
        trackingParticle != trackingParticleEnd;
        ++trackingParticle ) {
    if ( trackingParticle->charge() != 0 ) {
      if ( std::sqrt(trackingParticle->momentum().perp2()) >= 0.9 ) {

	unsigned int counter = 0;
	for ( std::vector<PSimHit>::const_iterator simHit = trackingParticle->pSimHit_begin(),
		simHitEnd = trackingParticle->pSimHit_end();
	      simHit != simHitEnd;
	      ++simHit ) {
	  GlobalPoint globalPosition = tracker_->idToDet(DetId(simHit->detUnitId()))->surface().toGlobal(simHit->localPosition());
	  x[counter] = globalPosition.z();
	  y[counter] = globalPosition.perp();
	  ++counter;
	} // loop over sim hits
	TGraph *graph = new TGraph(counter,x,y);
	graph->Draw("C*");
	particles.push_back(graph);
      }
    }
  } // loop over tracking particles

  // draw seeds
  for ( TrajectorySeedCollection::const_iterator seed = seedCollection->begin(),
          seedEnd = seedCollection->end();
	seed != seedEnd;
	++seed ) {
    unsigned int counter = 0;
    for ( edm::OwnVector<TrackingRecHit>::const_iterator seedHit = seed->recHits().first,
	    seedHitEnd = seed->recHits().second;
	  seedHit != seedHitEnd;
	  ++seedHit ) {
      GlobalPoint globalPosition = tracker_->idToDet(seedHit->geographicalId())->surface().toGlobal(seedHit->localPosition());
      x[counter] = globalPosition.z();
      y[counter] = globalPosition.perp();
      ++counter;
    }
    TGraph *graph = new TGraph(counter,x,y);
    graph->SetLineColor(2);
    graph->SetLineWidth(1);
    graph->SetMarkerColor(2);
    graph->Draw("*");
    particles.push_back(graph);
  }

  canvas_->Print(outputFileName_.c_str());

  // delete graphs
  for ( std::vector<TGraph*>::iterator graph = particles.begin();
	graph != particles.end();
	++graph ) {
    delete *graph;
  }


}


void 
GutSoftTrackingEventDisplay::beginJob(const edm::EventSetup& es)
{

  // get rings
  edm::ESHandle<Rings> ringHandle;
  es.get<RingRecord>().get(ringLabel_,ringHandle);
  rings_ = ringHandle.product();

  // get roads
  edm::ESHandle<Roads> roadHandle;
  es.get<RoadMapRecord>().get(roadLabel_,roadHandle);
  roads_ = roadHandle.product();

  // ring label
  std::ostringstream text;

  // fill all rings
  for (Rings::const_iterator ring = rings_->begin();
       ring != rings_->end();
       ++ring ) {
    text.str("");
    if ( labelRings_ ) {
      text << ring->second.getindex();
    }
    TPaveLabel *label = new TPaveLabel(ring->second.getzmin(),
				       ring->second.getrmin(),
				       ring->second.getzmax(),
				       ring->second.getrmax(),
				       text.str().c_str());
    label->SetFillColor(41);
    label->SetBorderSize(1);
    allRings_.push_back(label);
  } 

  // fill inner and outer seed rings
  for ( Roads::const_iterator road = roads_->begin();
	road != roads_->end();
	++road ) {

    const Roads::RoadSeed *seed = &(road->first);
//     const Roads::RoadSet  *set  = &(road->second);

    // fill inner seed rings
    for (std::vector<const Ring*>::const_iterator ring = seed->first.begin();
	 ring != seed->first.end();
	 ++ring ) {
      text.str("");
      if ( labelRings_ ) {
	text << (*ring)->getindex();
      }
      TPaveLabel *seed1 = new TPaveLabel((*ring)->getzmin(),
					 (*ring)->getrmin(),
					 (*ring)->getzmax(),
					 (*ring)->getrmax(),
					 text.str().c_str());
      seed1->SetFillColor(2);
      seed1->SetBorderSize(1);
      innerSeedRings_.push_back(seed1);
    }

    // fill outer seed rings
    for (std::vector<const Ring*>::const_iterator ring = seed->second.begin();
	 ring != seed->second.end();
	 ++ring ) {
      text.str("");
      if ( labelRings_ ) {
	text << (*ring)->getindex();
      }
      TPaveLabel *seed2 = new TPaveLabel((*ring)->getzmin(),
					 (*ring)->getrmin(),
					 (*ring)->getzmax(),
					 (*ring)->getrmax(),
					 text.str().c_str());
      seed2->SetFillColor(4);
      seed2->SetBorderSize(1);
      outerSeedRings_.push_back(seed2);
    }
  }
 
  // start canvas
  canvas_ = new TCanvas("event_display","Event Display",640,640);
  canvas_->SetTopMargin(0.02);
  canvas_->SetFrameBorderMode(0);
  canvas_->SetBorderMode(0);
  canvas_->SetFillColor(0);
  canvas_->Draw();

  // divide into 3 pads
  canvas_->Divide(1,2);
  xy_pad_ = (TPad*)canvas_->GetPrimitive("event_display_1");
  rz_pad_ = (TPad*)canvas_->GetPrimitive("event_display_2");
  xy_pad_->Divide(2,1);
  xy_pad_left_ = (TPad*)xy_pad_->GetPrimitive("event_display_1_1");
  xy_pad_right_ = (TPad*)xy_pad_->GetPrimitive("event_display_1_2");

  // begin file with plots
  canvas_->Print(outputFileNameBegin_.c_str());

  // create xy histogram
  xy_pad_right_->cd();
  xy_histo_ = new TH2D("xy_histo","",100,-120,120,100,-120,120);
  xy_histo_->SetStats(kFALSE);
  xy_histo_->GetXaxis()->SetTitle("x [cm]");
  xy_histo_->GetXaxis()->SetTitleOffset(1.0);
  xy_histo_->GetXaxis()->SetTitleSize(0.05);
  xy_histo_->GetYaxis()->SetTitle("y [cm]");
  xy_histo_->GetYaxis()->SetTitleOffset(1.1);
  xy_histo_->GetYaxis()->SetTitleSize(0.05);

  //draw an axis on the right side
  xy_histo_axis_right_ = new TGaxis(120,-120,120,120,
				    -120,120,510,"+L");

  //draw an axis on the top side
  xy_histo_axis_top_ = new TGaxis(-120,120,120,120,
				  -120,120,510,"-L");

  xy_histo_x_line_ = new TLine(-120,0,120,0);
  xy_histo_x_line_->SetLineWidth(3);

  xy_histo_y_line_ = new TLine(0,-120,0,120);
  xy_histo_y_line_->SetLineWidth(3);

  // create rz histogram
  rz_pad_->cd();
  rz_histo_ = new TH2D("rz_histo","",100,-280,280,100,0,120);
  rz_histo_->SetStats(kFALSE);
  rz_histo_->GetXaxis()->SetTitle("z [cm]");
  rz_histo_->GetXaxis()->SetTitleOffset(0.9);
  rz_histo_->GetXaxis()->SetTitleSize(0.05);
  rz_histo_->GetYaxis()->SetTitle("r [cm]");
  rz_histo_->GetYaxis()->SetTitleOffset(1.0);
  rz_histo_->GetYaxis()->SetTitleSize(0.05);

  //draw an axis on the right side
  rz_histo_axis_right_ = new TGaxis(280,0,280,120,
				    0,120,510,"+L");

  //draw an axis on the top side
  rz_histo_axis_top_ = new TGaxis(-280,120,280,120,
				  -280,280,510,"-L");

}

void 
GutSoftTrackingEventDisplay::endJob() {

  // end file with plots
  canvas_->Print(outputFileNameEnd_.c_str());

}
