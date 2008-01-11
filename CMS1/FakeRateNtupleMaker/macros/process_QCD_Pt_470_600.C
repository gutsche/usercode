{
  gROOT->ProcessLine(".x chain_QCD_Pt_470_600.C");
  cout << "Processing: " << chain->GetEntries() << " entries." << endl;
  gROOT->ProcessLine(".L fakeRates.C+");
  ScanChain(chain,"qcd_pt_470_600");
}
