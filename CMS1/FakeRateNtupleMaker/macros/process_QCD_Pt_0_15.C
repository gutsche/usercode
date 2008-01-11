{
  gROOT->ProcessLine(".x chain_QCD_Pt_0_15.C");
  cout << "Processing: " << chain->GetEntries() << " entries." << endl;
  gROOT->ProcessLine(".L fakeRates.C+");
  ScanChain(chain,"qcd_pt_0_15");
}
