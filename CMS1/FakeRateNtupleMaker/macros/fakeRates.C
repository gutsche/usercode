#include "TChain.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "Math/LorentzVector.h"
#include <vector>
#include "fakeRates.h"

struct SortTLorentzVectorByPt
{
	bool operator()(const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > &one, 
		const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > &two)
	{
		return one.Pt() > two.Pt();
	}
};

float CalculateDeltaR(const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > &one, 
const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > &two) {
	return sqrt((one.Eta()-two.Eta())*(one.Eta()-two.Eta()) +
		(one.Phi()-two.Phi())*(one.Phi()-two.Phi()) );
}


TH1F* createHistogram(string name,
	string title,
	unsigned int nBins,
	float low,
	float high,
	string x_axis_title,
string y_axis_title) {
	TH1F *temp = new TH1F(name.c_str(),
		title.c_str(),
		nBins,
		low,
		high);
	temp->Sumw2();
	temp->SetXTitle(x_axis_title.c_str());
	temp->SetYTitle(y_axis_title.c_str());

	return temp;
}

TH1F* createVarHistogram(string name,
	string title,
	vector<float> &bins,
	string x_axis_title,
string y_axis_title) {

	const unsigned int nBins = bins.size()-1;
	float binArray[nBins+1];
	for (unsigned int i = 0;
	i < nBins+1;
	++i) {
		binArray[i] = bins[i];
	}

	TH1F *temp = new TH1F(name.c_str(),
		title.c_str(),
		nBins,binArray);
	temp->Sumw2();
	temp->SetXTitle(x_axis_title.c_str());
	temp->SetYTitle(y_axis_title.c_str());

	return temp;
}

TH2F* create2DHistogram(string name,
	string title,
	unsigned int nXBins,
	float lowX,
	float highX,
	unsigned int nYBins,
	float lowY,
	float highY,
	string x_axis_title,
	string y_axis_title,
string z_axis_title) {
	TH2F *temp = new TH2F(name.c_str(),
		title.c_str(),
		nXBins,
		lowX,
		highX,
		nYBins,
		lowY,
		highY);
	temp->Sumw2();
	temp->SetXTitle(x_axis_title.c_str());
	temp->SetYTitle(y_axis_title.c_str());
	temp->SetZTitle(z_axis_title.c_str());

	return temp;
}

TH2F* createVar2DHistogram(string name,
	string title,
	vector<float> &xbins,
	vector<float> &ybins,
	string x_axis_title,
	string y_axis_title,
string z_axis_title) {

	const unsigned int nxBins = xbins.size()-1;
	float xbinArray[nxBins+1];
	for (unsigned int i = 0;
	i < nxBins+1;
	++i) {
		xbinArray[i] = xbins[i];
	}

	const unsigned int nyBins = ybins.size()-1;
	float ybinArray[nyBins+1];
	for (unsigned int i = 0;
	i < nyBins+1;
	++i) {
		ybinArray[i] = ybins[i];
	}

	TH2F *temp = new TH2F(name.c_str(),
		title.c_str(),
		nxBins,
		xbinArray,
		nyBins,
		ybinArray);
	temp->Sumw2();
	temp->SetXTitle(x_axis_title.c_str());
	temp->SetYTitle(y_axis_title.c_str());
	temp->SetZTitle(z_axis_title.c_str());

	return temp;
}

//----------------------------------------------------------------
// Collection of functions with lepton selections off the ntuple
//---------------------------------------------------------------

// First row loose selection thresholds, second row tight thresholds
float bcutsee[16] = {0.014,  0.0125, 0.0125, 0.0125, 0.0125, 0.0125, 0.0125, 0.0125, 
	0.014,  0.012,  0.012,  0.012,  0.012,  0.012,  0.012,  0.012};
float bcutdeta[16]= {0.009,  0.0085, 0.0085, 0.0035, 0.0085, 0.0035, 0.0085, 0.0085,  
	0.009,  0.0055, 0.0055, 0.004,  0.0055, 0.004,  0.0055, 0.0055};
float bcutdphi[16]= {0.06,   0.06,   0.10,   0.02,   0.06,   0.06,   0.06,   0.06,      
	0.04,   0.04,   0.07,   0.02,   0.04,   0.04,   0.04,   0.04};
float bcuteop2[16]= {0.55,   0.88,   0.88,   0.88,   0.88,   0.88,   0.88,   0.88,      
	0.55,   0.91,   0.91,   0.91,   0.91,   0.91,   0.91,   0.91};
float bcuthoe[16] = {0.125,  0.055,  0.055,  0.10,   0.055,  0.055,  0.055,  0.055,     
	0.12,   0.055,  0.055,  0.095,  0.055,  0.055,  0.055,  0.055};
float ecutsee[16] = {0.031,  0.028,  0.028,  0.028,  0.028,  0.028,  0.028,  0.028,     
	0.027,  0.026,  0.026,  0.026,  0.026,  0.026,  0.026,  0.026};
float ecutdeta[16]= {0.009,  0.009,  0.009,  0.009,  0.009,  0.009,  0.009,  0.009,     
	0.008,  0.008,  0.008,  0.008,  0.008,  0.008,  0.008,  0.008};
float ecutdphi[16]= {0.06,   0.10,   0.10,   0.02,   0.10,   0.10,   0.10,   0.10,      
	0.055,  0.085,  0.085,  0.02,   0.085,  0.085,  0.085,  0.085};
float ecuteop2[16]= {0.85,   0.85,   0.85,   0.85,   0.85,   0.85,   0.85,   0.85,      
	0.85,   0.85,   0.85,   0.85,   0.85,   0.85,   0.85,   0.85};
float ecuthoe[16] = {0.125,  0.10,   0.10,   0.10,   0.10,   0.10,   0.10,   0.10,      
	0.05,   0.03,   0.03,   0.03,   0.03,   0.03,   0.03,   0.03};


float cutsee, cutdeta, cutdphi, cuteop2, cuthoe;

bool goodGlobalMuon(int index) {
	bool good = true;
	if (mus_gfit_chi2->at(index)/mus_gfit_ndof->at(index) > 5.)
		good=false;
	if (abs(mus_d0->at(index))   > 0.25) good=false;
	if (mus_validHits->at(index) < 7)    good=false;
	return good;
}

int electron_category(float eOverP, float fBrem) {

	int cat=6;
	if(eOverP > 0.9*(1-fBrem)) {
		if(fBrem > 0.06) {
			if(eOverP > 1.5) 
				cat=2;
			else if(eOverP > 1.2) 
				cat=1;
			else if(eOverP > 0.8) 
				cat=0;
			else 
				cat=4;
		}
		else if(eOverP > 1.5) 
			cat=5;
		else 
			cat=3;
	} 

	return cat;
}

// type = 0, loose selection
// type = 1, tight selection

bool electron_selection(int index, int type=0) {

	bool selj;
	int cat = electron_category(els_eOverPIn->at(index),els_fBrem->at(index));

//  Chose cuts from barrel or endcap
	float seecor = els_sigmaEtaEta->at(index);
	if(fabs(els_p4->at(index).Eta())<1.479) {
		cutsee = bcutsee[cat+type*8]; 
		cutdeta = bcutdeta[cat+type*8]; 
		cutdphi = bcutdphi[cat+type*8]; 
		cuteop2 = bcuteop2[cat+type*8]; 
		cuthoe = bcuthoe[cat+type*8];
	} else {
		cutsee = ecutsee[cat+type*8]; 
		cutdeta = ecutdeta[cat+type*8]; 
		cutdphi = ecutdphi[cat+type*8]; 
		cuteop2 = ecuteop2[cat+type*8];   
		cuthoe = ecuthoe[cat+type*8];
		seecor = seecor-0.02*(fabs(els_p4->at(index).Eta())-2.3);       // linear correction for sigma-eta-eta in endcap
	}
//  Apply the 5 cuts
	selj = (seecor < cutsee) && (fabs(els_dEtaIn->at(index)) < cutdeta) && (fabs(els_dPhiIn->at(index)) < cutdphi) && (els_eSeedOverPOut->at(index) > cuteop2) && (els_hOverE->at(index) < cuthoe);

	return selj;
}

bool simplified_electron_selection(int index) {

	bool selj;

	selj = (fabs(els_p4->at(index).Eta()) > 1.479 || els_sigmaEtaEta->at(index) < 0.012) && els_sigmaEtaEta->at(index) < 0.03 && 
		fabs(els_dEtaIn->at(index)) < 0.012 && els_hOverE->at(index) < 0.1 && els_eOverPIn->at(index) > 0.9 && 
		sqrt(pow(els_dEtaIn->at(index), 2) + pow(els_dPhiIn->at(index), 2)) < 0.1;

	return selj;
}


bool cut_verysimple(float dEtaIn, float dPhiIn, float hoe, float eOverPout, float see, float eeta) {

	if (fabs(dEtaIn) > 0.01) 
		return false;

	if (hoe > 0.1) 
		return false;

//if (fabs(eeta)<1.5 && sqrt(see)>0.012) 
	if (fabs(eeta)<1.5 && see>0.012) 
		return false;

	if (eOverPout<0.9) 
		return false;

	if (sqrt(dEtaIn*dEtaIn + dPhiIn*dPhiIn)>0.1) 
		return false;

	return true;

}   

bool isDenominatorElectron(int index) {
//
// returns true if input fulfills certain cuts
//

// cut definition
	float et_cut       = 0.;
	float pt_cut       = 15.;
	float eta_cut      = 2.5;
	float tkIso_cut    = 10.; // was 50
	float eOverP_cut   = 999999.99;
	float hOverE_cut   = 0.2;

	bool result = true;

	if ( els_ESc->at(index)      < et_cut )       result = false;
	if ( els_p4->at(index).Pt()  < pt_cut )       result = false;
	if ( std::abs(els_p4->at(index).Eta()) > eta_cut )      result = false;
	if ( els_tkIso->at(index)    > tkIso_cut )    result = false;
	if ( els_eOverPIn->at(index) > eOverP_cut )   result = false;
	if ( els_hOverE->at(index)   > hOverE_cut )   result = false;

	return result;

}

bool isNumeratorElectron(int index, int type=0) { // 0=loose, 1=tight
//
// returns true if input fulfills certain cuts
//

// cut definition
	float et_cut       = 0.;
float pt_cut       = 15;
float eta_cut      = 2.5;
float tkIso_cut    = 5.;
float eOverP_cut   = 999999.99;
//float eOverP_cut   = 3.;
float hOverE_cut   = 0.2;

bool result = true;

if ( els_ESc->at(index)      < et_cut )       result = false;
if ( els_p4->at(index).Pt()  < pt_cut )       result = false;
if ( std::abs(els_p4->at(index).Eta()) > eta_cut )      result = false;
if ( els_tkIso->at(index)    > tkIso_cut )    result = false;
if ( els_eOverPIn->at(index) > eOverP_cut )   result = false;
if ( els_hOverE->at(index)   > hOverE_cut )   result = false;

/*
bool IdCuts = cut_verysimple(els_dEtaIn->at(index),
	els_dPhiIn->at(index), 
	els_hOverE->at(index),
	els_eSeedOverPOut->at(index),
	els_sigmaEtaEta->at(index),
	els_p4->at(index).Eta());
*/

bool IdCuts = electron_selection(index, type);
if (!IdCuts) result = false;

return result;

}

int ScanChain ( TChain* chain, const char* identifier = "histograms", int nEvents = -1) {

	chain->SetBranchAddress("evt_nmus", &evt_nmus);
//   chain->SetBranchAddress("evt_ntrks", &evt_ntrks);
//   chain->SetBranchAddress("evt_run", &evt_run);
//   chain->SetBranchAddress("evt_event", &evt_event);
	chain->SetBranchAddress("evt_nels", &evt_nels);
	chain->SetBranchAddress("evt_njets", &evt_njets);
	chain->SetBranchAddress("evt_ncorrectedjets", &evt_ncorrectedjets);
	chain->SetBranchAddress("mus_validHits", &mus_validHits);
//   chain->SetBranchAddress("mus_lostHits", &mus_lostHits, &b_mus_lostHits);
//   chain->SetBranchAddress("mus_mc_id", &mus_mc_id, &b_mus_mc_id);
//   chain->SetBranchAddress("mus_mc_motherid", &mus_mc_motherid, &b_mus_mc_motherid);
//   chain->SetBranchAddress("mus_nmatches", &mus_nmatches, &b_mus_nmatches);
//   chain->SetBranchAddress("mus_iso03_ntrk", &mus_iso03_ntrk, &b_mus_iso03_ntrk);
//   chain->SetBranchAddress("mus_iso05_ntrk", &mus_iso05_ntrk, &b_mus_iso05_ntrk);
//   chain->SetBranchAddress("mus_pid_TMLastStationLoose", &mus_pid_TMLastStationLoose, &b_mus_pid_TMLastStationLoose);
//   chain->SetBranchAddress("mus_pid_TMLastStationTight", &mus_pid_TMLastStationTight, &b_mus_pid_TMLastStationTight);
//   chain->SetBranchAddress("mus_pid_TM2DCompatibilityLoose", &mus_pid_TM2DCompatibilityLoose, &b_mus_pid_TM2DCompatibilityLoose);
//   chain->SetBranchAddress("mus_pid_TM2DCompatibilityTight", &mus_pid_TM2DCompatibilityTight, &b_mus_pid_TM2DCompatibilityTight);
//   chain->SetBranchAddress("mus_gfit_validHits", &mus_gfit_validHits, &b_mus_gfit_validHits);
//   chain->SetBranchAddress("mus_map_LooseTrackerMuons", &mus_map_LooseTrackerMuons, &b_mus_map_LooseTrackerMuons);
//   chain->SetBranchAddress("mus_map_TightTrackerMuons", &mus_map_TightTrackerMuons, &b_mus_map_TightTrackerMuons);
//   chain->SetBranchAddress("trks_validHits", &trks_validHits, &b_trks_validHits);
//   chain->SetBranchAddress("trks_lostHits", &trks_lostHits, &b_trks_lostHits);
//   chain->SetBranchAddress("trks_mc_id", &trks_mc_id, &b_trks_mc_id);
//   chain->SetBranchAddress("trks_mc_motherid", &trks_mc_motherid, &b_trks_mc_motherid);
//   chain->SetBranchAddress("els_validHits", &els_validHits, &b_els_validHits);
//   chain->SetBranchAddress("els_lostHits", &els_lostHits, &b_els_lostHits);
//   chain->SetBranchAddress("els_mc_id", &els_mc_id, &b_els_mc_id);
//   chain->SetBranchAddress("els_mc_motherid", &els_mc_motherid, &b_els_mc_motherid);
//   chain->SetBranchAddress("els_nSeed", &els_nSeed, &b_els_nSeed);
//   chain->SetBranchAddress("els_class", &els_class, &b_els_class);
//   chain->SetBranchAddress("els_looseId", &els_looseId, &b_els_looseId);
//   chain->SetBranchAddress("els_tightId", &els_tightId, &b_els_tightId);
//   chain->SetBranchAddress("genps_id", &genps_id, &b_genps_id);
	chain->SetBranchAddress("evt_met", &evt_met);
	chain->SetBranchAddress("evt_metPhi", &evt_metPhi);
//   chain->SetBranchAddress("evt_sumEt", &evt_sumEt, &b_evt_sumEt);
//   chain->SetBranchAddress("evt_metSig", &evt_metSig, &b_evt_metSig);
	chain->SetBranchAddress("evt_met_jetcorr", &evt_met_jetcorr);
	chain->SetBranchAddress("metphi_jetcorr", &metphi_jetcorr);
//   chain->SetBranchAddress("gen_met", &gen_met, &b_gen_met);
//   chain->SetBranchAddress("gen_metPhi", &gen_metPhi, &b_gen_metPhi);
	chain->SetBranchAddress("mus_d0", &mus_d0);
//   chain->SetBranchAddress("mus_z0", &mus_z0, &b_mus_z0);
//   chain->SetBranchAddress("mus_vertexphi", &mus_vertexphi, &b_mus_vertexphi);
//   chain->SetBranchAddress("mus_chi2", &mus_chi2, &b_mus_chi2);
//   chain->SetBranchAddress("mus_ndof", &mus_ndof, &b_mus_ndof);
//   chain->SetBranchAddress("mus_d0Err", &mus_d0Err, &b_mus_d0Err);
//   chain->SetBranchAddress("mus_z0Err", &mus_z0Err, &b_mus_z0Err);
//   chain->SetBranchAddress("mus_ptErr", &mus_ptErr, &b_mus_ptErr);
//   chain->SetBranchAddress("mus_etaErr", &mus_etaErr, &b_mus_etaErr);
//   chain->SetBranchAddress("mus_phiErr", &mus_phiErr, &b_mus_phiErr);
//   chain->SetBranchAddress("mus_e_em", &mus_e_em, &b_mus_e_em);
//   chain->SetBranchAddress("mus_e_had", &mus_e_had, &b_mus_e_had);
//   chain->SetBranchAddress("mus_e_ho", &mus_e_ho, &b_mus_e_ho);
//   chain->SetBranchAddress("mus_e_emS9", &mus_e_emS9, &b_mus_e_emS9);
//   chain->SetBranchAddress("mus_e_hadS9", &mus_e_hadS9, &b_mus_e_hadS9);
//   chain->SetBranchAddress("mus_e_hoS9", &mus_e_hoS9, &b_mus_e_hoS9);
	chain->SetBranchAddress("mus_iso03_sumPt", &mus_iso03_sumPt);
//   chain->SetBranchAddress("mus_iso03_emEt", &mus_iso03_emEt, &b_mus_iso03_emEt);
//   chain->SetBranchAddress("mus_iso03_hadEt", &mus_iso03_hadEt, &b_mus_iso03_hadEt);
//   chain->SetBranchAddress("mus_iso03_hoEt", &mus_iso03_hoEt, &b_mus_iso03_hoEt);
//   chain->SetBranchAddress("mus_iso05_sumPt", &mus_iso05_sumPt, &b_mus_iso05_sumPt);
//   chain->SetBranchAddress("mus_iso05_emEt", &mus_iso05_emEt, &b_mus_iso05_emEt);
//   chain->SetBranchAddress("mus_iso05_hadEt", &mus_iso05_hadEt, &b_mus_iso05_hadEt);
//   chain->SetBranchAddress("mus_iso05_hoEt", &mus_iso05_hoEt, &b_mus_iso05_hoEt);
	chain->SetBranchAddress("mus_gfit_chi2", &mus_gfit_chi2);
	chain->SetBranchAddress("mus_gfit_ndof", &mus_gfit_ndof);
//   chain->SetBranchAddress("trks_d0", &trks_d0, &b_trks_d0);
//   chain->SetBranchAddress("trks_z0", &trks_z0, &b_trks_z0);
//   chain->SetBranchAddress("trks_vertexphi", &trks_vertexphi, &b_trks_vertexphi);
//   chain->SetBranchAddress("trks_chi2", &trks_chi2, &b_trks_chi2);
//   chain->SetBranchAddress("trks_ndof", &trks_ndof, &b_trks_ndof);
//   chain->SetBranchAddress("trks_d0Err", &trks_d0Err, &b_trks_d0Err);
//   chain->SetBranchAddress("trks_z0Err", &trks_z0Err, &b_trks_z0Err);
//   chain->SetBranchAddress("trks_ptErr", &trks_ptErr, &b_trks_ptErr);
//   chain->SetBranchAddress("trks_etaErr", &trks_etaErr, &b_trks_etaErr);
//   chain->SetBranchAddress("trks_phiErr", &trks_phiErr, &b_trks_phiErr);
//   chain->SetBranchAddress("els_d0", &els_d0, &b_els_d0);
//   chain->SetBranchAddress("els_z0", &els_z0, &b_els_z0);
//   chain->SetBranchAddress("els_vertexphi", &els_vertexphi, &b_els_vertexphi);
//   chain->SetBranchAddress("els_chi2", &els_chi2, &b_els_chi2);
//   chain->SetBranchAddress("els_ndof", &els_ndof, &b_els_ndof);
//   chain->SetBranchAddress("els_d0Err", &els_d0Err, &b_els_d0Err);
//   chain->SetBranchAddress("els_z0Err", &els_z0Err, &b_els_z0Err);
//   chain->SetBranchAddress("els_ptErr", &els_ptErr, &b_els_ptErr);
//   chain->SetBranchAddress("els_etaErr", &els_etaErr, &b_els_etaErr);
//   chain->SetBranchAddress("els_phiErr", &els_phiErr, &b_els_phiErr);
	chain->SetBranchAddress("els_hOverE", &els_hOverE);
	chain->SetBranchAddress("els_eOverPIn", &els_eOverPIn);
	chain->SetBranchAddress("els_eSeedOverPOut", &els_eSeedOverPOut);
	chain->SetBranchAddress("els_fBrem", &els_fBrem);
	chain->SetBranchAddress("els_dEtaIn", &els_dEtaIn);
//   chain->SetBranchAddress("els_dEtaOut", &els_dEtaOut, &b_els_dEtaOut);
	chain->SetBranchAddress("els_dPhiIn", &els_dPhiIn);
//   chain->SetBranchAddress("els_dPhiOut", &els_dPhiOut, &b_els_dPhiOut);
	chain->SetBranchAddress("els_ESc", &els_ESc);
//   chain->SetBranchAddress("els_ESc_raw", &els_ESc_raw, &b_els_ESc_raw);
//   chain->SetBranchAddress("els_e3x3", &els_e3x3, &b_els_e3x3);
//   chain->SetBranchAddress("els_e5x5", &els_e5x5, &b_els_e5x5);
//   chain->SetBranchAddress("els_ESeed", &els_ESeed, &b_els_ESeed);
	chain->SetBranchAddress("els_sigmaPhiPhi", &els_sigmaPhiPhi);
	chain->SetBranchAddress("els_sigmaEtaEta", &els_sigmaEtaEta);
	chain->SetBranchAddress("els_tkIso", &els_tkIso);
//   chain->SetBranchAddress("genjs_emEnergy", &genjs_emEnergy);
//   chain->SetBranchAddress("genjs_hadEnergy", &genjs_hadEnergy);
//   chain->SetBranchAddress("genjs_invEnergy", &genjs_invEnergy, &b_genjs_invEnergy);
//   chain->SetBranchAddress("genjs_otherEnergy", &genjs_otherEnergy, &b_genjs_otherEnergy);
//   chain->SetBranchAddress("jets_emFrac", &jets_emFrac);
//   chain->SetBranchAddress("jets_chFrac", &jets_chFrac);
//   chain->SetBranchAddress("jets_mc_emEnergy", &jets_mc_emEnergy);
//   chain->SetBranchAddress("jets_mc_hadEnergy", &jets_mc_hadEnergy);
//   chain->SetBranchAddress("jets_mc_invEnergy", &jets_mc_invEnergy);
//   chain->SetBranchAddress("jets_mc_otherEnergy", &jets_mc_otherEnergy);
//   chain->SetBranchAddress("jets_cor", &jets_cor);
//   chain->SetBranchAddress("corrected_jets_emFrac", &corrected_jets_emFrac);
//   chain->SetBranchAddress("corrected_jets_chFrac", &corrected_jets_chFrac);
//   chain->SetBranchAddress("corrected_jets_mc_emEnergy", &corrected_jets_mc_emEnergy);
//   chain->SetBranchAddress("corrected_jets_mc_hadEnergy", &corrected_jets_mc_hadEnergy);
//   chain->SetBranchAddress("corrected_jets_mc_invEnergy", &corrected_jets_mc_invEnergy);
//   chain->SetBranchAddress("corrected_jets_mc_otherEnergy", &corrected_jets_mc_otherEnergy);
//   chain->SetBranchAddress("corrected_jets_cor", &corrected_jets_cor);
	chain->SetBranchAddress("mus_p4", &mus_p4);
//   chain->SetBranchAddress("mus_trk_p4", &mus_trk_p4);
//   chain->SetBranchAddress("mus_mc_p4", &mus_mc_p4);
//   chain->SetBranchAddress("trks_p4", &trks_p4);
//   chain->SetBranchAddress("trks_trk_p4", &trks_trk_p4);
//   chain->SetBranchAddress("trks_mc_p4", &trks_mc_p4);
	chain->SetBranchAddress("els_p4", &els_p4);
//   chain->SetBranchAddress("els_trk_p4", &els_trk_p4);
//   chain->SetBranchAddress("els_mc_p4", &els_mc_p4);
//   chain->SetBranchAddress("genps_p4", &genps_p4);
//   chain->SetBranchAddress("genjs_p4", &genjs_p4);
	chain->SetBranchAddress("jets_p4", &jets_p4);
	chain->SetBranchAddress("jets_mc_p4", &jets_mc_p4);
//   chain->SetBranchAddress("jets_mc_gp_p4", &jets_mc_gp_p4);
	chain->SetBranchAddress("corrected_jets_p4", &corrected_jets_p4);
	chain->SetBranchAddress("corrected_jets_mc_p4", &corrected_jets_mc_p4);
//   chain->SetBranchAddress("corrected_jets_mc_gp_p4", &corrected_jets_mc_gp_p4);
//   chain->SetBranchAddress("evt_lego", &evt_lego);

	vector<float> binsPt;
	binsPt.push_back(0.);
	binsPt.push_back(10.);
	binsPt.push_back(20.);
	binsPt.push_back(30.);
	binsPt.push_back(40.);
	binsPt.push_back(50.);
	binsPt.push_back(65.);
	binsPt.push_back(80.);
	binsPt.push_back(95.);
	binsPt.push_back(115.);
	binsPt.push_back(150.);
	vector<float> binsEta;
	binsEta.push_back(-2.5);
	binsEta.push_back(-2.1);
	binsEta.push_back(-1.9);
	binsEta.push_back(-1.7);
	binsEta.push_back(-1.5);
	binsEta.push_back(-1.3);
	binsEta.push_back(-1.1);
	binsEta.push_back(-0.9);
	binsEta.push_back(-0.7);
	binsEta.push_back(-0.5);
	binsEta.push_back(-0.3);
	binsEta.push_back(-0.1);
	binsEta.push_back(0.1);
	binsEta.push_back(0.3);
	binsEta.push_back(0.5);
	binsEta.push_back(0.7);
	binsEta.push_back(0.9);
	binsEta.push_back(1.1);
	binsEta.push_back(1.3);
	binsEta.push_back(1.5);
	binsEta.push_back(1.7);
	binsEta.push_back(1.9);
	binsEta.push_back(2.1);
	binsEta.push_back(2.5);

	TH1F *pt_num_ell  = createVarHistogram("pt_num_ell","p_{T} Numerator",binsPt,"p_{T} [GeV]","p_{T} Loose Electron Numerator");
	TH1F *pt_num_elt  = createVarHistogram("pt_num_elt","p_{T} Numerator",binsPt,"p_{T} [GeV]","p_{T} Tight Electron Numerator");
	TH1F *pt_den_ele  = createVarHistogram("pt_den_ele","p_{T} Denominator",binsPt,"p_{T} [GeV]","p_{T} Electron Denominator");

	TH1F *eta_num_ell = createVarHistogram("eta_num_ell","#eta Numerator",binsEta,"#eta","#eta Loose Electron Numerator");
	TH1F *eta_num_elt = createVarHistogram("eta_num_elt","#eta Numerator",binsEta,"#eta","#eta Tight Electron Numerator");
	TH1F *eta_den_ele = createVarHistogram("eta_den_ele","#eta Denominator",binsEta,"#eta","#eta Electron Denominator");

	TH2F *num_ell = createVar2DHistogram("num_ell","Numerator",binsEta,binsPt,"#eta","p_{T} [GeV]","Loose Electron Numerator");
	TH2F *num_elt = createVar2DHistogram("num_elt","Numerator",binsEta,binsPt,"#eta","p_{T} [GeV]","Tight Electron Numerator");
	TH2F *den_ele = createVar2DHistogram("den_ele","Denominator",binsEta,binsPt,"#eta","p_{T} [GeV]","Electron Denominator");

	TH1F *pt_num_wo_leading_ell  = createVarHistogram("pt_num_wo_leading_ell","p_{T} Numerator without leading Jet",binsPt,"p_{T} [GeV]","p_{T} Loose Electron Numerator");
	TH1F *pt_num_wo_leading_elt  = createVarHistogram("pt_num_wo_leading_elt","p_{T} Numerator without leading Jet",binsPt,"p_{T} [GeV]","p_{T} Tight Electron Numerator");
	TH1F *pt_den_wo_leading_ele  = createVarHistogram("pt_den_wo_leading_ele","p_{T} Denominator without leading Jet",binsPt,"p_{T} [GeV]","p_{T} Electron Denominator");

	TH1F *eta_num_wo_leading_ell = createVarHistogram("eta_num_wo_leading_ell","#eta Numerator without leading Jet",binsEta,"#eta","#eta Loose Electron Numerator");
	TH1F *eta_num_wo_leading_elt = createVarHistogram("eta_num_wo_leading_elt","#eta Numerator without leading Jet",binsEta,"#eta","#eta Tight Electron Numerator");
	TH1F *eta_den_wo_leading_ele = createVarHistogram("eta_den_wo_leading_ele","#eta Denominator without leading Jet",binsEta,"#eta","#eta Electron Denominator");

	TH2F *num_wo_leading_ell = createVar2DHistogram("num_wo_leading_ell","Numerator without leading Jet",binsEta,binsPt,"#eta","p_{T} [GeV]","Loose Electron Numerator");
	TH2F *num_wo_leading_elt = createVar2DHistogram("num_wo_leading_elt","Numerator without leading Jet",binsEta,binsPt,"#eta","p_{T} [GeV]","Tight Electron Numerator");
	TH2F *den_wo_leading_ele = createVar2DHistogram("den_wo_leading_ele","Denominator without leading Jet",binsEta,binsPt,"#eta","p_{T} [GeV]","Electron Denominator");

	TH1F *pt_num_wo_second_leading_ell  = createVarHistogram("pt_num_wo_second_leading_ell","p_{T} Numerator without second leading Jet",binsPt,"p_{T} [GeV]","p_{T} Loose Electron Numerator");
	TH1F *pt_num_wo_second_leading_elt  = createVarHistogram("pt_num_wo_second_leading_elt","p_{T} Numerator without second leading Jet",binsPt,"p_{T} [GeV]","p_{T} Tight Electron Numerator");
	TH1F *pt_den_wo_second_leading_ele  = createVarHistogram("pt_den_wo_second_leading_ele","p_{T} Denominator without second leading Jet",binsPt,"p_{T} [GeV]","p_{T} Electron Denominator");

	TH1F *eta_num_wo_second_leading_ell = createVarHistogram("eta_num_wo_second_leading_ell","#eta Numerator without second leading Jet",binsEta,"#eta","#eta Loose Electron Numerator");
	TH1F *eta_num_wo_second_leading_elt = createVarHistogram("eta_num_wo_second_leading_elt","#eta Numerator without second leading Jet",binsEta,"#eta","#eta Tight Electron Numerator");
	TH1F *eta_den_wo_second_leading_ele = createVarHistogram("eta_den_wo_second_leading_ele","#eta Denominator without second leading Jet",binsEta,"#eta","#eta Electron Denominator");

	TH2F *num_wo_second_leading_ell = createVar2DHistogram("num_wo_second_leading_ell","Numerator without second leading Jet",binsEta,binsPt,"#eta","p_{T} [GeV]","Loose Electron Numerator");
	TH2F *num_wo_second_leading_elt = createVar2DHistogram("num_wo_second_leading_elt","Numerator without second leading Jet",binsEta,binsPt,"#eta","p_{T} [GeV]","Tight Electron Numerator");
	TH2F *den_wo_second_leading_ele = createVar2DHistogram("den_wo_second_leading_ele","Denominator without second leading Jet",binsEta,binsPt,"#eta","p_{T} [GeV]","Electron Denominator");

	TH1F *pt_num_muo  = createVarHistogram("pt_num_muo","p_{T} Numerator",binsPt,"p_{T} [GeV]","p_{T} Muon Numerator");
	TH1F *pt_den_muo  = createVarHistogram("pt_den_muo","p_{T} Denominator",binsPt,"p_{T} [GeV]","p_{T} Muon Denominator");
	TH1F *eta_num_muo = createVarHistogram("eta_num_muo","#eta Numerator",binsEta,"#eta","#eta Muon Numerator");
	TH1F *eta_den_muo = createVarHistogram("eta_den_muo","#eta Denominator",binsEta,"#eta","#eta Muon Denominator");

	TH1F *muo_n   = createHistogram("muo_n","number of muons per event",15,0.,15.,"n_{e}","Events");
	TH1F *ele_n   = createHistogram("ele_n","number of electrons per event",15,0.,15.,"n_{e}","Events");

	unsigned int nJetsBins = 50;
	float        nJetsLow  = 0.;
	float        nJetsHigh = 50.;

	TH1F *njets = createHistogram("njets","Number of Jets",nJetsBins,nJetsLow,nJetsHigh,"n_{Jets}","Events");
	TH1F *njets_corr = createHistogram("njets_corr","Number of Corrected Jets",nJetsBins,nJetsLow,nJetsHigh,"n_{Jets}","Events");

	unsigned int jetPtBins = 4000;
	float        jetPtLow  = 0.;
	float        jetPtHigh = 4000.;

	TH1F *jetpt = createHistogram("jetpt","p_{T} of Jets",jetPtBins,jetPtLow,jetPtHigh,"p_{T} [GeV]","Events");
	TH1F *jetpt_mc = createHistogram("jetpt_mc","p_{T} of Jets matched to genJet",jetPtBins,jetPtLow,jetPtHigh,"p_{T} [GeV]","Events");
	TH1F *jetpt_corr = createHistogram("jetpt_corr","p_{T} of Corrected Jets",jetPtBins,jetPtLow,jetPtHigh,"p_{T} [GeV]","Events");
	TH1F *jetpt_corr_mc = createHistogram("jetpt_corr_mc","p_{T} of Corrected Jets matched to GenJet",jetPtBins,jetPtLow,jetPtHigh,"p_{T} [GeV]","Events");

	unsigned int jetEtaBins = 30;
	float        jetEtaLow  = -3.;
	float        jetEtaHigh =  3.;

	TH1F *jeteta = createHistogram("jeteta","#eta of Jets",jetEtaBins,jetEtaLow,jetEtaHigh,"#eta","Events");
	TH1F *jeteta_mc = createHistogram("jeteta_mc","#eta of Jets matched to genJet",jetEtaBins,jetEtaLow,jetEtaHigh,"#eta","Events");
	TH1F *jeteta_corr = createHistogram("jeteta_corr","#eta of Corrected Jets",jetEtaBins,jetEtaLow,jetEtaHigh,"#eta","Events");
	TH1F *jeteta_corr_mc = createHistogram("jeteta_corr_mc","#eta of Corrected Jets matched to GenJet",jetEtaBins,jetEtaLow,jetEtaHigh,"#eta","Events");

	unsigned int deltaRBins = 500;
	float        deltaRLow  = 0.;
	float        deltaRHigh = 10.;

	TH1F *deltaR = createHistogram("deltaR","deltaR between jet and denominator",deltaRBins,deltaRLow,deltaRHigh,"#Delta R","Events");

	unsigned int tkIsoBins = 400;
	float        tkIsoLow  = 0.;
	float        tkIsoHigh = 200.;

	TH1F *tkIso = createHistogram("tkIso","track isolation",tkIsoBins,tkIsoLow,tkIsoHigh,"tk_{iso}","Events");
	TH1F *tkIso_uncut = createHistogram("tkIso_uncut","uncut track isolation",tkIsoBins,tkIsoLow,tkIsoHigh,"tk_{iso}","Events");

	unsigned int EOverpBins = 30;
	float        EOverpLow  = -1.5;
	float        EOverpHigh = 1.5;

	TH1F *EOverp = createHistogram("EOverp","E/p",EOverpBins,EOverpLow,EOverpHigh,"E/p","Events");
	TH1F *EOverp_uncut = createHistogram("EOverp_uncut","uncut E/p",EOverpBins,EOverpLow,EOverpHigh,"E/p","Events");

	unsigned int HOverEBins = 300;
	float        HOverELow  = -5.;
	float        HOverEHigh = 5.;

	TH1F *HOverE = createHistogram("HOverE","H/E",HOverEBins,HOverELow,HOverEHigh,"H/E","Events");
	TH1F *HOverE_uncut = createHistogram("HOverE_uncut","uncut H/E",HOverEBins,HOverELow,HOverEHigh,"H/E","Events");

	// cuts
	float deltaRCut = 0.2;

	if ( nEvents == -1 ) {
		nEvents = chain->GetEntries();
	}

	//Event Loop
	for( int i = 0; i < nEvents; i++) {

		chain->GetEntry(i);

		if ( i%1000 == 0 ) {
			cout << "Event: " << i << endl;
		}

		ele_n->Fill(evt_nels);
		muo_n->Fill(evt_nmus);
		njets->Fill(evt_njets);
		njets_corr->Fill(evt_ncorrectedjets);

	// sort jets in decreasing pt
		sort(corrected_jets_p4->begin(),corrected_jets_p4->end(),SortTLorentzVectorByPt());
		sort(corrected_jets_p4->begin(),corrected_jets_p4->end(),SortTLorentzVectorByPt());

		for ( unsigned int jet_counter = 0;
		jet_counter < (unsigned int)evt_njets;
		++jet_counter ) {
			jetpt->Fill(jets_p4->at(jet_counter).Pt());
			jeteta->Fill(jets_p4->at(jet_counter).Eta());
			jetpt_mc->Fill(jets_mc_p4->at(jet_counter).Pt());
			jeteta_mc->Fill(jets_mc_p4->at(jet_counter).Eta());

		}

		for ( unsigned int jet_corr_counter = 0;
		jet_corr_counter < (unsigned int)evt_ncorrectedjets;
		++jet_corr_counter ) {
			jetpt_corr->Fill(corrected_jets_p4->at(jet_corr_counter).Pt());
			jeteta_corr->Fill(corrected_jets_p4->at(jet_corr_counter).Eta());
			jetpt_corr_mc->Fill(corrected_jets_mc_p4->at(jet_corr_counter).Pt());
			jeteta_corr_mc->Fill(corrected_jets_mc_p4->at(jet_corr_counter).Eta());
		}

		for ( unsigned int electron_counter = 0;
		electron_counter < (unsigned int)evt_nels;
		++electron_counter ) {

			tkIso_uncut->Fill(els_tkIso->at(electron_counter));
			EOverp_uncut->Fill(els_eOverPIn->at(electron_counter));
			HOverE_uncut->Fill(els_hOverE->at(electron_counter));

			if ( isDenominatorElectron(electron_counter)){
				float pt = els_p4->at(electron_counter).Pt();
				if (pt >= 150.) pt = 149.;
				pt_den_ele->Fill(pt);
				eta_den_ele->Fill(els_p4->at(electron_counter).Eta());
				den_ele->Fill(els_p4->at(electron_counter).Eta(),pt);
				for ( unsigned int jet_counter = 0;
				jet_counter < (unsigned int)evt_njets;
				++jet_counter ) {
					deltaR->Fill(CalculateDeltaR(els_p4->at(electron_counter),jets_p4->at(jet_counter)));
				}
				tkIso->Fill(els_tkIso->at(electron_counter));
				EOverp->Fill(els_eOverPIn->at(electron_counter));
				HOverE->Fill(els_hOverE->at(electron_counter));
			}

	// loose electrons
			if (isNumeratorElectron(electron_counter,0)){ 
				// 0=loose, 1=tight
				float pt = els_p4->at(electron_counter).Pt();
				if (pt >= 150.) pt = 149.;
				pt_num_ell->Fill(pt);
				eta_num_ell->Fill(els_p4->at(electron_counter).Eta());
				num_ell->Fill(els_p4->at(electron_counter).Eta(),pt);
			}

	// tight electrons
			if ( isNumeratorElectron(electron_counter,1)){ 
			// 0=loose, 1=tight
				float pt = els_p4->at(electron_counter).Pt();
				if (pt >= 150.) pt = 149.;
				pt_num_elt->Fill(pt);
				eta_num_elt->Fill(els_p4->at(electron_counter).Eta());
				num_elt->Fill(els_p4->at(electron_counter).Eta(),pt);
			}

	// exclude leading jet
			if ( CalculateDeltaR(els_p4->at(electron_counter),jets_p4->at(0)) >= deltaRCut ) {

				if ( isDenominatorElectron(electron_counter)){
					float pt = els_p4->at(electron_counter).Pt();
					if (pt >= 150.) pt = 149.;
					pt_den_wo_leading_ele->Fill(pt);
					eta_den_wo_leading_ele->Fill(els_p4->at(electron_counter).Eta());
					den_wo_leading_ele->Fill(els_p4->at(electron_counter).Eta(),pt);
				}

		// loose electrons
				if (isNumeratorElectron(electron_counter,0)){ 
			// 0=loose, 1=tight
					float pt = els_p4->at(electron_counter).Pt();
					if (pt >= 150.) pt = 149.;
					pt_num_wo_leading_ell->Fill(pt);
					eta_num_wo_leading_ell->Fill(els_p4->at(electron_counter).Eta());
					num_wo_leading_ell->Fill(els_p4->at(electron_counter).Eta(),pt);
				}

	// tight electrons
				if ( isNumeratorElectron(electron_counter,1)) { 
		// 0=loose, 1=tight
					float pt = els_p4->at(electron_counter).Pt();
					if (pt >= 150.) pt = 149.;
					pt_num_wo_leading_elt->Fill(pt);
					eta_num_wo_leading_elt->Fill(els_p4->at(electron_counter).Eta());
					num_wo_leading_elt->Fill(els_p4->at(electron_counter).Eta(),pt);
				}

			}

	// exclude leading and second leading jet
			if ( evt_njets > 1 ) {

				if ( (CalculateDeltaR(els_p4->at(electron_counter),jets_p4->at(0)) >= deltaRCut) &&
				(CalculateDeltaR(els_p4->at(electron_counter),jets_p4->at(1)) >= deltaRCut) ) {

					if ( isDenominatorElectron(electron_counter)){
						float pt = els_p4->at(electron_counter).Pt();
						if (pt >= 150.) pt = 149.;
						pt_den_wo_second_leading_ele->Fill(pt);
						eta_den_wo_second_leading_ele->Fill(els_p4->at(electron_counter).Eta());
						den_wo_second_leading_ele->Fill(els_p4->at(electron_counter).Eta(),pt);
					}

	// loose electrons
					if (isNumeratorElectron(electron_counter,0)){ 
			// 0=loose, 1=tight
						float pt = els_p4->at(electron_counter).Pt();
						if (pt >= 150.) pt = 149.;
						pt_num_wo_second_leading_ell->Fill(pt);
						eta_num_wo_second_leading_ell->Fill(els_p4->at(electron_counter).Eta());
						num_wo_second_leading_ell->Fill(els_p4->at(electron_counter).Eta(),pt);
					}

	// tight electrons
					if ( isNumeratorElectron(electron_counter,1)){ 
		// 0=loose, 1=tight
						float pt = els_p4->at(electron_counter).Pt();
						if (pt >= 150.) pt = 149.;
						pt_num_wo_second_leading_elt->Fill(pt);
						eta_num_wo_second_leading_elt->Fill(els_p4->at(electron_counter).Eta());
						num_wo_second_leading_elt->Fill(els_p4->at(electron_counter).Eta(),pt);
					}
				}
			}
		}
// end loop over electrons

		float muon_pt_cut = 15.0;
		float muon_tkIso_cut = 5.0;
		for ( unsigned int muon_counter = 0;
		muon_counter < (unsigned int)evt_nmus;
		++muon_counter ) {

			if (mus_p4->at(muon_counter).Pt() < muon_pt_cut) continue;
	// For now, all muons above pt cut are denominator muons
			pt_den_muo->Fill(mus_p4->at(muon_counter).Pt());
			eta_den_muo->Fill(mus_p4->at(muon_counter).Eta());

	// good muons
			if (goodGlobalMuon(muon_counter)&&(mus_iso03_sumPt->at(muon_counter)<muon_tkIso_cut)){
				pt_num_muo->Fill(mus_p4->at(muon_counter).Pt());
				eta_num_muo->Fill(mus_p4->at(muon_counter).Eta());
			}
		}// end loop over muons

	}

	TString filename = identifier;
	filename.Append(".root");
	TFile *file = new TFile(filename,"RECREATE");

	pt_num_ell->SetDirectory(file);
	pt_num_elt->SetDirectory(file);
	pt_den_ele->SetDirectory(file);

	eta_num_ell->SetDirectory(file);
	eta_num_elt->SetDirectory(file);
	eta_den_ele->SetDirectory(file);

	num_ell->SetDirectory(file);
	num_elt->SetDirectory(file);
	den_ele->SetDirectory(file);

	pt_num_wo_leading_ell->SetDirectory(file);
	pt_num_wo_leading_elt->SetDirectory(file);
	pt_den_wo_leading_ele->SetDirectory(file);

	eta_num_wo_leading_ell->SetDirectory(file);
	eta_num_wo_leading_elt->SetDirectory(file);
	eta_den_wo_leading_ele->SetDirectory(file);

	num_wo_leading_ell->SetDirectory(file);
	num_wo_leading_elt->SetDirectory(file);
	den_wo_leading_ele->SetDirectory(file);

	pt_num_wo_second_leading_ell->SetDirectory(file);
	pt_num_wo_second_leading_elt->SetDirectory(file);
	pt_den_wo_second_leading_ele->SetDirectory(file);

	eta_num_wo_second_leading_ell->SetDirectory(file);
	eta_num_wo_second_leading_elt->SetDirectory(file);
	eta_den_wo_second_leading_ele->SetDirectory(file);

	num_wo_second_leading_ell->SetDirectory(file);
	num_wo_second_leading_elt->SetDirectory(file);
	den_wo_second_leading_ele->SetDirectory(file);

	pt_num_muo->SetDirectory(file);
	pt_den_muo->SetDirectory(file);
	eta_num_muo->SetDirectory(file);
	eta_den_muo->SetDirectory(file);

	ele_n->SetDirectory(file);
	muo_n->SetDirectory(file);

	njets->SetDirectory(file);
	njets_corr->SetDirectory(file);

	jetpt->SetDirectory(file);
	jeteta->SetDirectory(file);
	jetpt_mc->SetDirectory(file);
	jeteta_mc->SetDirectory(file);

	jetpt_corr->SetDirectory(file);
	jeteta_corr->SetDirectory(file);
	jetpt_corr_mc->SetDirectory(file);
	jeteta_corr_mc->SetDirectory(file);

	deltaR->SetDirectory(file);


	tkIso_uncut->SetDirectory(file);
	EOverp_uncut->SetDirectory(file);
	HOverE_uncut->SetDirectory(file);

	tkIso->SetDirectory(file);
	EOverp->SetDirectory(file);
	HOverE->SetDirectory(file);


	cout<<" # Denom Electrons: " << eta_den_ele->GetEntries() << endl;
	cout<<" # Loose Electrons: " << eta_num_ell->GetEntries() << endl;
	cout<<" # Tight Electrons: " << eta_num_elt->GetEntries() << endl;
	cout<<" Loose Fraction: " << eta_num_ell->GetEntries()/eta_den_ele->GetEntries() << endl;
	cout<<" Tight Fraction: " << eta_num_elt->GetEntries()/eta_den_ele->GetEntries() << endl;

	cout<<" # Denom Electrons without leading jet: " << eta_den_wo_leading_ele->GetEntries() << endl;
	cout<<" # Loose Electrons without leading jet: " << eta_num_wo_leading_ell->GetEntries() << endl;
	cout<<" # Tight Electrons without leading jet: " << eta_num_wo_leading_elt->GetEntries() << endl;
	cout<<" Loose Fraction without leading jet: " << eta_num_wo_leading_ell->GetEntries()/eta_den_wo_leading_ele->GetEntries() << endl;
	cout<<" Tight Fraction without leading jet: " << eta_num_wo_leading_elt->GetEntries()/eta_den_wo_leading_ele->GetEntries() << endl;

	cout<<" # Denom Electrons without second leading jet: " << eta_den_wo_second_leading_ele->GetEntries() << endl;
	cout<<" # Loose Electrons without second leading jet: " << eta_num_wo_second_leading_ell->GetEntries() << endl;
	cout<<" # Tight Electrons without second leading jet: " << eta_num_wo_second_leading_elt->GetEntries() << endl;
	cout<<" Loose Fraction without second leading jet: " << eta_num_wo_second_leading_ell->GetEntries()/eta_den_wo_second_leading_ele->GetEntries() << endl;
	cout<<" Tight Fraction without second leading jet: " << eta_num_wo_second_leading_elt->GetEntries()/eta_den_wo_second_leading_ele->GetEntries() << endl;

	cout<<"\n\n # Denom Muons: " << eta_den_muo->GetEntries() << endl;
	cout<<" # Good  Muons: " << eta_num_muo->GetEntries() << endl;
	cout<<" Good Muon Fraction: " << eta_num_muo->GetEntries()/eta_den_muo->GetEntries() << endl;


	file->Write();

	return 0;
}
