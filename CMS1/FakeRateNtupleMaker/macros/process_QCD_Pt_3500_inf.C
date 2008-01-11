{
  gROOT->ProcessLine(".x chain_QCD_Pt_3500_inf.C");
  cout << "Processing: " << chain->GetEntries() << " entries." << endl;
  gROOT->ProcessLine(".L fakeRates.C+");
  ScanChain(chain,"qcd_pt_3500_inf");
}
