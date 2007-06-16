void test() {

  TFile *file1 = new TFile("070525_02_81/histograms.root");
  TFile *file2 = new TFile("070525_03_81/histograms.root");
  TFile *file3 = new TFile("070525_04_81/histograms.root");
  TFile *file4 = new TFile("070525_05_81/histograms.root");
  TFile *file5 = new TFile("070525_06_81/histograms.root");
  TFile *file6 = new TFile("070525_07_81/histograms.root");
  TFile *file7 = new TFile("070525_08_81/histograms.root");
  TFile *file8 = new TFile("070525_09_81/histograms.root");
  TFile *file9 = new TFile("070525_10_81/histograms.root");

//   string histoname = "rsSeedEfficiency/rsSeedingEfficiencyEta";
//   string histoname = "rsTrackingEfficiency_Hits/efficiencyEta";
  string histoname = "rsSeeds/nSeed";

  TH1D *histo1 = (TH1D*)file1->Get(histoname.c_str());
  TH1D *histo2 = (TH1D*)file2->Get(histoname.c_str());
  TH1D *histo3 = (TH1D*)file3->Get(histoname.c_str());
  TH1D *histo4 = (TH1D*)file4->Get(histoname.c_str());
  TH1D *histo5 = (TH1D*)file5->Get(histoname.c_str());
  TH1D *histo6 = (TH1D*)file6->Get(histoname.c_str());
  TH1D *histo7 = (TH1D*)file7->Get(histoname.c_str());
  TH1D *histo8 = (TH1D*)file8->Get(histoname.c_str());
  TH1D *histo9 = (TH1D*)file9->Get(histoname.c_str());

  unsigned int bins = 1000;
  histo1->Rebin(bins);
  histo2->Rebin(bins);
  histo3->Rebin(bins);
  histo4->Rebin(bins);
  histo5->Rebin(bins);
  histo6->Rebin(bins);
  histo7->Rebin(bins);
  histo8->Rebin(bins);
  histo9->Rebin(bins);

  histo1->SetLineColor(1);
  histo2->SetLineColor(2);
  histo3->SetLineColor(3);
  histo4->SetLineColor(4);
  histo5->SetLineColor(5);
  histo6->SetLineColor(6);
  histo7->SetLineColor(7);
  histo8->SetLineColor(8);
  histo9->SetLineColor(9);

  histo1->Draw("HIST");
  histo2->Draw("HIST,SAME");
  histo3->Draw("HIST,SAME");
  histo4->Draw("HIST,SAME");
  histo5->Draw("HIST,SAME");
  histo6->Draw("HIST,SAME");
  histo7->Draw("HIST,SAME");
  histo8->Draw("HIST,SAME");
  histo9->Draw("HIST,SAME");

  std::cout << histo1->GetMean() << " " << histo1->GetRMS() << std::endl;
  std::cout << histo2->GetMean() << " " << histo2->GetRMS() << std::endl;
  std::cout << histo3->GetMean() << " " << histo3->GetRMS() << std::endl;
  std::cout << histo4->GetMean() << " " << histo4->GetRMS() << std::endl;
  std::cout << histo5->GetMean() << " " << histo5->GetRMS() << std::endl;
  std::cout << histo6->GetMean() << " " << histo6->GetRMS() << std::endl;
  std::cout << histo7->GetMean() << " " << histo7->GetRMS() << std::endl;
  std::cout << histo8->GetMean() << " " << histo8->GetRMS() << std::endl;
  std::cout << histo9->GetMean() << " " << histo9->GetRMS() << std::endl;



}
