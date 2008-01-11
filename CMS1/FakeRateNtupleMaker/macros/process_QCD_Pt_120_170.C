{
  gROOT->ProcessLine(".x chain_QCD_Pt_120_170.C");
  cout << "Processing: " << chain->GetEntries() << " entries." << endl;
  gROOT->ProcessLine(".L fakeRates.C+");
  ScanChain(chain,"qcd_pt_120_170");
}
