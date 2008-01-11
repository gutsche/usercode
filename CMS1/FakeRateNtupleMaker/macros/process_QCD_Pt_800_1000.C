{
  gROOT->ProcessLine(".x chain_QCD_Pt_800_1000.C");
  cout << "Processing: " << chain->GetEntries() << " entries." << endl;
  gROOT->ProcessLine(".L fakeRates.C+");
  ScanChain(chain,"qcd_pt_800_1000");
}
