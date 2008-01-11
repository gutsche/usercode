{
  gROOT->ProcessLine(".x chain_QCD_Pt_1800_2200.C");
  cout << "Processing: " << chain->GetEntries() << " entries." << endl;
  gROOT->ProcessLine(".L fakeRates.C+");
  ScanChain(chain,"qcd_pt_1800_2200");
}
