//
// Package:         CMS1/EventDisplay
// Class:           EventDisplay
// 
// Description:     event display for CMS1
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Tue Apr  3 21:47:43 UTC 2007
//
// $Author: latb $
// $Date: 2007/07/11 18:25:07 $
// $Revision: 1.5 $
//
#include "CMS1/EventDisplay/interface/EventDisplay.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/Framework/interface/ESHandle.h"

#include "RecoTracker/RingRecord/interface/RingRecord.h"
#include "RecoTracker/RoadMapRecord/interface/RoadMapRecord.h"


#include "TrackingTools/RoadSearchHitAccess/interface/RoadSearchDetIdHelper.h"

#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"

#include "DataFormats/CaloTowers/interface/CaloTower.h"
#include "DataFormats/CaloTowers/interface/CaloTowerDetId.h"
#include "DataFormats/TrajectorySeed/interface/TrajectorySeed.h"
#include "DataFormats/TrajectorySeed/interface/TrajectorySeedCollection.h"

#include "DataFormats/DetId/interface/DetId.h"

#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticleFwd.h"

#include "SimDataFormats/TrackingHit/interface/PSimHit.h"
#include "TGraph.h"
#include "TEllipse.h"
#include "TBox.h"
#include "TLatex.h"
#include "DataFormats/Math/interface/LorentzVector.h"

/*
ECAL: Volume (minR, maxR, minZ, maxZ): 129.054, 160.52, 315.899, 342.844
HCAL: Volume (minR, maxR, minZ, maxZ): 190.181, 332.502, 388, 568
towers active volume: Volume (minR, maxR, minZ, maxZ): 143.136, 407.388, 320, 1265
CaloTowers: EBradius = 143.0, EEz = 320.0
*/

EventDisplay::EventDisplay(const edm::ParameterSet& iConfig)
{

	trackingTruthInputTag_  = iConfig.getUntrackedParameter<edm::InputTag>("TrackingTruthInputTag");
	trajectorySeedInputTag_ = iConfig.getUntrackedParameter<edm::InputTag>("TrajectorySeedInputTag");

	ringLabel_              = iConfig.getUntrackedParameter<std::string>("RingLabel");
	roadLabel_              = iConfig.getUntrackedParameter<std::string>("RoadLabel");

	outputFileNameBegin_    = iConfig.getUntrackedParameter<std::string>("OutputFileName") +  ".ps[";
	outputFileName_         = iConfig.getUntrackedParameter<std::string>("OutputFileName") +  ".ps";
	outputFileNameEnd_      = iConfig.getUntrackedParameter<std::string>("OutputFileName") +  ".ps]";

	labelRings_             = iConfig.getUntrackedParameter<bool>("LabelRings");
	
	minTrackPt_             = iConfig.getUntrackedParameter<double>("MinTrackPt");

        displayVectorOfInts_    = iConfig.getUntrackedParameter<std::vector<edm::InputTag> >("DisplayVectorOfInts");
        displayVectorOfFloats_  = iConfig.getUntrackedParameter<std::vector<edm::InputTag> >("DisplayVectorOfFloats");
        displayVectorOfP4s_     = iConfig.getUntrackedParameter<std::vector<edm::InputTag> >("DisplayVectorOfP4s");
   
	RZprojective_ = iConfig.getUntrackedParameter<bool>("RZprojective");
	XYshowEndcaps_ = iConfig.getUntrackedParameter<bool>("XYshowEndcaps");
	hadColor_ = 34; //TColor::GetColor(190,190,255);
	emColor_ = kBlue; //TColor::GetColor(90,90,255);

	EBradius_ = 143.;
	EEz_ = 320.;

	debug_ = iConfig.getUntrackedParameter<bool>("Debug");
	
	
	nFiltered_ = 0;
	
	filterOn_ = iConfig.getUntrackedParameter<bool>("FilterOn");
	std::cout << "---->> EventFilter is "; filterOn_ ? std::cout << "ON" : std::cout << "OFF"; std::cout << std::endl;

	filters_ = iConfig.getUntrackedParameter<std::vector<std::string> >("Filters");
	
	filterRunEventList_ = false;
	for(std::vector<std::string>::const_iterator iter=filters_.begin();
		iter != filters_.end(); 
		++iter) {
			if (*iter == "RunEventList") {
				std::cout << "---->> EventFilter is filtering for " << *iter << std::endl;
				filterRunEventList_ = true;
			}
	}

	runEventList_  = iConfig.getUntrackedParameter<std::vector<unsigned int> >("RunEventList");
	if (debug_) {
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


EventDisplay::~EventDisplay()
{

}


void
	EventDisplay::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	
	if (filterOn_) {
		bool skip = true;

		if (filterRunEventList_) {

			unsigned int r = iEvent.id().run();
			unsigned int e = iEvent.id().event();
			unsigned int thisRen = 1000000*r+e;

			for(std::vector<unsigned int>::const_iterator iter=runEventList_.begin();
			iter!= runEventList_.end(); 
			iter++) {
				unsigned int ren = *iter;		
				if ( ren == thisRen ) { 
					skip = false;
					++nFiltered_;
					if (debug_)
						std::cout << "--> EventFilter: " << nFiltered_ << " - run: " << r << " event: " << e <<	" passed." << std::endl;
					break; 
				}
			}
		}
		if (skip) return;
	}	

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

	// draw run and event number, EPH view
	ef_pad_left_->cd();
   ef_histo_->Draw();
//	ef_histo_axis_right_->Draw("SAME");
//	ef_histo_axis_top_->Draw("SAME");
////??????????????????????????????????????????????????????
	hadColor_ = TColor::GetColor(190,190,255);
	emColor_ = TColor::GetColor(90,90,255);
////??????????????????????????????????????????????????????

	displayCaloTowers(EPH, iEvent);

	std::ostringstream run_event;
	run_event << "run: " << iEvent.id().run() << " event: " << iEvent.id().event();
	TPaveLabel label(0.,0.9,1.,1.,run_event.str().c_str(),"brNDC");
	label.Draw();

	// draw xy
	xy_pad_right_->cd();
	xy_histo_->Draw();
	xy_histo_axis_right_->Draw("SAME");
	xy_histo_axis_top_->Draw("SAME");

	TEllipse innerBarrel(0.,0.,EBradius_, EBradius_);
	innerBarrel.Draw();
	displayCaloTowers(XY, iEvent);


	// draw tracking particles
	for ( TrackingParticleCollection::const_iterator trackingParticle = trackingParticleCollection->begin(),
		trackingParticleEnd = trackingParticleCollection->end();
	trackingParticle != trackingParticleEnd;
	++trackingParticle ) {
		if ( trackingParticle->charge() != 0 ) {
			if ( std::sqrt(trackingParticle->momentum().perp2()) >= minTrackPt_ ) {

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
			        
			   switch ( abs(trackingParticle->pdgId()) ) {
			    case 13: // muon
			      graph->SetLineColor(kMagenta);
			      graph->SetMarkerColor(kMagenta); 
			      break;
			    case 211: // pion
			      graph->SetLineColor(kBlue);
			      graph->SetMarkerColor(kBlue);
			      break;
			    case 11: // electron
			      graph->SetLineColor(kRed);
			      graph->SetMarkerColor(kRed);
			      break;
			    default:
			      graph->SetLineColor(kBlack);
			   }
			   
			   
				graph->SetMarkerSize(.7);
		  	graph->SetMarkerStyle(7); 
				graph->Draw("CP");

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

		graph->SetMarkerSize(.7);
  	graph->SetMarkerStyle(7); 
		graph->Draw("P");

		particles.push_back(graph);
	}


	// draw rz
	rz_pad_->cd();
	rz_histo_->Draw();
	rz_histo_axis_right_->Draw("SAME");
	rz_histo_axis_top_->Draw("SAME");

	TBox myBox(-EEz_, 0., EEz_, EBradius_);
	myBox.SetFillStyle(0);
	myBox.SetLineColor(kBlack);
	myBox.SetLineWidth(1);
	myBox.Draw("l");

	displayCaloTowers(RZ, iEvent);


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
			if ( std::sqrt(trackingParticle->momentum().perp2()) >= minTrackPt_ ) {

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
			   
			   switch ( abs(trackingParticle->pdgId()) ) {
			    case 13: // muon
			      graph->SetLineColor(kMagenta);
			      graph->SetMarkerColor(kMagenta); 
			      break;
			    case 211: // pion
			      graph->SetLineColor(kBlue);
			      graph->SetMarkerColor(kBlue);
			      break;
			    case 11: // electron
			      graph->SetLineColor(kRed);
			      graph->SetMarkerColor(kRed);
			      break;
			    default:
			      graph->SetLineColor(kBlack);
			   }

				graph->SetMarkerSize(.7);
		  	graph->SetMarkerStyle(7); 
				graph->Draw("CP");

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
		graph->SetMarkerSize(.7);
  	graph->SetMarkerStyle(7); 
		graph->Draw("P");
		particles.push_back(graph);
	}


   info_pad_->cd();
   info_pad_->Clear();
   // Display information
   TLatex display;
   display.SetTextAlign(12);
   double tsize = 0.025;
   display.SetTextSize(tsize);
   
   int line(0),column(0);
   // ints first
   std::vector<std::string> messages;
   getText<std::vector<int> >(iEvent, messages, displayVectorOfInts_);
   getText<std::vector<float> >(iEvent, messages, displayVectorOfFloats_);
   getText<std::vector<math::XYZTLorentzVector> >(iEvent, messages, displayVectorOfP4s_);
   
   for ( std::vector<std::string>::const_iterator message = messages.begin();
	 message != messages.end(); ++message )
     {
	display.DrawText(0.05+column*0.5, 1 - (0.05 + tsize*line*1.5), message->c_str() );
	std::cout << message->c_str() << std::endl;
	++line;
	if (1 - (0.05 + tsize*line*1.5) < 0 ) 
	  {
	     ++column;
	     line = 0;
	  }
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
	EventDisplay::beginJob(const edm::EventSetup& es)
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
		label->SetFillColor(18);
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
			seed1->SetFillColor(18);
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
			seed2->SetFillColor(18);
			seed2->SetBorderSize(1);
			outerSeedRings_.push_back(seed2);
		}
	}

	// start canvas
	canvas_ = new TCanvas("event_display","Event Display",900,600);
	canvas_->SetTopMargin(0.02);
	canvas_->SetFrameBorderMode(0);
	canvas_->SetBorderMode(0);
	canvas_->SetFillColor(0);
	canvas_->Draw();

	// divide into 3 pads
	canvas_->Divide(1,2);
	xy_pad_ = (TPad*)canvas_->GetPrimitive("event_display_1");
	rz_pad_ = (TPad*)canvas_->GetPrimitive("event_display_2");
	xy_pad_->Divide(3,1);
	ef_pad_left_ = (TPad*)xy_pad_->GetPrimitive("event_display_1_1");
	xy_pad_right_ = (TPad*)xy_pad_->GetPrimitive("event_display_1_2");
	info_pad_ = (TPad*)xy_pad_->GetPrimitive("event_display_1_3");

	// begin file with plots
	canvas_->Print(outputFileNameBegin_.c_str());

	// create xy histogram

	xy_pad_right_->cd();
	double xy_max = 250.;
	xy_histo_ = new TH2D("xy_histo","",100,-xy_max,xy_max,100,-xy_max,xy_max);
	xy_histo_->SetStats(kFALSE);
	xy_pad_right_->SetTicks(1,1);
	xy_pad_right_->SetLeftMargin(.001);
	xy_pad_right_->SetRightMargin(.001);
	xy_pad_right_->SetTopMargin(.001);
	xy_pad_right_->SetBottomMargin(.001);

//////	gStyle->SetHistFillColor(kWhite);
	xy_histo_->GetXaxis()->SetTitle("x [cm]");
	xy_histo_->GetXaxis()->SetTitleOffset(1.0);
	xy_histo_->GetXaxis()->SetTitleSize(0.05);
	xy_histo_->GetYaxis()->SetTitle("y [cm]");
	xy_histo_->GetYaxis()->SetTitleOffset(1.1);
	xy_histo_->GetYaxis()->SetTitleSize(0.05);

	xy_histo_->GetYaxis()->SetLabelOffset(-.11);
	xy_histo_->GetXaxis()->SetLabelOffset(-.95);

	//draw an axis on the right side
	xy_histo_axis_right_ = new TGaxis(xy_max,-xy_max,xy_max,xy_max,
		-xy_max,xy_max,510,"+L");

	//draw an axis on the top side
	xy_histo_axis_top_ = new TGaxis(-xy_max,xy_max,xy_max,xy_max,
		-xy_max,xy_max,510,"-L");

	// create ef (eta-phi) histogram
	ef_pad_left_->cd();
	double eta_max = 5;
	double phi_max = 3.5;
	ef_histo_ = new TH2D("ef_histo","",100,-eta_max,eta_max,100,-phi_max,phi_max);
	ef_histo_->SetStats(kFALSE);
	ef_pad_left_->SetTicks(1,1);
	ef_pad_left_->SetLeftMargin(.001);
	ef_pad_left_->SetRightMargin(.001);
	double marg = (eta_max-phi_max)/eta_max/2.;
	ef_pad_left_->SetTopMargin(marg);
	ef_pad_left_->SetBottomMargin(marg);
	ef_histo_->GetXaxis()->SetTitle("eta");
	ef_histo_->GetXaxis()->SetTitleOffset(-1.0);
	ef_histo_->GetXaxis()->SetTitleSize(0.05);
	ef_histo_->GetYaxis()->SetTitle("phi");
	ef_histo_->GetYaxis()->SetTitleOffset(-1.1);
	ef_histo_->GetYaxis()->SetTitleSize(0.05);
//	ef_histo_->GetXaxis()->SetLabelOffset(3.);
	ef_histo_->GetYaxis()->SetLabelOffset(-.1);

	//draw an axis on the right side
//	ef_histo_axis_right_ = new TGaxis(eta_max,-eta_max,ef_max,ef_max,
//		-ef_max,ef_max,510,"+L");

	//draw an axis on the top side
//	ef_histo_axis_top_ = new TGaxis(-ef_max,ef_max,ef_max,ef_max,
//		-ef_max,ef_max,510,"-L");

	// create rz histogram
	rz_pad_->cd();
	double rz_max = 400;
	rz_histo_ = new TH2D("rz_histo","",100,-rz_max,rz_max,100,0,xy_max);
	rz_histo_->SetStats(kFALSE);
	rz_pad_->SetTicks(1,1);
	rz_pad_->SetLeftMargin(.001);
	rz_pad_->SetRightMargin(.001);
	rz_pad_->SetTopMargin(.001);
	rz_pad_->SetBottomMargin(.001);

	rz_histo_->GetXaxis()->SetTitle("z [cm]");
	rz_histo_->GetXaxis()->SetTitleOffset(0.9);
	rz_histo_->GetXaxis()->SetTitleSize(0.05);
	rz_histo_->GetYaxis()->SetTitle("r [cm]");
	rz_histo_->GetYaxis()->SetTitleOffset(-50.0);
	rz_histo_->GetYaxis()->SetTitleSize(0.05);
	rz_histo_->GetXaxis()->SetLabelOffset(-.95);
	rz_histo_->GetYaxis()->SetLabelOffset(-.001);

	//draw an axis on the right side
	rz_histo_axis_right_ = new TGaxis(rz_max,0,rz_max,xy_max,
		0,xy_max,510,"+L");

	//draw an axis on the top side
	rz_histo_axis_top_ = new TGaxis(-rz_max,xy_max,rz_max,xy_max,
		-rz_max,rz_max,510,"-L");

}

void EventDisplay::displayCaloTowers(View_t viewType, const edm::Event& iEvent) {

// fill CaloTowers
	edm::Handle<CaloTowerCollection> caloTowers;
	iEvent.getByLabel( "towerMaker", caloTowers );

	for(CaloTowerCollection::const_iterator tower = caloTowers->begin(); tower != caloTowers->end(); ++tower) {
		CaloTower ct = *tower;

		displayCaloTower(viewType, &ct);

		CaloTowerDetId id = tower->id();
		if (debug_) std::cout << "Tower (" << id.ieta() << "," << id.iphi() << ")" << ":"; 
		if (debug_) std::cout << ct.et() << " GeV ET (EM=" << ct.emEt() <<
		  " HAD=" << ct.hadEt() << " OUTER=" << ct.outerEt() << ") (" << ct.eta() << "," << ct.phi() << ")"
			<< std::endl;
	}

// show scale indicator
	
//	displayCaloTowerScale(viewType);	

}

void EventDisplay::displayCaloTower(View_t viewType, CaloTower* t) {

	double eta                = t->eta();
	double phi                = t->phi();
	double et                 = t->et();
	double emfrac             = abs(t->emEt()/et);


	double rectx[5]           = {-1., 1., 1., -1., -1.};
	double recty[5]           = {-1., -1., 1., 1., -1.};

	int iEta = t->id().ieta();
	Layer_t layer = BARREL;
	if (iEta < -17) 		layer = ENDCAP;
	else if (iEta > 17) layer = ENDCAP;
	Layer_t hlayer = BARREL;
	if (iEta < -16) 		hlayer = ENDCAP;
	else if (iEta > 16) hlayer = ENDCAP;


	
	switch( viewType ) {
		case EPH:
		{
			double me                 = 50.;
			double ampl               = .5;
			ampl *= (log(et + 1.)/log(me + 1.));
			double etaprop[5];
			double phiprop[5];
			for ( unsigned jc = 0; jc<4; ++jc ) { 	
				etaprop[jc]            = eta + rectx[jc]*ampl;
				phiprop[jc]            = phi + recty[jc]*ampl;
			}

			etaprop[4]              = etaprop[0];
			phiprop[4]              = phiprop[0]; // closing the polycell    

			if (debug_) cout << "e,p " << eta << " " << phi <<  " " << etaprop[0] << " " << phiprop[0] << endl;

			TPolyLine linePropXY;          
			linePropXY.SetLineColor(kBlack);
			linePropXY.SetLineWidth(1);
			linePropXY.SetFillColor(hadColor_);
			linePropXY.DrawPolyLine(5,etaprop,phiprop,"F");

			for ( unsigned jc = 0; jc<4; ++jc ) { 	
				etaprop[jc]            = eta + rectx[jc]*ampl*emfrac;
				phiprop[jc]            = phi + recty[jc]*ampl*emfrac;
			}
			etaprop[4]              = etaprop[0];
			phiprop[4]              = phiprop[0]; // closing the polycell    
			linePropXY.SetFillColor(emColor_);
			linePropXY.DrawPolyLine(5,etaprop,phiprop,"F");



			break;
		}
		case  XY:
		{
			if(layer == BARREL) {
				double me                 = 50.;
				double ampl               = 25.;
				ampl *= (log(et + 1.)/log(me + 1.));

				double rho = EBradius_;//cm barrel radius
				double dphi = .05;
				double xprop[5];
				double yprop[5];

				double sphic1 = sin(phi-dphi);
				double cphic1 = cos(phi-dphi);
				double sphic2 = sin(phi+dphi);
				double cphic2 = cos(phi+dphi);
				double rho1 = rho;
				double rho2 = rho+5*ampl;

				xprop[0] = rho1*cphic1;
				yprop[0] = rho1*sphic1;
				xprop[1] = rho2*cphic1;
				yprop[1] = rho2*sphic1;
				xprop[2] = rho2*cphic2;
				yprop[2] = rho2*sphic2;
				xprop[3] = rho1*cphic2;
				yprop[3] = rho1*sphic2;
				xprop[4] = xprop[0];
				yprop[4] = yprop[0]; // closing the polycell    
				
				TPolyLine linePropXY;          
				linePropXY.SetLineColor(kBlack);
				linePropXY.SetLineWidth(1);
				if(hlayer == BARREL) {
					linePropXY.SetFillColor(hadColor_);
					linePropXY.DrawPolyLine(5,xprop,yprop,"F");
				}

				rho2 = rho+5*ampl*emfrac;
				xprop[0] = rho1*cphic1;
				yprop[0] = rho1*sphic1;
				xprop[1] = rho2*cphic1;
				yprop[1] = rho2*sphic1;
				xprop[2] = rho2*cphic2;
				yprop[2] = rho2*sphic2;
				xprop[3] = rho1*cphic2;
				yprop[3] = rho1*sphic2;
				xprop[4] = xprop[0];
				yprop[4] = yprop[0]; // closing the polycell    

				linePropXY.SetFillColor(emColor_);
				linePropXY.DrawPolyLine(5,xprop,yprop,"F");

			} else if (hlayer == ENDCAP) {
				if (!XYshowEndcaps_) break;
				
				double me                 = 50.;
				double ampl               = 25.;
				ampl *= (log(et + 1.)/log(me + 1.));
				if (layer==BARREL) ampl = ampl * (1.-emfrac); // correct overlap: only show HCAL in endcap

				double the             = 2.*std::atan(std::exp(-eta));
				double zec             = EEz_; //cm endcap z position
				double rho             = abs(zec*tan(the));
				double xx              = rho*cos(phi);
				double yy              = rho*sin(phi);

				double xprop[5];
				double yprop[5];
				for ( unsigned jc = 0; jc<4; ++jc ) { 	
					xprop[jc]             = xx + rectx[jc]*ampl;
					yprop[jc]             = yy + recty[jc]*ampl;
				}
				xprop[4]               = xprop[0];
				yprop[4]               = yprop[0]; // closing the polycell    

				TPolyLine linePropXY;    
				linePropXY.SetLineColor(kBlack);
				linePropXY.SetLineWidth(1);
				linePropXY.SetFillColor(hadColor_);
				linePropXY.DrawPolyLine(5,xprop,yprop,"F");

				if (layer == ENDCAP) {
					for ( unsigned jc = 0; jc<4; ++jc ) { 	
						xprop[jc]             = xx + rectx[jc]*ampl*emfrac;
						yprop[jc]             = yy + recty[jc]*ampl*emfrac;
					}
					xprop[4]                = xprop[0];
					yprop[4]                = yprop[0]; // closing the polycell    
					linePropXY.SetFillColor(emColor_);
					linePropXY.DrawPolyLine(5,xprop,yprop,"F");
				}
			}else {
				std::cout << "ERROR: CaloTower neither ENDCAP nor BARREL -- this can never happen!!!" << std::endl;
			}
			break;
		}
		case  RZ:
		{
			if(layer == BARREL) {
				double me = 50.;
				double ampl = 25.;
				ampl *= (log(et + 1.)/log(me + 1.));

				double rho = EBradius_;//cm barrel radius
				double dz = 10.;
				double zprop[5];
				double rprop[5];

				double the = 2.*std::atan(std::exp(-eta));
				double tthe = tan(the);
				double rho1 = rho;
				double rho2 = rho+5*ampl;

				if (RZprojective_) {
					zprop[0] = rho1/tthe+dz;
					rprop[0] = rho1;
					zprop[1] = rho2/tthe+dz;
					rprop[1] = rho2;
					zprop[2] = rho2/tthe-dz;
					rprop[2] = rho2;
					zprop[3] = rho1/tthe-dz;
					rprop[3] = rho1;
				} else {
					zprop[0] = rho1/tthe+dz;
					rprop[0] = rho1;
					zprop[1] = rho1/tthe+dz;
					rprop[1] = rho2;
					zprop[2] = rho1/tthe-dz;
					rprop[2] = rho2;
					zprop[3] = rho1/tthe-dz;
					rprop[3] = rho1;
				}
				zprop[4] = zprop[0];
				rprop[4] = rprop[0]; // closing the polycell    

				TPolyLine linePropXY;          
				linePropXY.SetLineColor(kBlack);
				linePropXY.SetLineWidth(1);
				if(hlayer == BARREL) {
					linePropXY.SetFillColor(hadColor_);
					linePropXY.DrawPolyLine(5,zprop,rprop,"F");
				}

				rho2 = rho+5*ampl*emfrac;
				if (RZprojective_) {
					zprop[0] = rho1/tthe+dz;
					rprop[0] = rho1;
					zprop[1] = rho2/tthe+dz;
					rprop[1] = rho2;
					zprop[2] = rho2/tthe-dz;
					rprop[2] = rho2;
					zprop[3] = rho1/tthe-dz;
					rprop[3] = rho1;
				} else {
					zprop[0] = rho1/tthe+dz;
					rprop[0] = rho1;
					zprop[1] = rho1/tthe+dz;
					rprop[1] = rho2;
					zprop[2] = rho1/tthe-dz;
					rprop[2] = rho2;
					zprop[3] = rho1/tthe-dz;
					rprop[3] = rho1;
				}
				zprop[4] = zprop[0];
				rprop[4] = rprop[0]; // closing the polycell    

				linePropXY.SetFillColor(emColor_);
				linePropXY.DrawPolyLine(5,zprop,rprop,"F");

			} else if (hlayer == ENDCAP) {
				double me                 = 50.;
				double ampl               = 25.;
				ampl *= (log(et + 1.)/log(me + 1.));
				if (layer==BARREL) ampl = ampl * (1.-emfrac); // correct overlap: only show HCAL in endcap

				double zec = EEz_; //cm endcap z
				double dr = 5.;
				double zprop[5];
				double rprop[5];

				double the = 2.*std::atan(std::exp(-eta));
				double tthe = tan(the);

				double z1 = zec;
				double z2 = z1+5*ampl;
				if (eta < 0.) { 
					z1 = -z1;
					z2 = -z2;
				}
				if (RZprojective_) {
					zprop[0] = z1;
					rprop[0] = z1*tthe-dr;
					zprop[1] = z2;
					rprop[1] = z2*tthe-dr;
					zprop[2] = z2;
					rprop[2] = z2*tthe+dr;
					zprop[3] = z1;
					rprop[3] = z1*tthe+dr;
				} else {
					zprop[0] = z1;
					rprop[0] = z1*tthe-dr;
					zprop[1] = z2;
					rprop[1] = z1*tthe-dr;
					zprop[2] = z2;
					rprop[2] = z1*tthe+dr;
					zprop[3] = z1;
					rprop[3] = z1*tthe+dr;
				}
				zprop[4] = zprop[0];
				rprop[4] = rprop[0]; // closing the polycell    

				TPolyLine linePropXY; 
				linePropXY.SetLineColor(kBlack);
				linePropXY.SetLineWidth(1);
				linePropXY.SetFillColor(hadColor_);
				linePropXY.DrawPolyLine(5,zprop,rprop,"F");

				if (layer == ENDCAP) {
					z2 = zec+5*ampl*emfrac;
					if (eta < 0.) z2 = -z2;
					if (RZprojective_) {
						zprop[0] = z1;
						rprop[0] = z1*tthe-dr;
						zprop[1] = z2;
						rprop[1] = z2*tthe-dr;
						zprop[2] = z2;
						rprop[2] = z2*tthe+dr;
						zprop[3] = z1;
						rprop[3] = z1*tthe+dr;
					} else {
						zprop[0] = z1;
						rprop[0] = z1*tthe-dr;
						zprop[1] = z2;
						rprop[1] = z1*tthe-dr;
						zprop[2] = z2;
						rprop[2] = z1*tthe+dr;
						zprop[3] = z1;
						rprop[3] = z1*tthe+dr;	
					}
					zprop[4] = zprop[0];
					rprop[4] = rprop[0]; // closing the polycell    

					linePropXY.SetFillColor(emColor_);
					linePropXY.DrawPolyLine(5,zprop,rprop,"F");
					if (debug_) cout << "emfrac " << emfrac << " z,r " << zprop[0] << " " << rprop[0] <<  " " << zprop[1] << " " << rprop[1] << endl;

				}	
			}else {
				std::cout << "ERROR: CaloTower neither ENDCAP nor BARREL -- this can never happen!!!" << std::endl;
			}
			break;
		}
		default:
		break;
	}

	return;
}

void EventDisplay::displayCaloTowerScale(View_t viewType) {

	double eta                = -4.;
	double phi                = -3.8;
	double et                 = 50.;
	double emfrac             = 0.2;


	double rectx[5]           = {-1., 1., 1., -1., -1.};
	double recty[5]           = {-1., -1., 1., 1., -1.};

	Layer_t layer = BARREL;
	Layer_t hlayer = BARREL;
	
	switch( viewType ) {
		case EPH:
		{
			double me                 = 50.;
			double ampl               = .5;
			ampl *= (log(et + 1.)/log(me + 1.));
			double etaprop[5];
			double phiprop[5];
			for ( unsigned jc = 0; jc<4; ++jc ) { 	
				etaprop[jc]            = eta + rectx[jc]*ampl;
				phiprop[jc]            = phi + recty[jc]*ampl;
			}

			etaprop[4]              = etaprop[0];
			phiprop[4]              = phiprop[0]; // closing the polycell    

			if (debug_) cout << "e,p " << eta << " " << phi <<  " " << etaprop[0] << " " << phiprop[0] << endl;

			TPolyLine linePropXY;          
			linePropXY.SetLineColor(kBlack);
			linePropXY.SetLineWidth(1);
			linePropXY.SetFillColor(kRed);
			linePropXY.DrawPolyLine(5,etaprop,phiprop,"F");

			for ( unsigned jc = 0; jc<4; ++jc ) { 	
				etaprop[jc]            = eta + rectx[jc]*ampl*emfrac;
				phiprop[jc]            = phi + recty[jc]*ampl*emfrac;
			}
			etaprop[4]              = etaprop[0];
			phiprop[4]              = phiprop[0]; // closing the polycell    
			linePropXY.SetFillColor(kYellow);
			linePropXY.DrawPolyLine(5,etaprop,phiprop,"F");



			break;
		}
		case  XY:
		{
			if(layer == BARREL) {
				double me                 = 50.;
				double ampl               = 25.;
				ampl *= (log(et + 1.)/log(me + 1.));

				double rho = EBradius_;//cm barrel radius
				double dphi = .05;
				double xprop[5];
				double yprop[5];

				double sphic1 = sin(phi-dphi);
				double cphic1 = cos(phi-dphi);
				double sphic2 = sin(phi+dphi);
				double cphic2 = cos(phi+dphi);
				double rho1 = rho;
				double rho2 = rho+5*ampl;

				xprop[0] = rho1*cphic1;
				yprop[0] = rho1*sphic1;
				xprop[1] = rho2*cphic1;
				yprop[1] = rho2*sphic1;
				xprop[2] = rho2*cphic2;
				yprop[2] = rho2*sphic2;
				xprop[3] = rho1*cphic2;
				yprop[3] = rho1*sphic2;
				xprop[4] = xprop[0];
				yprop[4] = yprop[0]; // closing the polycell    
				
				TPolyLine linePropXY;          
				linePropXY.SetLineColor(kBlack);
				linePropXY.SetLineWidth(1);
				if(hlayer == BARREL) {
					linePropXY.SetFillColor(kRed);
					linePropXY.DrawPolyLine(5,xprop,yprop,"F");
				}

				rho2 = rho+5*ampl*emfrac;
				xprop[0] = rho1*cphic1;
				yprop[0] = rho1*sphic1;
				xprop[1] = rho2*cphic1;
				yprop[1] = rho2*sphic1;
				xprop[2] = rho2*cphic2;
				yprop[2] = rho2*sphic2;
				xprop[3] = rho1*cphic2;
				yprop[3] = rho1*sphic2;
				xprop[4] = xprop[0];
				yprop[4] = yprop[0]; // closing the polycell    

				linePropXY.SetFillColor(kYellow);
				linePropXY.DrawPolyLine(5,xprop,yprop,"F");

			} else if (hlayer == ENDCAP) {
				if (!XYshowEndcaps_) break;
				
				double me                 = 50.;
				double ampl               = 25.;
				ampl *= (log(et + 1.)/log(me + 1.));
				if (layer==BARREL) ampl = ampl * (1.-emfrac); // correct overlap: only show HCAL in endcap

				double the             = 2.*std::atan(std::exp(-eta));
				double zec             = EEz_; //cm endcap z position
				double rho             = abs(zec*tan(the));
				double xx              = rho*cos(phi);
				double yy              = rho*sin(phi);

				double xprop[5];
				double yprop[5];
				for ( unsigned jc = 0; jc<4; ++jc ) { 	
					xprop[jc]             = xx + rectx[jc]*ampl;
					yprop[jc]             = yy + recty[jc]*ampl;
				}
				xprop[4]               = xprop[0];
				yprop[4]               = yprop[0]; // closing the polycell    

				TPolyLine linePropXY;    
				linePropXY.SetLineColor(kBlack);
				linePropXY.SetLineWidth(1);
				linePropXY.SetFillColor(kRed);
				linePropXY.DrawPolyLine(5,xprop,yprop,"F");

				if (layer == ENDCAP) {
					for ( unsigned jc = 0; jc<4; ++jc ) { 	
						xprop[jc]             = xx + rectx[jc]*ampl*emfrac;
						yprop[jc]             = yy + recty[jc]*ampl*emfrac;
					}
					xprop[4]                = xprop[0];
					yprop[4]                = yprop[0]; // closing the polycell    
					linePropXY.SetFillColor(kYellow);
					linePropXY.DrawPolyLine(5,xprop,yprop,"F");
				}
			}else {
				std::cout << "ERROR: CaloTower neither ENDCAP nor BARREL -- this can never happen!!!" << std::endl;
			}
			break;
		}
		case  RZ:
		{
			if(layer == BARREL) {
				double me = 50.;
				double ampl = 25.;
				ampl *= (log(et + 1.)/log(me + 1.));

				double rho = EBradius_;//cm barrel radius
				double dz = 10.;
				double zprop[5];
				double rprop[5];

				double the = 2.*std::atan(std::exp(-eta));
				double tthe = tan(the);
				double rho1 = rho;
				double rho2 = rho+5*ampl;

				if (RZprojective_) {
					zprop[0] = rho1/tthe+dz;
					rprop[0] = rho1;
					zprop[1] = rho2/tthe+dz;
					rprop[1] = rho2;
					zprop[2] = rho2/tthe-dz;
					rprop[2] = rho2;
					zprop[3] = rho1/tthe-dz;
					rprop[3] = rho1;
				} else {
					zprop[0] = rho1/tthe+dz;
					rprop[0] = rho1;
					zprop[1] = rho1/tthe+dz;
					rprop[1] = rho2;
					zprop[2] = rho1/tthe-dz;
					rprop[2] = rho2;
					zprop[3] = rho1/tthe-dz;
					rprop[3] = rho1;
				}
				zprop[4] = zprop[0];
				rprop[4] = rprop[0]; // closing the polycell    

				TPolyLine linePropXY;          
				linePropXY.SetLineColor(kBlack);
				linePropXY.SetLineWidth(1);
				if(hlayer == BARREL) {
					linePropXY.SetFillColor(kRed);
					linePropXY.DrawPolyLine(5,zprop,rprop,"F");
				}

				rho2 = rho+5*ampl*emfrac;
				if (RZprojective_) {
					zprop[0] = rho1/tthe+dz;
					rprop[0] = rho1;
					zprop[1] = rho2/tthe+dz;
					rprop[1] = rho2;
					zprop[2] = rho2/tthe-dz;
					rprop[2] = rho2;
					zprop[3] = rho1/tthe-dz;
					rprop[3] = rho1;
				} else {
					zprop[0] = rho1/tthe+dz;
					rprop[0] = rho1;
					zprop[1] = rho1/tthe+dz;
					rprop[1] = rho2;
					zprop[2] = rho1/tthe-dz;
					rprop[2] = rho2;
					zprop[3] = rho1/tthe-dz;
					rprop[3] = rho1;
				}
				zprop[4] = zprop[0];
				rprop[4] = rprop[0]; // closing the polycell    

				linePropXY.SetFillColor(kYellow);
				linePropXY.DrawPolyLine(5,zprop,rprop,"F");

			} else if (hlayer == ENDCAP) {
				double me                 = 50.;
				double ampl               = 25.;
				ampl *= (log(et + 1.)/log(me + 1.));
				if (layer==BARREL) ampl = ampl * (1.-emfrac); // correct overlap: only show HCAL in endcap

				double zec = EEz_; //cm endcap z
				double dr = 5.;
				double zprop[5];
				double rprop[5];

				double the = 2.*std::atan(std::exp(-eta));
				double tthe = tan(the);

				double z1 = zec;
				double z2 = z1+5*ampl;
				if (eta < 0.) { 
					z1 = -z1;
					z2 = -z2;
				}
				if (RZprojective_) {
					zprop[0] = z1;
					rprop[0] = z1*tthe-dr;
					zprop[1] = z2;
					rprop[1] = z2*tthe-dr;
					zprop[2] = z2;
					rprop[2] = z2*tthe+dr;
					zprop[3] = z1;
					rprop[3] = z1*tthe+dr;
				} else {
					zprop[0] = z1;
					rprop[0] = z1*tthe-dr;
					zprop[1] = z2;
					rprop[1] = z1*tthe-dr;
					zprop[2] = z2;
					rprop[2] = z1*tthe+dr;
					zprop[3] = z1;
					rprop[3] = z1*tthe+dr;
				}
				zprop[4] = zprop[0];
				rprop[4] = rprop[0]; // closing the polycell    

				TPolyLine linePropXY; 
				linePropXY.SetLineColor(kBlack);
				linePropXY.SetLineWidth(1);
				linePropXY.SetFillColor(kRed);
				linePropXY.DrawPolyLine(5,zprop,rprop,"F");

				if (layer == ENDCAP) {
					z2 = zec+5*ampl*emfrac;
					if (eta < 0.) z2 = -z2;
					if (RZprojective_) {
						zprop[0] = z1;
						rprop[0] = z1*tthe-dr;
						zprop[1] = z2;
						rprop[1] = z2*tthe-dr;
						zprop[2] = z2;
						rprop[2] = z2*tthe+dr;
						zprop[3] = z1;
						rprop[3] = z1*tthe+dr;
					} else {
						zprop[0] = z1;
						rprop[0] = z1*tthe-dr;
						zprop[1] = z2;
						rprop[1] = z1*tthe-dr;
						zprop[2] = z2;
						rprop[2] = z1*tthe+dr;
						zprop[3] = z1;
						rprop[3] = z1*tthe+dr;	
					}
					zprop[4] = zprop[0];
					rprop[4] = rprop[0]; // closing the polycell    

					linePropXY.SetFillColor(kYellow);
					linePropXY.DrawPolyLine(5,zprop,rprop,"F");
					if (debug_) cout << "emfrac " << emfrac << " z,r " << zprop[0] << " " << rprop[0] <<  " " << zprop[1] << " " << rprop[1] << endl;

				}	
			}else {
				std::cout << "ERROR: CaloTower neither ENDCAP nor BARREL -- this can never happen!!!" << std::endl;
			}
			break;
		}
		default:
		break;
	}

	return;
}


void 
EventDisplay::endJob() {

	// end file with plots
	canvas_->Print(outputFileNameEnd_.c_str());

}

