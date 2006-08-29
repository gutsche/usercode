bool createPlot(TString name, TCanvas *canvas, TFile *file1, TFile *file2, TH1F* histo1, TH1F* histo2) {

  // chi2 plot
  histo1 = (TH1F*)file1->Get(name);
  histo2 = (TH1F*)file2->Get(name);

  TString name1 = histo1->GetName();
  name1.Append(" - File 1");
  histo1->SetName(name1);
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
  histo2->Draw("SAMES");
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

  TFile *file1 = new TFile("before/validate-minbias.root");
  TFile *file2 = new TFile("after/validate-minbias.root");

  TH1F *histo1;
  TH1F *histo2;

  createPlot("chi2",canvas,file1,file2,histo1,histo2);
  createPlot("pt",canvas,file1,file2,histo1,histo2);
  createPlot("nhit",canvas,file1,file2,histo1,histo2);
  createPlot("eta",canvas,file1,file2,histo1,histo2);
  createPlot("phi",canvas,file1,file2,histo1,histo2);
  createPlot("transCurv",canvas,file1,file2,histo1,histo2);
  createPlot("phiZero",canvas,file1,file2,histo1,histo2);
  createPlot("theta",canvas,file1,file2,histo1,histo2);
  createPlot("dZero",canvas,file1,file2,histo1,histo2);
  createPlot("dZ",canvas,file1,file2,histo1,histo2);

  



}
