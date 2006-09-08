double GetYMaximum (TH1 *f1, TH1 *f2) {
  double max = 0;
  double old = f1->GetBinContent(f1->GetMaximumBin());
  double new = f2->GetBinContent(f2->GetMaximumBin());
  (old>new) ? max=old : max=new;
  return max;
}

bool createPlot(TString name, TCanvas *canvas, TFile *file, TH1* histo1, TH1* histo2, Int_t do_fit) {

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
  histo1->SetMaximum(GetYMaximum(histo1,histo2)+10);
  if(do_fit) histo1->Fit("gaus","0");

  histo2->Draw();
  if(do_fit) histo2->Fit("gaus","0");

  histo1->Draw();
  histo2->Draw("sames");

  canvas->Update();
  gPad->Update();
  
  TPaveStats *st1 = (TPaveStats*)(histo1->GetListOfFunctions()->FindObject("stats"));
  st1->SetX1NDC(0.77);
  st1->SetY1NDC(0.64);
  st1->SetX2NDC(0.98);
  st1->SetY2NDC(0.98);

  Double_t defaulth = st1->GetY2NDC() - st1->GetY1NDC();
  Double_t gaph = 0.02;
  TPaveStats *st2 = (TPaveStats*)(histo2->GetListOfFunctions()->FindObject("stats"));
  st2->SetX1NDC(0.77);  
  st2->SetY1NDC(st1->GetY1NDC() - 1.0*defaulth);
  st2->SetX2NDC(0.98);    
  st2->SetY2NDC(st1->GetY1NDC() - gaph);


  canvas->Update();

  TLegend *leg = new TLegend(0.69,0.86,0.76,0.98);
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
  gStyle->SetOptFit(1);
  TCanvas *canvas = new TCanvas;

  TFile *file = new TFile("validation_147_0.root");

  TH1 *histo1;
  TH1 *histo2;
  
  const int do_gauss_fit = 1;
  const int do_not_gauss_fit = 0;

  createPlot("chi2",canvas,file,histo1,histo2,do_gauss_fit);
  createPlot("pt",canvas,file,histo1,histo2,do_gauss_fit);
  createPlot("nhit",canvas,file,histo1,histo2,do_gauss_fit);
  createPlot("eta",canvas,file,histo1,histo2,do_not_gauss_fit);
  createPlot("phi",canvas,file,histo1,histo2,do_not_gauss_fit);
  createPlot("transCurv",canvas,file,histo1,histo2,do_gauss_fit);
  createPlot("phiZero",canvas,file,histo1,histo2,do_gauss_fit);
  createPlot("theta",canvas,file,histo1,histo2,do_not_gauss_fit);
  createPlot("dZero",canvas,file,histo1,histo2,do_gauss_fit);
  createPlot("dZ",canvas,file,histo1,histo2,do_gauss_fit);
 
}
