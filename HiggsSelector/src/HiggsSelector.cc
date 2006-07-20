#include "Gutsche/HiggsSelector/interface/HiggsSelector.h"

#include <TFile.h>
#include <iostream>

#include "FWCore/Framework/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"

#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"

HiggsSelector::HiggsSelector() {
   h_chisqtr = 0;
   h_pttr    = 0;
   h_mmumu   = 0;
   h_noz     = 0;
   h_mzz     = 0;
   h_m4mu    = 0;
}

HiggsSelector::~HiggsSelector() {
  if ( h_chisqtr != 0 ) {
    delete h_chisqtr;
    h_chisqtr = 0;
  }
  if ( h_pttr != 0 ) {
    delete h_pttr;
    h_pttr = 0;
  }
  if ( h_mmumu != 0 ) {
    delete h_mmumu;
    h_mmumu = 0;
  }
  if ( h_noz != 0 ) {
    delete h_noz;
    h_noz = 0;
  }
  if ( h_mzz != 0 ) {
    delete h_mzz;
    h_mzz = 0;
  }
  if ( h_m4mu != 0 ) {
    delete h_m4mu;
    h_m4mu = 0;
  }
}



void HiggsSelector::begin(TList*&)
{
}

void HiggsSelector::preProcessing(const TList*, TList& out ) {
  // initialize objects
  if ( h_chisqtr != 0 ) {
    out.Remove(h_chisqtr);
    delete h_chisqtr;
    h_chisqtr = 0;
  }
  if ( h_pttr != 0 ) {
    out.Remove(h_pttr);
    delete h_pttr;
    h_pttr = 0;
  }
  if ( h_mmumu != 0 ) {
    out.Remove(h_mmumu);
    delete h_mmumu;
    h_mmumu = 0;
  }
  if ( h_noz != 0 ) {
    out.Remove(h_noz);
    delete h_noz;
    h_noz = 0;
  }
  if ( h_mzz != 0 ) {
    out.Remove(h_mzz);
    delete h_mzz;
    h_mzz = 0;
  }
  if ( h_m4mu != 0 ) {
    out.Remove(h_m4mu);
    delete h_m4mu;
    h_m4mu = 0;
  }
  h_chisqtr = new TH1F("chisqtr","Track chisq",100,0.,40.);
  h_pttr = new TH1F("pttr","Track pT (GeV)",100,0.0,200.0);
  h_mmumu = new TH1F("mmumu","mu+mu- mass (GeV)",100,0.0,200.0);
  h_noz = new TH1F("noz","num of reco Z",10,0.0,10.0);
  h_mzz = new TH1F("mzz","Z0Z0 mass (GeV)",100,100.0,300.0);
  h_m4mu = new TH1F("m4mu","2mu+2mu- mass (GeV)",100,100.0,300.0);
  out.Add(h_chisqtr);
  out.Add(h_pttr);
  out.Add(h_mmumu);
  out.Add(h_noz);
  out.Add(h_mzz);
  out.Add(h_m4mu);
}

void HiggsSelector::process( const edm::Event& iEvent ) {
  
  std::cout << "processing run " << iEvent.id().run() << " event " << iEvent.id().event() << std::endl;

  edm::Handle<reco::TrackCollection> tracks;
  iEvent.getByLabel("RSWMaterial", tracks);

  int q4 = 0;
  int n4 = 0;
  double px4 = 0.0;
  double py4 = 0.0;
  double pz4 = 0.0;
  double e4  = 0.0;

  for ( reco::TrackCollection::const_iterator track = tracks->begin();
	track != tracks->end();
	++track ) {

    // fill chi2 and pt
    h_chisqtr->Fill(track->chi2());
    h_pttr->Fill(track->pt());

    // count
    ++n4;
    if ( track->transverseCurvature() > 0.0 ) {
      ++q4;
    } else {
      --q4;
    }

    px4+=track->px(); 
    py4+=track->py(); 
    pz4+=track->pz(); 
    
    e4+= sqrt(track->px()*track->px()+track->py()*track->py()
	      +track->pz()*track->pz()+0.011163691);
  }

  double ptot = sqrt( px4*px4 + py4*py4 + pz4*pz4 );
  double mz = sqrt((e4+ptot)*(e4-ptot));
  if ((4==n4)&&(0==q4))h_m4mu->Fill(mz);
  std::vector<double> Zpx;
  std::vector<double> Zpy;
  std::vector<double> Zpz;
  std::vector<double> Ze;
  std::vector<int> Zpart1;
  std::vector<int> Zpart2;
  reco::TrackCollection trackCollection = *tracks.product();

  if (tracks->size() >1){
    for ( unsigned int bindex = 0; bindex < tracks->size()-1; ++bindex ) {
      reco::Track track1 = trackCollection[bindex];
      for ( unsigned int cindex = bindex+1; cindex < tracks->size(); ++cindex ) {
	reco::Track track2 = trackCollection[cindex];
	if (track1.transverseCurvature()*track2.transverseCurvature() < 0.0){
	  double e1 = sqrt((track1.px()*track1.px())
			   +(track1.py()*track1.py())
			   +(track1.pz()*track1.pz())+0.011163691);
	  double e2 = sqrt((track2.px()*track2.px())
			   +(track2.py()*track2.py())
			   +(track2.pz()*track2.pz())+0.011163691);
	  double etot = e1+e2;
	  double pxtot = track1.px()+track2.px();
	  double pytot = track1.py()+track2.py();
	  double pztot = track1.pz()+track2.pz();
	  double ptot = sqrt( pxtot*pxtot + pytot*pytot + pztot*pztot );
	  double mz = sqrt((etot+ptot)*(etot-ptot));
	  h_mmumu->Fill(mz);
	  if ((mz>80.0)&&(mz<100.0)){
	    Zpx.push_back(pxtot); Zpy.push_back(pytot); Zpz.push_back(pztot); 
	    Ze.push_back(etot); Zpart1.push_back(bindex); Zpart2.push_back(cindex); 
	  }
	}//tracks opposite charge
      }//end track2
    }//end track1
  }//end got >1 trk
  std::cout << "Z count: " << Zpx.size() << std::endl;
  h_noz->Fill( Zpx.size() );
  if (Zpx.size() >1){
    for ( unsigned int bindex = 0; bindex < Zpx.size()-1; ++bindex ) {
      for ( unsigned int cindex = bindex+1; cindex < Zpx.size(); ++cindex ) {
	if ((Zpart1[bindex]!=Zpart1[cindex])&&(Zpart1[bindex]!=Zpart2[cindex])
            &&(Zpart2[bindex]!=Zpart1[cindex])&&(Zpart2[bindex]!=Zpart2[cindex])){
	  double etot = Ze[bindex]+Ze[cindex];
	  double pxtot = Zpx[bindex]+Zpx[cindex];
	  double pytot = Zpy[bindex]+Zpy[cindex];
	  double pztot = Zpz[bindex]+Zpz[cindex];
	  double ptot = sqrt( pxtot*pxtot + pytot*pytot + pztot*pztot );
	  double mh = sqrt((etot+ptot)*(etot-ptot));
	  h_mzz->Fill(mh);
	}
      }
    }
  }
}

void HiggsSelector::postProcessing(TList&)
{
}

void HiggsSelector::terminate(TList& out) {
  std::cout << "terminate" << std::endl;

  TFile output("HZZ.root","RECREATE");
  h_chisqtr->Clone();
  h_pttr->Clone();
  h_mmumu->Clone();
  h_noz->Clone();
  h_mzz->Clone();
  h_m4mu->Clone();
  output.Write();
  output.Close();

}
