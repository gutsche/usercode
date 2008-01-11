{
  gROOT->ProcessLine(".x chain_QCD_Pt_30_50.C");
  cout << "Processing: " << chain->GetEntries() << " entries." << endl;
  gROOT->ProcessLine(".L fakeRates.C+");
  ScanChain(chain,"qcd_pt_30_50");
}
