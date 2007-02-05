void drawCircleXY() {

  TCanvas *canvas = new TCanvas("circle","circle",820,720);


  canvas->SetLeftMargin(0.13);
  canvas->SetRightMargin(0.13);
  canvas->SetTopMargin(0.11);
  canvas->SetBottomMargin(0.11);

  canvas->SetFrameBorderMode(0);
  canvas->SetBorderMode(0);
  canvas->SetFillColor(0);

  canvas->SetGridx();
  canvas->SetGridy();

  canvas->Draw();

  TH2D *histo = new TH2D("histo","",100,-120,120,100,-120,120);
  histo->SetStats(kFALSE);
  histo->GetXaxis()->SetTitle("x [cm]");
  histo->GetXaxis()->SetTitleOffset(1.0);
  histo->GetXaxis()->SetTitleSize(0.05);
  histo->GetYaxis()->SetTitle("y [cm]");
  histo->GetYaxis()->SetTitleOffset(1.3);
  histo->GetYaxis()->SetTitleSize(0.05);

  //draw an axis on the right side
  TGaxis *axis_1 = new TGaxis(120,-120,120,120,
			      -120,120,510,"+L");
  axis_1->SetTitle("y [cm]");
  axis_1->SetTitleSize(0.05);
  axis_1->SetLabelSize(0.05);
  axis_1->SetTitleOffset(1.3);

  //draw an axis on the right side
  TGaxis *axis_2 = new TGaxis(-120,120,120,120,
			      -120,120,510,"-L");
  axis_2->SetTitle("x [cm]");
  axis_2->SetTitleSize(0.05);
  axis_2->SetLabelSize(0.05);
  axis_2->SetTitleOffset(1.0);

  TLine *line_1 = new TLine(-120,0,120,0);
  line_1->SetLineWidth(3);

  TLine *line_2 = new TLine(0,-120,0,120);
  line_2->SetLineWidth(3);

  histo->Draw();
  axis_1->Draw("SAME");
  axis_2->Draw("SAME");
  line_1->Draw("SAME");
  line_2->Draw("SAME");

drawCircleSeed(-126.082,-102.992,159.207,16.6521,-32.4664,17.813,-34.8664,20.1184,-39.9656);
drawCircleSeed(-1177.95,-611.813,1327.67,16.6521,-32.4664,17.813,-34.8664,25.1354,-50.2953);
drawCircleSeed(486.064,179.956,515.238,16.6521,-32.4664,20.1184,-39.9656,25.1354,-50.2953);
drawCircleSeed(301.578,90.3532,310.166,17.813,-34.8664,20.1184,-39.9656,25.1354,-50.2953);

}

void drawCircleSeed(double x, double y, double r,
		    double x_1, double y_1,
		    double x_2, double y_2,
		    double x_3, double y_3) {

  TEllipse *circle = new TEllipse(x,y,r);
  circle->SetLineColor(4);
  circle->Draw("SAME");

  TMarker *point_1 = new TMarker(x_1,y_1,3);
  point_1->SetMarkerColor(2);
  point_1->SetMarkerSize(3);
  TMarker *point_2 = new TMarker(x_2,y_2,3);
  point_2->SetMarkerColor(2);
  point_2->SetMarkerSize(3);
  TMarker *point_3 = new TMarker(x_3,y_3,3);
  point_3->SetMarkerColor(2);
  point_3->SetMarkerSize(3);

  point_1->Draw("SAME");
  point_2->Draw("SAME");
  point_3->Draw("SAME");



}

void drawCircleSeed(double x, double y, double r,
		    double x_1, double y_1,
		    double x_2, double y_2,
		    double x_3, double y_3,
		    double x_4, double y_4) {

  TEllipse *circle = new TEllipse(x,y,r);
  circle->SetLineColor(4);
  circle->Draw("SAME");

  TMarker *point_1 = new TMarker(x_1,y_1,3);
  point_1->SetMarkerColor(2);
  point_1->SetMarkerSize(3);
  TMarker *point_2 = new TMarker(x_2,y_2,3);
  point_2->SetMarkerColor(2);
  point_2->SetMarkerSize(3);
  TMarker *point_3 = new TMarker(x_3,y_3,3);
  point_3->SetMarkerColor(2);
  point_3->SetMarkerSize(3);
  TMarker *point_4 = new TMarker(x_4,y_4,3);
  point_4->SetMarkerColor(2);
  point_4->SetMarkerSize(3);

  point_1->Draw("SAME");
  point_2->Draw("SAME");
  point_3->Draw("SAME");
  point_4->Draw("SAME");



}
