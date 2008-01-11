{
  gROOT->ProcessLine(".x chain_QCD_Pt_50_80.C");
  cout << "Processing: " << chain->GetEntries() << " entries." << endl;
  gROOT->ProcessLine(".L fakeRates.C+");
  ScanChain(chain,"qcd_pt_50_80");
}
