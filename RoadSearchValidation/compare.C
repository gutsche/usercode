double GetYMaximum (TH1 *f1, TH1 *f2) {
  double max = 0;
  double max1 = f1->GetBinContent(f1->GetMaximumBin());
  double max2 = f2->GetBinContent(f2->GetMaximumBin());
  (max1>max2) ? max=max1 : max=max2;
  return max;
}

bool createPlot(TString name, TCanvas *canvas, TFile *file1, TFile *file2, TH1F* histo1, TH1F* histo2, double scale1 = 1.0, double scale2 = 1.0) {

  // chi2 plot
  histo1 = (TH1F*)file1->Get(name);
  histo1->Scale(scale1);
  histo2 = (TH1F*)file2->Get(name);
  histo2->Scale(scale2);

  TString name1 = histo1->GetName();
  name1.Append(" - File 1");
  histo1->SetName(name1);
  histo1->SetMaximum(GetYMaximum(histo1,histo2)+10);
  histo1->Draw();
  histo1->SetLineColor(2);
  TString name2 = histo2->GetName();
  name2.Append(" - File 2");
  histo2->SetName(name2);
  histo2->Draw("SAMES");
  histo2->SetLineColor(4);
  canvas->Update();
  TPaveStats *stats = (TPaveStats*)(histo2->GetListOfFunctions()->FindObject("stats"));
  stats->SetX1NDC(0.778736);
  stats->SetY1NDC(0.508475);
  stats->SetX2NDC(0.979885);
  stats->SetY2NDC(0.747881);
  TLegend *leg = new TLegend(0.693966,0.872881,0.774425,0.993644);
  leg->SetTextSize(0.035);
  leg->SetFillColor(10); 
  leg->SetBorderSize(1); // no frame, no shadow
  leg->AddEntry(histo1, "File 1" , "L" );
  leg->AddEntry(histo2, "File 2" , "L" );
  leg->Draw("SAME");
  TString filename = name;
  filename.Append(".eps");
  canvas->Print(filename);
}

void compare () {

  gROOT->SetBatch(kTRUE);

  gStyle->SetOptStat(111111);

  TCanvas *canvas = new TCanvas;

  TFile *file1 = new TFile("060906_02/singlemu/validation_149_0.root");
  TFile *file2 = new TFile("060907_05/singlemu/validation_263_0.root");

  TH1F *histo1;
  TH1F *histo2;

  double scale1 = 1.0;
  double scale2 = 1.0;

  createPlot("rs_chi2",canvas,file1,file2,histo1,histo2,scale1,scale2);
  createPlot("rs_pt",canvas,file1,file2,histo1,histo2,scale1,scale2);
  createPlot("rs_nhit",canvas,file1,file2,histo1,histo2,scale1,scale2);
  createPlot("rs_eta",canvas,file1,file2,histo1,histo2,scale1,scale2);
  createPlot("rs_phi",canvas,file1,file2,histo1,histo2,scale1,scale2);
  createPlot("rs_transCurv",canvas,file1,file2,histo1,histo2,scale1,scale2);
  createPlot("rs_phiZero",canvas,file1,file2,histo1,histo2,scale1,scale2);
  createPlot("rs_theta",canvas,file1,file2,histo1,histo2,scale1,scale2);
  createPlot("rs_dZero",canvas,file1,file2,histo1,histo2,scale1,scale2);
  createPlot("rs_dZ",canvas,file1,file2,histo1,histo2,scale1,scale2);

  createPlot("ckf_chi2",canvas,file1,file2,histo1,histo2,scale1,scale2);
  createPlot("ckf_pt",canvas,file1,file2,histo1,histo2,scale1,scale2);
  createPlot("ckf_nhit",canvas,file1,file2,histo1,histo2,scale1,scale2);
  createPlot("ckf_eta",canvas,file1,file2,histo1,histo2,scale1,scale2);
  createPlot("ckf_phi",canvas,file1,file2,histo1,histo2,scale1,scale2);
  createPlot("ckf_transCurv",canvas,file1,file2,histo1,histo2,scale1,scale2);
  createPlot("ckf_phiZero",canvas,file1,file2,histo1,histo2,scale1,scale2);
  createPlot("ckf_theta",canvas,file1,file2,histo1,histo2,scale1,scale2);
  createPlot("ckf_dZero",canvas,file1,file2,histo1,histo2,scale1,scale2);
  createPlot("ckf_dZ",canvas,file1,file2,histo1,histo2,scale1,scale2);

}
