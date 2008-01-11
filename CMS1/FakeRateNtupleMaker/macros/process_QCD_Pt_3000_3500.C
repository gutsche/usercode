{
  gROOT->ProcessLine(".x chain_QCD_Pt_3000_3500.C");
  cout << "Processing: " << chain->GetEntries() << " entries." << endl;
  gROOT->ProcessLine(".L fakeRates.C+");
  ScanChain(chain,"qcd_pt_3000_3500");
}
