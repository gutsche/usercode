{
  gROOT->ProcessLine(".x chain_QCD_Pt_15_20.C");
  cout << "Processing: " << chain->GetEntries() << " entries." << endl;
  gROOT->ProcessLine(".L fakeRates.C+");
  ScanChain(chain,"qcd_pt_15_20");
}
