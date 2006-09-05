bool createPlot(TString name, TCanvas *canvas, TFile *file, TH1* histo1, TH1* histo2) {

  // chi2 plot
  TString rs_name = "rs_";
  rs_name.Append(name);
  histo1 = (TH1*)file->Get(rs_name);
  TString ckf_name = "ckf_";
  ckf_name.Append(name);
  histo2 = (TH1*)file->Get(ckf_name);

  TString name1 = histo1->GetName();
  name1.Append(" - RS");
  histo1->SetName(name1);
  histo1->SetLineColor(2);
  TString name2 = histo2->GetName();
  name2.Append(" - Ckf");
  histo2->SetName(name2);
  histo2->SetLineColor(4);

  histo1->Draw();
  histo2->Draw("SAMES");
  canvas->Update();
  TPaveStats *stats = (TPaveStats*)(histo2->GetListOfFunctions()->FindObject("stats"));
  stats->SetX1NDC(0.778736);
  stats->SetY1NDC(0.508475);
  stats->SetX2NDC(0.979885);
  stats->SetY2NDC(0.747881);
  histo2->Draw("SAMES");
  TLegend *leg = new TLegend(0.693966,0.872881,0.774425,0.993644);
  leg->SetTextSize(0.035);
  leg->SetFillColor(10); 
  leg->SetBorderSize(1); // no frame, no shadow
  leg->AddEntry(histo1, "RS" , "L" );
  leg->AddEntry(histo2, "Ckf" , "L" );
  leg->Draw("SAME");
  TString filename = name;
  filename.Append(".eps");
  canvas->Print(filename);
}

void compare_rs_ckf() {

  gROOT->SetBatch(kTRUE);

  gStyle->SetOptStat(111111);

  TCanvas *canvas = new TCanvas;

  TFile *file = new TFile("validation_1428_0.root");

  TH1 *histo1;
  TH1 *histo2;

  createPlot("chi2",canvas,file,histo1,histo2);
  createPlot("pt",canvas,file,histo1,histo2);
  createPlot("nhit",canvas,file,histo1,histo2);
  createPlot("eta",canvas,file,histo1,histo2);
  createPlot("phi",canvas,file,histo1,histo2);
  createPlot("transCurv",canvas,file,histo1,histo2);
  createPlot("phiZero",canvas,file,histo1,histo2);
  createPlot("theta",canvas,file,histo1,histo2);
  createPlot("dZero",canvas,file,histo1,histo2);
  createPlot("dZ",canvas,file,histo1,histo2);
  createPlot("nhitVsEta",canvas,file,histo1,histo2);

  



}
