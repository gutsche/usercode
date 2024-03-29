{
   gROOT->SetStyle("Plain");
   gStyle->SetPalette(1);
   gStyle->SetOptStat(1111111);
   
   TFile *fzmm = new TFile("/uscms_data/d1/cms1/V01-09-06/results/tablemaker_Zmumu_ntuple.root");
   TTree *tzmm = (TTree*)(fzmm->Get("event"));
   
   TFile *fzee = new TFile("/uscms_data/d1/cms1/V01-09-06/results/tablemaker_Zee_ntuple.root");
   TTree *tzee = (TTree*)(fzee->Get("event"));
   
   TFile *fww = new TFile("/uscms_data/d1/cms1/V01-09-06/results/tablemaker_WW_ntuple.root");
   TTree *tww = (TTree*)(fww->Get("event"));
   
   TFile *fwz = new TFile("/uscms_data/d1/cms1/V01-09-06/results/tablemaker_WZ_ntuple.root");
   TTree *twz = (TTree*)(fwz->Get("event"));
   
   TFile *fzz = new TFile("/uscms_data/d1/cms1/V01-09-06/results/tablemaker_ZZ_ntuple.root");
   TTree *tzz = (TTree*)(fzz->Get("event"));
   
   TFile *fztt = new TFile("/uscms_data/d1/cms1/V01-09-06/results/tablemaker_Ztautau_ntuple.root");
   TTree *tztt = (TTree*)(fztt->Get("event"));

   TFile *fhww = new TFile("/uscms_data/d1/cms1/V01-09-06/results/tablemaker_HWW_ntuple.root");
   TTree *thww = (TTree*)(fhww->Get("event"));

   TFile *ftt = new TFile("/uscms_data/d1/cms1/V01-09-06/results/tablemaker_TTbar_ntuple.root");
   TTree *ttt = (TTree*)(ftt->Get("event"));

   gROOT->LoadMacro("dilepton_functions.cxx");
   
   TCut emu_mue("(hyp_type==1||hyp_type==2)");
   TCut pass1("((hyp_met>40)||(hyp_met>20&&(hyp_p4.mass()<80||hyp_p4.mass()>100)))");
   TCut align_old("hyp_met/hyp_p4.pt()>0.6||fabs(fabs(hyp_metPhi-hyp_p4.phi())-3.1416)>0.25");
   TCut align("hyp_met/hyp_p4.pt()>0.6||acos(cos(hyp_metPhi-hyp_p4.phi()-3.1416))>0.25");
   TCut dyTight("hyp_met>30&&fabs(hyp_p4.M()-90)>10");
   TCut dyLoose("hyp_met>30||(fabs(hyp_p4.M()-90)<10&&hyp_met>50)");
   TCut iso("hyp_lt_iso<5&&hyp_ll_iso<5");

   TCut iso("hyp_lt_iso<5&&hyp_ll_iso<5");
   TCut align("hyp_met/hyp_p4.pt()>0.6||acos(cos(hyp_metPhi-hyp_p4.phi()-3.1416))>0.25");
   TCut drellyan("hyp_met>30&&fabs(hyp_p4.M()-90)>10");
   
   TCut mue("hyp_type==1");
   TCut emu("hyp_type==2");
   TCut ee("hyp_type==3");
   TCut mumu("hyp_type==0");

   TCut pass2_mumu(mumu+iso+align+drellyan);
   TCut pass2_ee(ee+iso+align+drellyan);
   TCut pass2_mue(mue+iso+"hyp_met>20");
   TCut pass2_emu(emu+iso+"hyp_met>20");
   TCut pass2_ee_tight(ee+iso+align+drellyan+"elid_tightId[hyp_lt_index]==1");
   TCut pass2_ee_loose(ee+iso+align+drellyan+"elid_looseId[hyp_lt_index]==1");
   TCut pass2_mue_loose(mue+iso+"hyp_met>20"+"elid_looseId[hyp_ll_index]==1");
   TCut pass2_mue_tight(mue+iso+"hyp_met>20"+"elid_tightId[hyp_ll_index]==1");
   TCut pass2_emu_loose(emu+iso+"hyp_met>20"+"elid_looseId[hyp_lt_index]==1");
   TCut pass2_emu_tight(emu+iso+"hyp_met>20"+"elid_tightId[hyp_lt_index]==1");
   
   cout << "Finished initialization." << endl;
}
