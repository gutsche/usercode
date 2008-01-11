{
  gROOT->ProcessLine(".x chain_QCD_Pt_380_470.C");
  cout << "Processing: " << chain->GetEntries() << " entries." << endl;
  gROOT->ProcessLine(".L fakeRates.C+");
  ScanChain(chain,"qcd_pt_380_470");
}
