{
   gROOT->SetStyle("Plain");
   gStyle->SetPalette(1);
   gStyle->SetOptStat(1111111);
   
   TFile *fzmm = new TFile("/uscms_data/d1/cms1/V01-09-00/results/tablemaker_Zmumu_ntuple.root");
   TTree *tzmm = (TTree*)(fzmm->Get("event"));
   
   TFile *fzee = new TFile("/uscms_data/d1/cms1/V01-09-00/results/tablemaker_Zee_ntuple.root");
   TTree *tzee = (TTree*)(fzee->Get("event"));
   
   TFile *fww = new TFile("/uscms_data/d1/cms1/V01-09-00/results/tablemaker_WW_ntuple.root");
   TTree *tww = (TTree*)(fww->Get("event"));
   
   TFile *fwz = new TFile("/uscms_data/d1/cms1/V01-09-00/results/tablemaker_WZ_ntuple.root");
   TTree *twz = (TTree*)(fwz->Get("event"));
   
   TFile *fzz = new TFile("/uscms_data/d1/cms1/V01-09-00/results/tablemaker_ZZ_ntuple.root");
   TTree *tzz = (TTree*)(fzz->Get("event"));
   
   TFile *fztt = new TFile("/uscms_data/d1/cms1/V01-09-00/results/tablemaker_Ztautau_ntuple.root");
   TTree *tztt = (TTree*)(fztt->Get("event"));

   TFile *fhww = new TFile("/uscms_data/d1/cms1/V01-09-00/results/tablemaker_HWW_ntuple.root");
   TTree *thww = (TTree*)(fhww->Get("event"));

   TFile *ftt = new TFile("/uscms_data/d1/cms1/V01-09-00/results/tablemaker_TTbar_ntuple.root");
   TTree *ttt = (TTree*)(ftt->Get("event"));

   gROOT->LoadMacro("dilepton_functions.cxx");
   
   cout << "Finished initialization." << endl;
}
