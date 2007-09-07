#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"
#include "Math/LorentzVector.h"
#include "TMath.h"
#include <vector>
#include "CMS1.h"
#include <algorithm>
#include "selections.h"


// this is Jake's magic to sort jets by Pt
Bool_t comparePt(ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > lv1, 
                 ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > lv2) {
   return lv1.pt() > lv2.pt();
}


// specDY is a flag to handle the DY in a special way.
//        Default: do nothing
//        specDY=0   only process DY->ee (as determined from doc lines)
//        specDY=1   only process DY->mm (as determined from doc lines)
//        specDY=2   only process DY->tt (as determined from doc lines)
int ScanTree ( TTree* tree, char * prefix="", int specDY=-1) {

	tree->SetBranchAddress("evt_nmus", &evt_nmus);
	tree->SetBranchAddress("evt_ntrks", &evt_ntrks);
	tree->SetBranchAddress("evt_run", &evt_run);
	tree->SetBranchAddress("evt_event", &evt_event);
	tree->SetBranchAddress("evt_nels", &evt_nels);
	tree->SetBranchAddress("hyp_lt_validHits", &hyp_lt_validHits);
	tree->SetBranchAddress("hyp_lt_lostHits", &hyp_lt_lostHits);
	tree->SetBranchAddress("hyp_lt_mc_id", &hyp_lt_mc_id);
	tree->SetBranchAddress("hyp_lt_mc_motherid", &hyp_lt_mc_motherid);
	tree->SetBranchAddress("hyp_ll_validHits", &hyp_ll_validHits);
	tree->SetBranchAddress("hyp_ll_lostHits", &hyp_ll_lostHits);
	tree->SetBranchAddress("hyp_ll_mc_id", &hyp_ll_mc_id);
	tree->SetBranchAddress("hyp_ll_mc_motherid", &hyp_ll_mc_motherid);
	tree->SetBranchAddress("hyp_type", &hyp_type);
	tree->SetBranchAddress("hyp_njets", &hyp_njets);
	tree->SetBranchAddress("hyp_nojets", &hyp_nojets);
	tree->SetBranchAddress("hyp_lt_index", &hyp_lt_index);
	tree->SetBranchAddress("hyp_ll_index", &hyp_ll_index);
	tree->SetBranchAddress("hyp_lt_id", &hyp_lt_id);
	tree->SetBranchAddress("hyp_ll_id", &hyp_ll_id);
	tree->SetBranchAddress("evt_nCand", &evt_nCand);
	tree->SetBranchAddress("mus_validHits", &mus_validHits);
	tree->SetBranchAddress("mus_lostHits", &mus_lostHits);
	tree->SetBranchAddress("mus_mc_id", &mus_mc_id);
	tree->SetBranchAddress("mus_mc_motherid", &mus_mc_motherid);
	tree->SetBranchAddress("mus_nmatches", &mus_nmatches);
	tree->SetBranchAddress("mus_iso03_ntrk", &mus_iso03_ntrk);
	tree->SetBranchAddress("mus_iso05_ntrk", &mus_iso05_ntrk);
	tree->SetBranchAddress("mus_pid_TMLastStationLoose", &mus_pid_TMLastStationLoose);
	tree->SetBranchAddress("mus_pid_TMLastStationTight", &mus_pid_TMLastStationTight);
	tree->SetBranchAddress("mus_pid_TM2DCompatibilityLoose", &mus_pid_TM2DCompatibilityLoose);
	tree->SetBranchAddress("mus_pid_TM2DCompatibilityTight", &mus_pid_TM2DCompatibilityTight);
	tree->SetBranchAddress("mus_gfit_validHits", &mus_gfit_validHits);
	tree->SetBranchAddress("mus_map_LooseTrackerMuons", &mus_map_LooseTrackerMuons);
	tree->SetBranchAddress("mus_map_TightTrackerMuons", &mus_map_TightTrackerMuons);
	tree->SetBranchAddress("trks_validHits", &trks_validHits);
	tree->SetBranchAddress("trks_lostHits", &trks_lostHits);
	tree->SetBranchAddress("trks_mc_id", &trks_mc_id);
	tree->SetBranchAddress("trks_mc_motherid", &trks_mc_motherid);
	tree->SetBranchAddress("els_validHits", &els_validHits);
	tree->SetBranchAddress("els_lostHits", &els_lostHits);
	tree->SetBranchAddress("els_mc_id", &els_mc_id);
	tree->SetBranchAddress("els_mc_motherid", &els_mc_motherid);
	tree->SetBranchAddress("els_nSeed", &els_nSeed);
	tree->SetBranchAddress("els_class", &els_class);
	tree->SetBranchAddress("els_looseId", &els_looseId);
	tree->SetBranchAddress("els_tightId", &els_tightId);
	tree->SetBranchAddress("genps_id", &genps_id);
	tree->SetBranchAddress("hyp_jets_mc_id", &hyp_jets_mc_id);
	tree->SetBranchAddress("hyp_other_jets_mc_id", &hyp_other_jets_mc_id);
	tree->SetBranchAddress("evt_met", &evt_met);
	tree->SetBranchAddress("evt_metPhi", &evt_metPhi);
	tree->SetBranchAddress("evt_sumEt", &evt_sumEt);
	tree->SetBranchAddress("evt_metSig", &evt_metSig);
	tree->SetBranchAddress("evt_met_jetcorr", &evt_met_jetcorr);
	tree->SetBranchAddress("metphi_jetcorr", &metphi_jetcorr);
	tree->SetBranchAddress("gen_met", &gen_met);
	tree->SetBranchAddress("gen_metPhi", &gen_metPhi);
	tree->SetBranchAddress("hyp_lt_d0", &hyp_lt_d0);
	tree->SetBranchAddress("hyp_lt_z0", &hyp_lt_z0);
	tree->SetBranchAddress("hyp_lt_vertexphi", &hyp_lt_vertexphi);
	tree->SetBranchAddress("hyp_lt_chi2", &hyp_lt_chi2);
	tree->SetBranchAddress("hyp_lt_ndof", &hyp_lt_ndof);
	tree->SetBranchAddress("hyp_lt_d0Err", &hyp_lt_d0Err);
	tree->SetBranchAddress("hyp_lt_z0Err", &hyp_lt_z0Err);
	tree->SetBranchAddress("hyp_lt_ptErr", &hyp_lt_ptErr);
	tree->SetBranchAddress("hyp_lt_etaErr", &hyp_lt_etaErr);
	tree->SetBranchAddress("hyp_lt_phiErr", &hyp_lt_phiErr);
	tree->SetBranchAddress("hyp_ll_d0", &hyp_ll_d0);
	tree->SetBranchAddress("hyp_ll_z0", &hyp_ll_z0);
	tree->SetBranchAddress("hyp_ll_vertexphi", &hyp_ll_vertexphi);
	tree->SetBranchAddress("hyp_ll_chi2", &hyp_ll_chi2);
	tree->SetBranchAddress("hyp_ll_ndof", &hyp_ll_ndof);
	tree->SetBranchAddress("hyp_ll_d0Err", &hyp_ll_d0Err);
	tree->SetBranchAddress("hyp_ll_z0Err", &hyp_ll_z0Err);
	tree->SetBranchAddress("hyp_ll_ptErr", &hyp_ll_ptErr);
	tree->SetBranchAddress("hyp_ll_etaErr", &hyp_ll_etaErr);
	tree->SetBranchAddress("hyp_ll_phiErr", &hyp_ll_phiErr);
	tree->SetBranchAddress("hyp_met", &hyp_met);
	tree->SetBranchAddress("hyp_metPhi", &hyp_metPhi);
	tree->SetBranchAddress("hyp_metCone", &hyp_metCone);
	tree->SetBranchAddress("hyp_metPhiCone", &hyp_metPhiCone);
	tree->SetBranchAddress("hyp_metNoCalo", &hyp_metNoCalo);
	tree->SetBranchAddress("hyp_metPhiNoCalo", &hyp_metPhiNoCalo);
	tree->SetBranchAddress("hyp_metAll", &hyp_metAll);
	tree->SetBranchAddress("hyp_metPhiAll", &hyp_metPhiAll);
	tree->SetBranchAddress("hyp_metJes5", &hyp_metJes5);
	tree->SetBranchAddress("hyp_metPhiJes5", &hyp_metPhiJes5);
	tree->SetBranchAddress("hyp_metJes10", &hyp_metJes10);
	tree->SetBranchAddress("hyp_metPhiJes10", &hyp_metPhiJes10);
	tree->SetBranchAddress("hyp_metJes15", &hyp_metJes15);
	tree->SetBranchAddress("hyp_metPhiJes15", &hyp_metPhiJes15);
	tree->SetBranchAddress("hyp_metJes30", &hyp_metJes30);
	tree->SetBranchAddress("hyp_metPhiJes30", &hyp_metPhiJes30);
	tree->SetBranchAddress("hyp_metJes50", &hyp_metJes50);
	tree->SetBranchAddress("hyp_metPhiJes50", &hyp_metPhiJes50);
	tree->SetBranchAddress("hyp_metDPhiJet10", &hyp_metDPhiJet10);
	tree->SetBranchAddress("hyp_metDPhiJet15", &hyp_metDPhiJet15);
	tree->SetBranchAddress("hyp_metDPhiJet20", &hyp_metDPhiJet20);
	tree->SetBranchAddress("hyp_metDPhiTrk10", &hyp_metDPhiTrk10);
	tree->SetBranchAddress("hyp_metDPhiTrk25", &hyp_metDPhiTrk25);
	tree->SetBranchAddress("hyp_metDPhiTrk50", &hyp_metDPhiTrk50);
	tree->SetBranchAddress("hyp_lt_iso", &hyp_lt_iso);
	tree->SetBranchAddress("hyp_ll_iso", &hyp_ll_iso);
	tree->SetBranchAddress("evt_xsec_incl", &evt_xsec_incl);
	tree->SetBranchAddress("evt_xsec_excl", &evt_xsec_excl);
	tree->SetBranchAddress("mus_d0", &mus_d0);
	tree->SetBranchAddress("mus_z0", &mus_z0);
	tree->SetBranchAddress("mus_vertexphi", &mus_vertexphi);
	tree->SetBranchAddress("mus_chi2", &mus_chi2);
	tree->SetBranchAddress("mus_ndof", &mus_ndof);
	tree->SetBranchAddress("mus_d0Err", &mus_d0Err);
	tree->SetBranchAddress("mus_z0Err", &mus_z0Err);
	tree->SetBranchAddress("mus_ptErr", &mus_ptErr);
	tree->SetBranchAddress("mus_etaErr", &mus_etaErr);
	tree->SetBranchAddress("mus_phiErr", &mus_phiErr);
	tree->SetBranchAddress("mus_e_em", &mus_e_em);
	tree->SetBranchAddress("mus_e_had", &mus_e_had);
	tree->SetBranchAddress("mus_e_ho", &mus_e_ho);
	tree->SetBranchAddress("mus_e_emS9", &mus_e_emS9);
	tree->SetBranchAddress("mus_e_hadS9", &mus_e_hadS9);
	tree->SetBranchAddress("mus_e_hoS9", &mus_e_hoS9);
	tree->SetBranchAddress("mus_iso03_sumPt", &mus_iso03_sumPt);
	tree->SetBranchAddress("mus_iso03_emEt", &mus_iso03_emEt);
	tree->SetBranchAddress("mus_iso03_hadEt", &mus_iso03_hadEt);
	tree->SetBranchAddress("mus_iso03_hoEt", &mus_iso03_hoEt);
	tree->SetBranchAddress("mus_iso05_sumPt", &mus_iso05_sumPt);
	tree->SetBranchAddress("mus_iso05_emEt", &mus_iso05_emEt);
	tree->SetBranchAddress("mus_iso05_hadEt", &mus_iso05_hadEt);
	tree->SetBranchAddress("mus_iso05_hoEt", &mus_iso05_hoEt);
	tree->SetBranchAddress("mus_gfit_chi2", &mus_gfit_chi2);
	tree->SetBranchAddress("mus_gfit_ndof", &mus_gfit_ndof);
	tree->SetBranchAddress("trks_d0", &trks_d0);
	tree->SetBranchAddress("trks_z0", &trks_z0);
	tree->SetBranchAddress("trks_vertexphi", &trks_vertexphi);
	tree->SetBranchAddress("trks_chi2", &trks_chi2);
	tree->SetBranchAddress("trks_ndof", &trks_ndof);
	tree->SetBranchAddress("trks_d0Err", &trks_d0Err);
	tree->SetBranchAddress("trks_z0Err", &trks_z0Err);
	tree->SetBranchAddress("trks_ptErr", &trks_ptErr);
	tree->SetBranchAddress("trks_etaErr", &trks_etaErr);
	tree->SetBranchAddress("trks_phiErr", &trks_phiErr);
	tree->SetBranchAddress("els_d0", &els_d0);
	tree->SetBranchAddress("els_z0", &els_z0);
	tree->SetBranchAddress("els_vertexphi", &els_vertexphi);
	tree->SetBranchAddress("els_chi2", &els_chi2);
	tree->SetBranchAddress("els_ndof", &els_ndof);
	tree->SetBranchAddress("els_d0Err", &els_d0Err);
	tree->SetBranchAddress("els_z0Err", &els_z0Err);
	tree->SetBranchAddress("els_ptErr", &els_ptErr);
	tree->SetBranchAddress("els_etaErr", &els_etaErr);
	tree->SetBranchAddress("els_phiErr", &els_phiErr);
	tree->SetBranchAddress("els_hOverE", &els_hOverE);
	tree->SetBranchAddress("els_eOverPIn", &els_eOverPIn);
	tree->SetBranchAddress("els_eSeedOverPOut", &els_eSeedOverPOut);
	tree->SetBranchAddress("els_fBrem", &els_fBrem);
	tree->SetBranchAddress("els_dEtaIn", &els_dEtaIn);
	tree->SetBranchAddress("els_dEtaOut", &els_dEtaOut);
	tree->SetBranchAddress("els_dPhiIn", &els_dPhiIn);
	tree->SetBranchAddress("els_dPhiOut", &els_dPhiOut);
	tree->SetBranchAddress("els_ESc", &els_ESc);
	tree->SetBranchAddress("els_ESc_raw", &els_ESc_raw);
	tree->SetBranchAddress("els_e3x3", &els_e3x3);
	tree->SetBranchAddress("els_e5x5", &els_e5x5);
	tree->SetBranchAddress("els_ESeed", &els_ESeed);
	tree->SetBranchAddress("els_sigmaPhiPhi", &els_sigmaPhiPhi);
	tree->SetBranchAddress("els_sigmaEtaEta", &els_sigmaEtaEta);
	tree->SetBranchAddress("els_tkIso", &els_tkIso);
	tree->SetBranchAddress("genjs_emEnergy", &genjs_emEnergy);
	tree->SetBranchAddress("genjs_hadEnergy", &genjs_hadEnergy);
	tree->SetBranchAddress("genjs_invEnergy", &genjs_invEnergy);
	tree->SetBranchAddress("genjs_otherEnergy", &genjs_otherEnergy);
	tree->SetBranchAddress("hyp_jets_emFrac", &hyp_jets_emFrac);
	tree->SetBranchAddress("hyp_jets_chFrac", &hyp_jets_chFrac);
	tree->SetBranchAddress("hyp_jets_mc_emEnergy", &hyp_jets_mc_emEnergy);
	tree->SetBranchAddress("hyp_jets_mc_hadEnergy", &hyp_jets_mc_hadEnergy);
	tree->SetBranchAddress("hyp_jets_mc_invEnergy", &hyp_jets_mc_invEnergy);
	tree->SetBranchAddress("hyp_jets_mc_otherEnergy", &hyp_jets_mc_otherEnergy);
	tree->SetBranchAddress("hyp_jets_cor", &hyp_jets_cor);
	tree->SetBranchAddress("hyp_other_jets_emFrac", &hyp_other_jets_emFrac);
	tree->SetBranchAddress("hyp_other_jets_chFrac", &hyp_other_jets_chFrac);
	tree->SetBranchAddress("hyp_other_jets_mc_emEnergy", &hyp_other_jets_mc_emEnergy);
	tree->SetBranchAddress("hyp_other_jets_mc_hadEnergy", &hyp_other_jets_mc_hadEnergy);
	tree->SetBranchAddress("hyp_other_jets_mc_invEnergy", &hyp_other_jets_mc_invEnergy);
	tree->SetBranchAddress("hyp_other_jets_mc_otherEnergy", &hyp_other_jets_mc_otherEnergy);
	tree->SetBranchAddress("hyp_other_jets_cor", &hyp_other_jets_cor);
	tree->SetBranchAddress("hyp_lt_p4", &hyp_lt_p4);
	tree->SetBranchAddress("hyp_lt_trk_p4", &hyp_lt_trk_p4);
	tree->SetBranchAddress("hyp_lt_mc_p4", &hyp_lt_mc_p4);
	tree->SetBranchAddress("hyp_ll_p4", &hyp_ll_p4);
	tree->SetBranchAddress("hyp_ll_trk_p4", &hyp_ll_trk_p4);
	tree->SetBranchAddress("hyp_ll_mc_p4", &hyp_ll_mc_p4);
	tree->SetBranchAddress("hyp_p4", &hyp_p4);
	tree->SetBranchAddress("mus_p4", &mus_p4);
	tree->SetBranchAddress("mus_trk_p4", &mus_trk_p4);
	tree->SetBranchAddress("mus_mc_p4", &mus_mc_p4);
	tree->SetBranchAddress("trks_p4", &trks_p4);
	tree->SetBranchAddress("trks_trk_p4", &trks_trk_p4);
	tree->SetBranchAddress("trks_mc_p4", &trks_mc_p4);
	tree->SetBranchAddress("els_p4", &els_p4);
	tree->SetBranchAddress("els_trk_p4", &els_trk_p4);
	tree->SetBranchAddress("els_mc_p4", &els_mc_p4);
	tree->SetBranchAddress("genps_p4", &genps_p4);
	tree->SetBranchAddress("genjs_p4", &genjs_p4);
	tree->SetBranchAddress("hyp_jets_p4", &hyp_jets_p4);
	tree->SetBranchAddress("hyp_jets_mc_p4", &hyp_jets_mc_p4);
	tree->SetBranchAddress("hyp_jets_mc_gp_p4", &hyp_jets_mc_gp_p4);
	tree->SetBranchAddress("hyp_other_jets_p4", &hyp_other_jets_p4);
	tree->SetBranchAddress("hyp_other_jets_mc_p4", &hyp_other_jets_mc_p4);
	tree->SetBranchAddress("hyp_other_jets_mc_gp_p4", &hyp_other_jets_mc_gp_p4);
	tree->SetBranchAddress("evt_scale1fb", &evt_scale1fb);


	// Make sure the specDY flag is kosher
	if (specDY < -1 || specDY > 2) {
	  std::cout << "specDY flag is not allowed...quit" << std::endl;
	  return 1;
	}

	//  Book histograms...
	//  Naming Convention:
	//  Prefix comes from the sample and it is passed to the scanning function
	//  Suffix is "ee" "em" "em" "all" which depends on the final state
	//  For example: histogram named tt_hnJet_ee would be the Njet distribution
	//  for the ee final state in the ttbar sample.

	// MAKE SURE TO CAL SUMW2 FOR EACH 1D HISTOGRAM BEFORE FILLING!!!!!! 

	TH1F* hnJet[4];       // Njet distributions
	TH1F* helePt[4];      // electron Pt
	TH1F* hmuPt[4];       // muon Pt
	TH1F* hminLepPt[4];   // minimum lepton Pt
	TH1F* hmaxLepPt[4];   // maximum lepton Pt
	TH1F* helePhi[4];     // electron phi
	TH1F* hmuPhi[4];      // muon phi
	TH1F* hdphiLep[4];    // delta phi between leptons
	TH1F* heleEta[4];     // electron eta
	TH1F* hmuEta[4];      // muon eta
	TH1F* hdilMass[4];    // dilepton mass
	TH1F* hdilMassTightWindow[4]; // dilepton mass, but zooming around Z
	TH1F* hdilPt[4];       // dilepton Pt
	TH1F* hmet[4];       // MET
	TH1F* hmetPhi[4];       // MET phi
	TH2F* hmetVsDilepPt[4];  // MET vs dilepton Pt

	TH2F* hmetOverPtVsDphi[4]; // MET/Lepton Pt vs DeltaPhi between MET and Lepton Pt
	TH1F* hptJet1[4];   // Pt of 1st jet
	TH1F* hptJet2[4];   // Pt of 2nd jet
	TH1F* hptJet3[4];   // Pt of 3rd jet
	TH1F* hptJet4[4];   // Pt of 4th jet
	TH1F* hetaJet1[4];   // eta of 1st jet
	TH1F* hetaJet2[4];   // eta of 2nd jet
	TH1F* hetaJet3[4];   // eta of 3rd jet
	TH1F* hetaJet4[4];   // eta of 4th jet


	char *suffix[3];
	suffix[0] = "ee";
	suffix[1] = "mm";
	suffix[2] = "em";
	suffix[3] = "all";

	// The statement below should work but does not work due to bug in root when TH@ are also used
	// Rene Brun promised a fix.
	//TH1::SetDefaultSumw2(kTRUE); // do errors properly based on weights

	for (int i=0; i<4; i++) {
	  hnJet[i] = new TH1F(Form("%s_hnJet_%s",prefix,suffix[i]),Form("%s_nJet_%s",prefix,suffix[i]),
			      5,0.,5.);	
	  helePt[i] = new TH1F(Form("%s_helePt_%s",prefix,suffix[i]),Form("%s_elePt_%s",prefix,suffix[i]),
			       150,0.,150.);
	  hmuPt[i]  = new TH1F(Form("%s_hmuPt_%s",prefix,suffix[i]),Form("%s_muPt_%s",prefix,suffix[i]),
			       150,0.,150.);
	  hminLepPt[i]  = new TH1F(Form("%s_hminLepPt_%s",prefix,suffix[i]),
                                   Form("%s_minLepPt_%s",prefix,suffix[i]),150,0.,150.);
	  hmaxLepPt[i]  = new TH1F(Form("%s_hmaxLepPt_%s",prefix,suffix[i]),
                                   Form("%s_maxLepPt_%s",prefix,suffix[i]),150,0.,150.);
          helePhi[i] = new TH1F(Form("%s_helePhi_%s",prefix,suffix[i]),Form("%s_elePhi_%s",prefix,suffix[i]),
				50,-1*TMath::Pi(), TMath::Pi());
          hmuPhi[i]  = new TH1F(Form("%s_hmuPhi_%s",prefix,suffix[i]),Form("%s_muPhi_%s",prefix,suffix[i]),
				50,-1*TMath::Pi(), TMath::Pi());
          hdphiLep[i]  = new TH1F(Form("%s_hdphiLep_%s",prefix,suffix[i]),Form("%s_dphiLep_%s",prefix,suffix[i]),
				50,0., TMath::Pi());
          heleEta[i] = new TH1F(Form("%s_heleEta_%s",prefix,suffix[i]),Form("%s_eleEta_%s",prefix,suffix[i]),
				60, -3., 3.);
          hmuEta[i]  = new TH1F(Form("%s_hmuEta_%s",prefix,suffix[i]),Form("%s_muEta_%s",prefix,suffix[i]),
				60, -3., 3.);
	  hdilMass[i] = new TH1F(Form("%s_hdilMass_%s",prefix,suffix[i]),Form("%s_dilMass_%s",prefix,suffix[i]),
				 100, 0., 300.);
	  hdilMassTightWindow[i] = new TH1F(Form("%s_hdilMassTightWindow_%s",prefix,suffix[i]),
					    Form("%s_dilMassTightWindow_%s",prefix,suffix[i]),
				            120, 60., 120.);
	  hdilPt[i] = new TH1F(Form("%s_hdilPt_%s",prefix,suffix[i]),Form("%s_dilPt_%s",prefix,suffix[i]),
			       	    100, 0., 300.);
	  hmet[i] = new TH1F(Form("%s_hmet_%s",prefix,suffix[i]),Form("%s_met_%s",prefix,suffix[i]),100,0.,200.);
	  hmetPhi[i] = new TH1F(Form("%s_hmetPhi_%s",prefix,suffix[i]),Form("%s_metPhi_%s",prefix,suffix[i]),
				50,-1*TMath::Pi(), TMath::Pi());
	  hmetVsDilepPt[i] = new TH2F(Form("%s_hmetVsDilepPt_%s",prefix,suffix[i]),
				      Form("%s_metVsDilepPt_%s",prefix,suffix[i]),
				      100,0.,200.,100,0.,200.);
	  hmetOverPtVsDphi[i] = new TH2F(Form("%s_hmetOverPtVsDphi_%s",prefix,suffix[i]),
					 Form("%s_metOverPtVsDphi_%s",prefix,suffix[i]),
					 100,0.,3.,50,0., TMath::Pi());
	  hptJet1[i] = new TH1F(Form("%s_hptJet1_%s",prefix,suffix[i]),Form("%s_ptJet1_%s",prefix,suffix[i]),
	  			       	    100, 0., 300.);
	  hptJet2[i] = new TH1F(Form("%s_hptJet2_%s",prefix,suffix[i]),Form("%s_ptJet2_%s",prefix,suffix[i]),
				              100, 0., 300.);
	  hptJet3[i] = new TH1F(Form("%s_hptJet3_%s",prefix,suffix[i]),Form("%s_ptJet3_%s",prefix,suffix[i]),
	  			       	    100, 0., 300.);
	  hptJet4[i] = new TH1F(Form("%s_hptJet4_%s",prefix,suffix[i]),Form("%s_ptJet4_%s",prefix,suffix[i]),
	  			       	    100, 0., 300.);

	  hetaJet1[i] = new TH1F(Form("%s_hetaJet1_%s",prefix,suffix[i]),Form("%s_etaJet1_%s",prefix,suffix[i]),
				 50, -4., 4.);
	  hetaJet2[i] = new TH1F(Form("%s_hetaJet2_%s",prefix,suffix[i]),Form("%s_etaJet2_%s",prefix,suffix[i]),
				 50, -4., 4.);
	  hetaJet3[i] = new TH1F(Form("%s_hetaJet3_%s",prefix,suffix[i]),Form("%s_etaJet3_%s",prefix,suffix[i]),
				 50, -4., 4.);
	  hetaJet4[i] = new TH1F(Form("%s_hetaJet4_%s",prefix,suffix[i]),Form("%s_etaJet4_%s",prefix,suffix[i]),
				 50, -4., 4.);


	  hnJet[i]->Sumw2();
	  helePt[i]->Sumw2();
	  hmuPt[i]->Sumw2();
	  hminLepPt[i]->Sumw2();
	  hmaxLepPt[i]->Sumw2();
	  helePhi[i]->Sumw2();
	  hmuPhi[i]->Sumw2();
	  hdphiLep[i]->Sumw2();
	  heleEta[i]->Sumw2();
	  hmuEta[i]->Sumw2();
	  hdilMass[i]->Sumw2();
	  hdilMassTightWindow[i]->Sumw2();
	  hdilPt[i]->Sumw2();
	  hmet[i]->Sumw2();
	  hmetPhi[i]->Sumw2();
	  hptJet1[i]->Sumw2();
	  hptJet2[i]->Sumw2();
	  hptJet3[i]->Sumw2();
	  hptJet4[i]->Sumw2();
	  hetaJet1[i]->Sumw2();
	  hetaJet2[i]->Sumw2();
	  hetaJet3[i]->Sumw2();
	  hetaJet4[i]->Sumw2();

	}

	//Event Loop
	int nEntries = tree->GetEntries();
	int nLoop = nEntries;
	//	nLoop = min(nEntries,10000);
	for( int z = 0; z < nLoop; z++) {
	  int iz = z/1000;
	  if (z-1000*iz == 0) std::cout << "Processing event " << z+1 << std::endl;
	  tree->GetEntry(z);
	
	  // special handling for DY
	  bool processEvent=true;
	  if (specDY == 0) {
	    if ( !isDYee() ) processEvent = false;
	  } else if (specDY == 1) {
	    if ( !isDYmm() ) processEvent = false;
	  } else if (specDY == 2) {
	    if ( !isDYtt() ) processEvent = false;
	  }
	  if (!processEvent) continue;


	  // More special handling for DY, since the scale factor in the ntuple is wrong
	  // 1750 pb is the x-section for mass>40 DY->ee  (same for mm, tautau)
	  // The cross-section that was used is evt_xsec_excl in pb
	  // the factor of 1.2 is there for NLO
	  float kFactor=1.0;
	  if (specDY == 0 || specDY == 1 || specDY == 2) {
	    kFactor = 1.2 * (3. * 1750) / evt_xsec_excl;
	  }

	  // Apply all the Pass2 cuts
	  if (! pass2Selection() ) continue;


	  // Muon quality cuts
	  if (abs(hyp_lt_id) == 13) {
	    if ( !goodGlobalMuon(hyp_lt_index) ) continue;
	  }
	  if (abs(hyp_ll_id) == 13) {
	    if ( !goodGlobalMuon(hyp_ll_index) ) continue;
	  }
	    

	  // Electron quality cuts.  For starters, apply loose to both (?)
	  if (abs(hyp_lt_id) == 11) {
	    if ( !electron_selection(hyp_lt_index, 0) ) continue;
	  }
	  if (abs(hyp_ll_id) == 11) {
	    if ( !electron_selection(hyp_ll_index, 0) ) continue;
	  }


	  // If we made it to here, we passed all cuts and we are ready to fill
	  float weight = evt_scale1fb * kFactor;    // histogram weight
	  int myType = 99;
	  if (hyp_type == 3) myType = 0;  // ee
	  if (hyp_type == 0) myType = 1;  // mm
	  if (hyp_type == 1 || hyp_type == 2) myType=2; // em
	  if (myType == 99) {
	    std::cout << "YUK:  unknown dilepton type = " << hyp_type << std::endl;
	    continue;
	  }

	  // jet count
	  hnJet[myType]->Fill(hyp_njets, weight);
	  hnJet[3]->Fill(hyp_njets, weight);

	  // lepton Pt
	  if (abs(hyp_lt_id) == 11) helePt[myType]->Fill(hyp_lt_p4->pt(), weight);
	  if (abs(hyp_ll_id) == 11) helePt[myType]->Fill(hyp_ll_p4->pt(), weight);
	  if (abs(hyp_lt_id) == 13) hmuPt[myType]->Fill(hyp_lt_p4->pt(), weight);
	  if (abs(hyp_ll_id) == 13) hmuPt[myType]->Fill(hyp_ll_p4->pt(), weight);
	  hminLepPt[myType]->Fill(min(hyp_ll_p4->pt(), hyp_lt_p4->pt()), weight);
	  hmaxLepPt[myType]->Fill(max(hyp_ll_p4->pt(), hyp_lt_p4->pt()), weight );

	  if (abs(hyp_lt_id) == 11) helePt[3]->Fill(hyp_lt_p4->pt(), weight);
	  if (abs(hyp_ll_id) == 11) helePt[3]->Fill(hyp_ll_p4->pt(), weight);
	  if (abs(hyp_lt_id) == 13) hmuPt[3]->Fill(hyp_lt_p4->pt(), weight);
	  if (abs(hyp_ll_id) == 13) hmuPt[3]->Fill(hyp_ll_p4->pt(), weight);
	  hminLepPt[3]->Fill(min(hyp_ll_p4->pt(), hyp_lt_p4->pt()), weight);
	  hmaxLepPt[3]->Fill(max(hyp_ll_p4->pt(), hyp_lt_p4->pt()), weight );

	  // lepton Phi
	  if (abs(hyp_lt_id) == 11) helePhi[myType]->Fill(hyp_lt_p4->phi(), weight);
	  if (abs(hyp_ll_id) == 11) helePhi[myType]->Fill(hyp_ll_p4->phi(), weight);
	  if (abs(hyp_lt_id) == 13) hmuPhi[myType]->Fill(hyp_lt_p4->phi(), weight);
	  if (abs(hyp_ll_id) == 13) hmuPhi[myType]->Fill(hyp_ll_p4->phi(), weight);

	  if (abs(hyp_lt_id) == 11) helePhi[3]->Fill(hyp_lt_p4->phi(), weight);
	  if (abs(hyp_ll_id) == 11) helePhi[3]->Fill(hyp_ll_p4->phi(), weight);
	  if (abs(hyp_lt_id) == 13) hmuPhi[3]->Fill(hyp_lt_p4->phi(), weight);
	  if (abs(hyp_ll_id) == 13) hmuPhi[3]->Fill(hyp_ll_p4->phi(), weight);

	  // delta phi btw leptons
	  double dphi = fabs(hyp_lt_p4->phi() - hyp_ll_p4->phi());
	  if (dphi > TMath::Pi()) dphi = TMath::TwoPi() - dphi;
	  hdphiLep[myType]->Fill(dphi, weight);
	  hdphiLep[3]->Fill(dphi, weight);

	  // lepton Eta
	  if (abs(hyp_lt_id) == 11) heleEta[myType]->Fill(hyp_lt_p4->eta(), weight);
	  if (abs(hyp_ll_id) == 11) heleEta[myType]->Fill(hyp_ll_p4->eta(), weight);
	  if (abs(hyp_lt_id) == 13) hmuEta[myType]->Fill(hyp_lt_p4->eta(), weight);
	  if (abs(hyp_ll_id) == 13) hmuEta[myType]->Fill(hyp_ll_p4->eta(), weight);

	  if (abs(hyp_lt_id) == 11) heleEta[3]->Fill(hyp_lt_p4->eta(), weight);
	  if (abs(hyp_ll_id) == 11) heleEta[3]->Fill(hyp_ll_p4->eta(), weight);
	  if (abs(hyp_lt_id) == 13) hmuEta[3]->Fill(hyp_lt_p4->eta(), weight);
	  if (abs(hyp_ll_id) == 13) hmuEta[3]->Fill(hyp_ll_p4->eta(), weight);

	  // dilepton mass
	  hdilMass[myType]->Fill(hyp_p4->mass(), weight);
	  hdilMassTightWindow[myType]->Fill(hyp_p4->mass(), weight);
	  hdilMass[3]->Fill(hyp_p4->mass(), weight);
	  hdilMassTightWindow[3]->Fill(hyp_p4->mass(), weight);

	  // dilepton pt
	  hdilPt[myType]->Fill(hyp_p4->pt(), weight);
	  hdilPt[3]->Fill(hyp_p4->pt(), weight);

	  // Met and Met phi
	  hmet[myType]->Fill(hyp_met, weight);      
	  hmetPhi[myType]->Fill(hyp_metPhi, weight);      
	  hmet[3]->Fill(hyp_met, weight);      
	  hmetPhi[3]->Fill(hyp_metPhi, weight);      

	  // Met vs dilepton Pt
	  hmetVsDilepPt[myType]->Fill(hyp_met, hyp_p4->pt(), weight);
	  hmetVsDilepPt[3]->Fill(hyp_met, hyp_p4->pt(), weight);
    
	  // Met over dilepton Pt vs deltaphi btw the two
	  double dphi2 = fabs(hyp_p4->phi() - hyp_metPhi);
	  if (dphi2 > TMath::Pi()) dphi2 = TMath::TwoPi() - dphi2;
	  hmetOverPtVsDphi[myType]->Fill(hyp_met/hyp_p4->pt(), dphi2, weight);
	  hmetOverPtVsDphi[3]->Fill(hyp_met/hyp_p4->pt(), dphi2, weight);

	  // Make a vector of sorted jets, fill jet histograms
	  if (hyp_njets > 0) {
	    vector<ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> > > my_hyp_jets_p4(*hyp_jets_p4);
	    sort(my_hyp_jets_p4.begin(), my_hyp_jets_p4.end(), comparePt);   // sort them by Pt
	    hptJet1[myType]->Fill(my_hyp_jets_p4[0].Pt(), weight);
	    hptJet1[3]->Fill(my_hyp_jets_p4[0].Pt(), weight);
	    hetaJet1[myType]->Fill(my_hyp_jets_p4[0].Eta(), weight);
	    hetaJet1[3]->Fill(my_hyp_jets_p4[0].Eta(), weight);
	    if (hyp_njets > 1) {
	      hptJet2[myType]->Fill(my_hyp_jets_p4[1].Pt(), weight);
	      hptJet2[3]->Fill(my_hyp_jets_p4[1].Pt(), weight);
	      hetaJet2[myType]->Fill(my_hyp_jets_p4[1].Eta(), weight);
	      hetaJet2[3]->Fill(my_hyp_jets_p4[1].Eta(), weight);
	    }
	    if (hyp_njets > 2) {
	      hptJet3[myType]->Fill(my_hyp_jets_p4[2].Pt(), weight);
	      hptJet3[3]->Fill(my_hyp_jets_p4[2].Pt(), weight);
	      hetaJet3[myType]->Fill(my_hyp_jets_p4[2].Eta(), weight);
	      hetaJet3[3]->Fill(my_hyp_jets_p4[2].Eta(), weight);
	    }
	    if (hyp_njets > 3) {
	      hptJet4[myType]->Fill(my_hyp_jets_p4[3].Pt(), weight);
	      hptJet4[3]->Fill(my_hyp_jets_p4[3].Pt(), weight);
	      hetaJet4[myType]->Fill(my_hyp_jets_p4[3].Eta(), weight);
	      hetaJet4[3]->Fill(my_hyp_jets_p4[3].Eta(), weight);
	    }
	  }
    
  }
  return 0;
}
