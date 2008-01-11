{
  gROOT->ProcessLine(".x chain_QCD_Pt_80_120.C");
  cout << "Processing: " << chain->GetEntries() << " entries." << endl;
  gROOT->ProcessLine(".L fakeRates.C+");
  ScanChain(chain,"qcd_pt_80_120");
}
