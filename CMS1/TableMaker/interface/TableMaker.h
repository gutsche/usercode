#ifndef TableMaker_h
#define TableMaker_h

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
// $Date: 2007/03/09 21:24:33 $
// $Revision: 1.8 $
//

#include "CMS1/Muons/interface/Muons.h"
#include "CMS1/Electrons/interface/Electrons.h"
#include "CMS1/Jets/interface/Jets.h"
#include "CMS1/MET/interface/MET.h"

#include "CMS1/Base/interface/Cuts.h"

#include <TH1.h>
#include <TH2.h>
#include <TFile.h>
#include <vector>


namespace cms1 {

  class TableMaker {
  public:
    void analyze();
    void beginJob();
    void endJob();
  	TableMaker();
  	~TableMaker();

  protected:
    Cuts          tightMuon_;
    Cuts          looseMuon_;
    Cuts          allMuon_;

    Cuts          tightElectron_;
    Cuts          looseElectron_;

    Cuts          jet_;

    Cuts          metCut_;
    Cuts          metCutAroundZ_;

    Cuts          noCut_;

    double        ZRangeMinMass_;
    double        ZRangeMaxMass_;

    Muons         muons_;

    Electrons     electrons_;

    Jets          jets_;

    MET           MET_;

    reco::TrackCollection* trackCollection_;

    unsigned int  events_;
    unsigned int  countedEEJets_[5];
    unsigned int  countedEMuJets_[5];
    unsigned int  countedMuEJets_[5];
    unsigned int  countedMuMuJets_[5];

	
	//UCSD Grads
	std::string fileTag;  //Help differentiate files, input in cfg
	
	TH1I *hNJets;  //Number of events per jet binning
	TH2I *hTable;
	std::vector<TH1F *> hPTJet;//Events vs first four highest PT Jet
	std::vector<TH1F *> hMll; //Events vs inariant mass  x5
	std::vector<TH1F *> hPTTight; //events per missing tight lepton  x5
	std::vector<TH1F *> hPTLoose; //events per missing loose electron  x5
	std::vector<TH1F *> hPTLeading; //Events vs. highest PT lepton  x5
	std::vector<TH1F *> hPTTrailing; //Events vs. highest PT lepton  x5
	std::vector<TH1F *> hHT; //Events vs. temperature   x5
	std::vector<TH1F *> hMET; //events per missing et  x5


	void FillHistograms(std::vector<const reco::CaloJet*> jets, const RecoCandidate *,const RecoCandidate *, double);
	
  };
}

#endif
