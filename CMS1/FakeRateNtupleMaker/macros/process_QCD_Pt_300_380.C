{
  gROOT->ProcessLine(".x chain_QCD_Pt_300_380.C");
  cout << "Processing: " << chain->GetEntries() << " entries." << endl;
  gROOT->ProcessLine(".L fakeRates.C+");
  ScanChain(chain,"qcd_pt_300_380");
}
