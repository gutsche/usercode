{
  gROOT->ProcessLine(".x chain_QCD_Pt_2200_2600.C");
  cout << "Processing: " << chain->GetEntries() << " entries." << endl;
  gROOT->ProcessLine(".L fakeRates.C+");
  ScanChain(chain,"qcd_pt_2200_2600");
}
