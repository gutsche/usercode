TCanvas* plot(const char* draw_command, TCut cut)
{
   c1 = new TCanvas("c1","c1",1600,800);
   c1->Divide(4,2);
   c1->cd(1);
   tzmm->Draw(draw_command, cut);
   htemp->SetTitle("Zmumu"); htemp->Draw();
   c1->cd(2);
   tzee->Draw(draw_command, cut);
   htemp->SetTitle("Zee"); htemp->Draw();
   c1->cd(3);
   tztt->Draw(draw_command, cut);
   htemp->SetTitle("Ztautau"); htemp->Draw();
   c1->cd(4);
   tzz->Draw(draw_command, cut);
   htemp->SetTitle("ZZ"); htemp->Draw();
   c1->cd(5);
   tww->Draw(draw_command, cut);
   htemp->SetTitle("WW"); htemp->Draw();
   c1->cd(6);
   twz->Draw(draw_command, cut);
   htemp->SetTitle("WZ"); htemp->Draw();
   c1->cd(7);
   thww->Draw(draw_command, cut);
   htemp->SetTitle("HWW"); htemp->Draw();
   c1->cd(8);
   ttt->Draw(draw_command, cut);
   htemp->SetTitle("TTbar"); htemp->Draw();
   return c1;
}

TCanvas* plotStack(string draw_command, TCut cut, string title, string name = "histo")
{ 
   c1 = new TCanvas(name.c_str(),name.c_str(),500,500);
   THStack* stack = new THStack(name.c_str(),title.c_str());
   legend = new TLegend(0.75,0.65,0.98,0.98,"Key");

   int color = 1;
   // Scales:
   // http://hepuser.ucsd.edu/twiki/bin/view/CMSPhysics/PassOnePlots
   // 
   hww = new TH1F((name+"_ww").c_str(),(name+"_ww").c_str(),5,0,5);
   tww->Draw((draw_command+">>"+name+"_ww").c_str(), cut);
   hww->SetFillColor(++color);
   // scale to 100 pb^-1
   hww->Scale(0.116);
   stack->Add(hww);
   legend->AddEntry(hww,"WW");

   hwz = new TH1F((name+"_wz").c_str(),(name+"_wz").c_str(),5,0,5);
   twz->Draw((draw_command+">>"+name+"_wz").c_str(), cut);
   hwz->SetFillColor(++color);
   // scale to 100 pb^-1
   hwz->Scale(0.0526);
   stack->Add(hwz);
   legend->AddEntry(hwz,"WZ");

   hzz = new TH1F((name+"_zz").c_str(),(name+"_zz").c_str(),5,0,5);
   tzz->Draw((draw_command+">>"+name+"_zz").c_str(), cut);
   hzz->SetFillColor(++color);
   // scale to 100 pb^-1
   hzz->Scale(0.0496);
   stack->Add(hzz);
   legend->AddEntry(hzz,"ZZ");

   htt = new TH1F((name+"_tt").c_str(),(name+"_tt").c_str(),5,0,5);
   ttt->Draw((draw_command+">>"+name+"_tt").c_str(), cut);
   htt->SetFillColor(++color);
   // scale to 100 pb^-1
   htt->Scale(0.113);
   stack->Add(htt);
   legend->AddEntry(htt,"TTbar");
   
   hzee = new TH1F((name+"_zee").c_str(),(name+"_zee").c_str(),5,0,5);
   tzee->Draw((draw_command+">>"+name+"_zee").c_str(), cut);
   hzee->SetFillColor(++color);
   // scale to 100 pb^-1
   hzee->Scale(5.15);
   stack->Add(hzee);
   legend->AddEntry(hzee,"Zee");

   hzmm = new TH1F((name+"_zmm").c_str(),(name+"_zmm").c_str(),5,0,5);
   tzmm->Draw((draw_command+">>"+name+"_zmm").c_str(), cut);
   hzmm->SetFillColor(++color);
   // scale to 100 pb^-1
   hzmm->Scale(4.36);
   stack->Add(hzmm);
   legend->AddEntry(hzmm,"Zmumu");
   
   stack->Draw("hist");
   legend->Draw();
   return c1;
}

TH1F* getSinglePlot(int n, float xmin, float xmax, TTree* tree, TCut cut,
		    std::string name, std::string draw_command, 
		    std::string title)
{
   if (draw_command.length() == 0) return 0;
   TH1F* h = new TH1F(name.c_str(),name.c_str(),n,xmin,xmax);
   h->SetLineWidth(2);
   tree->Draw((draw_command+">>"+name).c_str(), cut,"goff");
   h->SetTitle(title.c_str());
   return h;
}

void drawHistograms( TH1F* h1, TH1F* h2 = 0, TH1F* h3 = 0 )
{
   float maxY = h1->GetMaximum();
   if (h2 && h2->GetMaximum()>maxY)  maxY = h2->GetMaximum();
   if (h3 && h3->GetMaximum()>maxY)  maxY = h3->GetMaximum();
   h1->SetMaximum( maxY );
   h1->Draw();
   if (h2) {
      h2->SetMaximum( maxY );
      h2->SetLineColor ( kBlue );
      h2->Draw("same");
   }
   if (h3) {
      h3->SetMaximum( maxY );
      h3->SetLineColor ( kRed );
      h3->Draw("same");
   }
}

TCanvas* plotOverlaid(int n, float xmin, float xmax, TCut cut, 
		      std::string draw_command1, 
		      std::string draw_command2 = "", 
		      std::string draw_command3 = "" )
{
   c1 = new TCanvas("c1","c1",1600,800);
   c1->Divide(4,2);
   c1->cd(1);
   drawHistograms( getSinglePlot(n,xmin,xmax,tzmm,cut,"h11",draw_command1,"Zmumu"),
		   getSinglePlot(n,xmin,xmax,tzmm,cut,"h12",draw_command2,"Zmumu"),
		   getSinglePlot(n,xmin,xmax,tzmm,cut,"h13",draw_command3,"Zmumu") );
   
   c1->cd(2);
   drawHistograms( getSinglePlot(n,xmin,xmax,tzee,cut,"h21",draw_command1,"Zee"),
		   getSinglePlot(n,xmin,xmax,tzee,cut,"h22",draw_command2,"Zee"),
		   getSinglePlot(n,xmin,xmax,tzee,cut,"h23",draw_command3,"Zee") );

   c1->cd(3);
   drawHistograms( getSinglePlot(n,xmin,xmax,tztt,cut,"h31",draw_command1,"Ztautau"),
		   getSinglePlot(n,xmin,xmax,tztt,cut,"h32",draw_command2,"Ztautau"),
		   getSinglePlot(n,xmin,xmax,tztt,cut,"h33",draw_command3,"Ztautau") );

   c1->cd(4);
   drawHistograms( getSinglePlot(n,xmin,xmax,tzz,cut,"h41",draw_command1,"ZZ"),
		   getSinglePlot(n,xmin,xmax,tzz,cut,"h42",draw_command2,"ZZ"),
		   getSinglePlot(n,xmin,xmax,tzz,cut,"h43",draw_command3,"ZZ") );
   
   c1->cd(5);
   drawHistograms( getSinglePlot(n,xmin,xmax,tww,cut,"h51",draw_command1,"WW"),
		   getSinglePlot(n,xmin,xmax,tww,cut,"h52",draw_command2,"WW"),
		   getSinglePlot(n,xmin,xmax,tww,cut,"h53",draw_command3,"WW") );
   
   c1->cd(6);
   drawHistograms( getSinglePlot(n,xmin,xmax,twz,cut,"h61",draw_command1,"WZ"),
		   getSinglePlot(n,xmin,xmax,twz,cut,"h62",draw_command2,"WZ"),
		   getSinglePlot(n,xmin,xmax,twz,cut,"h63",draw_command3,"WZ") );

   c1->cd(7);
   drawHistograms( getSinglePlot(n,xmin,xmax,thww,cut,"h71",draw_command1,"HWW"),
		   getSinglePlot(n,xmin,xmax,thww,cut,"h72",draw_command2,"HWW"),
		   getSinglePlot(n,xmin,xmax,thww,cut,"h73",draw_command3,"HWW") );

   c1->cd(8);
   drawHistograms( getSinglePlot(n,xmin,xmax,ttt,cut,"h81",draw_command1,"TTbar"),
		   getSinglePlot(n,xmin,xmax,ttt,cut,"h82",draw_command2,"TTbar"),
		   getSinglePlot(n,xmin,xmax,ttt,cut,"h83",draw_command3,"TTbar") );
   return c1;
}

test_cuts(TCut& cut1, TCut& cut2)
{
   cout << "Cut 1: " << cut1.GetTitle() << endl;
   cout << "Cut 2: " << cut2.GetTitle() << endl;
   
   int n1 = tzmm->Draw("hyp_met",cut1,"goff");
   int n2 = tzmm->Draw("hyp_met",cut2,"goff");
   cout << "Zmumu:          Eff: " << n2/(n1+1e-5) << "\t N1: " << n1 << "\t N2: " << n2 << endl;
   
   n1 = tzee->Draw("hyp_met",cut1,"goff");
   n2 = tzee->Draw("hyp_met",cut2,"goff");
   cout << "Zee:            Eff: " << n2/(n1+1e-5) << "\t N1: " << n1 << "\t N2: " << n2 << endl;

   n1 = tztt->Draw("hyp_met",cut1,"goff");
   n2 = tztt->Draw("hyp_met",cut2,"goff");
   cout << "Ztautau:        Eff: " << n2/(n1+1e-5) << "\t N1: " << n1 << "\t N2: " << n2 << endl;
   
   n1 = tzz->Draw("hyp_met",cut1,"goff");
   n2 = tzz->Draw("hyp_met",cut2,"goff");
   cout << "ZZ:             Eff: " << n2/(n1+1e-5) << "\t N1: " << n1 << "\t N2: " << n2 << endl;

   n1 = tww->Draw("hyp_met",cut1,"goff");
   n2 = tww->Draw("hyp_met",cut2,"goff");
   cout << "WW:             Eff: " << n2/(n1+1e-5) << "\t N1: " << n1 << "\t N2: " << n2 << endl;

   n1 = twz->Draw("hyp_met",cut1,"goff");
   n2 = twz->Draw("hyp_met",cut2,"goff");
   cout << "WZ:             Eff: " << n2/(n1+1e-5) << "\t N1: " << n1 << "\t N2: " << n2 << endl;

   n1 = thww->Draw("hyp_met",cut1,"goff");
   n2 = thww->Draw("hyp_met",cut2,"goff");
   cout << "HWW:            Eff: " << n2/(n1+1e-5) << "\t N1: " << n1 << "\t N2: " << n2 << endl;

   n1 = ttt->Draw("hyp_met",cut1,"goff");
   n2 = ttt->Draw("hyp_met",cut2,"goff");
   cout << "TTbar:          Eff: " << n2/(n1+1e-5) << "\t N1: " << n1 << "\t N2: " << n2 << endl;
}
