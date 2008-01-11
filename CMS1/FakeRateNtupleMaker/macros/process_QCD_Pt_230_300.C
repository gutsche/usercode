{
  gROOT->ProcessLine(".x chain_QCD_Pt_230_300.C");
  cout << "Processing: " << chain->GetEntries() << " entries." << endl;
  gROOT->ProcessLine(".L fakeRates.C+");
  ScanChain(chain,"qcd_pt_230_300");
}
