{

  TCanvas canvas;

  TString histogram_name = "ckf_nhit";

  TFile file1("track-analysis_1.root");

  TH1D *histo = (TH1D*)(file1.Get(histogram_name));
  
  TFile file2("track-analysis_2.root");

  histo->Add((TH1D*)(file2.Get(histogram_name)));

  TFile file3("track-analysis_3.root");

  histo->Add((TH1D*)(file3.Get(histogram_name)));

  TFile file4("track-analysis_4.root");

  histo->Add((TH1D*)(file4.Get(histogram_name)));

  TFile file5("track-analysis_5.root");

  histo->Add((TH1D*)(file5.Get(histogram_name)));

  TFile file6("track-analysis_6.root");

  histo->Add((TH1D*)(file6.Get(histogram_name)));

  TFile file7("track-analysis_7.root");

  histo->Add((TH1D*)(file7.Get(histogram_name)));

  TFile file8("track-analysis_8.root");

  histo->Add((TH1D*)(file8.Get(histogram_name)));

  TFile file9("track-analysis_9.root");

  histo->Add((TH1D*)(file9.Get(histogram_name)));

  TFile file10("track-analysis_10.root");

  histo->Add((TH1D*)(file10.Get(histogram_name)));

  histo->Draw();
  canvas.SaveAs("nhit.eps");
  canvas.SaveAs("nhit.png");

  histogram_name = "ckf_pt";

  histo = (TH1D*)(file1.Get(histogram_name));
  
  histo->Add((TH1D*)(file2.Get(histogram_name)));

  histo->Add((TH1D*)(file3.Get(histogram_name)));

  histo->Add((TH1D*)(file4.Get(histogram_name)));

  histo->Add((TH1D*)(file5.Get(histogram_name)));

  histo->Add((TH1D*)(file6.Get(histogram_name)));

  histo->Add((TH1D*)(file7.Get(histogram_name)));

  histo->Add((TH1D*)(file8.Get(histogram_name)));

  histo->Add((TH1D*)(file9.Get(histogram_name)));

  histo->Add((TH1D*)(file10.Get(histogram_name)));

  histo->Draw();
  canvas.SaveAs("pt.eps");
  canvas.SaveAs("pt.png");

  histogram_name = "ckf_eta";

  histo = (TH1D*)(file1.Get(histogram_name));
  
  histo->Add((TH1D*)(file2.Get(histogram_name)));

  histo->Add((TH1D*)(file3.Get(histogram_name)));

  histo->Add((TH1D*)(file4.Get(histogram_name)));

  histo->Add((TH1D*)(file5.Get(histogram_name)));

  histo->Add((TH1D*)(file6.Get(histogram_name)));

  histo->Add((TH1D*)(file7.Get(histogram_name)));

  histo->Add((TH1D*)(file8.Get(histogram_name)));

  histo->Add((TH1D*)(file9.Get(histogram_name)));

  histo->Add((TH1D*)(file10.Get(histogram_name)));

  histo->Draw();
  canvas.SaveAs("eta.eps");
  canvas.SaveAs("eta.png");

  histogram_name = "ckf_numTracks";

  histo = (TH1D*)(file1.Get(histogram_name));
  
  histo->Add((TH1D*)(file2.Get(histogram_name)));

  histo->Add((TH1D*)(file3.Get(histogram_name)));

  histo->Add((TH1D*)(file4.Get(histogram_name)));

  histo->Add((TH1D*)(file5.Get(histogram_name)));

  histo->Add((TH1D*)(file6.Get(histogram_name)));

  histo->Add((TH1D*)(file7.Get(histogram_name)));

  histo->Add((TH1D*)(file8.Get(histogram_name)));

  histo->Add((TH1D*)(file9.Get(histogram_name)));

  histo->Add((TH1D*)(file10.Get(histogram_name)));

  histo->Draw();

}
