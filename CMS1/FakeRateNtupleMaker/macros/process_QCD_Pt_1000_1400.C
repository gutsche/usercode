{
  gROOT->ProcessLine(".x chain_QCD_Pt_1000_1400.C");
  cout << "Processing: " << chain->GetEntries() << " entries." << endl;
  gROOT->ProcessLine(".L fakeRates.C+");
  ScanChain(chain,"qcd_pt_1000_1400");
}
