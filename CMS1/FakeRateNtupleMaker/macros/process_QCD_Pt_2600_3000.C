{
  gROOT->ProcessLine(".x chain_QCD_Pt_2600_3000.C");
  cout << "Processing: " << chain->GetEntries() << " entries." << endl;
  gROOT->ProcessLine(".L fakeRates.C+");
  ScanChain(chain,"qcd_pt_2600_3000");
}
