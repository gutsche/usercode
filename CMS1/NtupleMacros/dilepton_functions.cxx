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

TCanvas* plotStack(string draw_command, TCut cut, string title, string hname = "histo")
{ 
   c1 = new TCanvas("c1","c1",500,500);
   THStack* stack = new THStack(hname.c_str(),title.c_str());
   legend = new TLegend(0.75,0.65,0.98,0.98,"Key");

   int color = 1;
   // Scales:
   // http://hepuser.ucsd.edu/twiki/bin/view/CMSPhysics/PassOnePlots
   // 
   hww = new TH1F((hname+"_ww").c_str(),(hname+"_ww").c_str(),5,0,5);
   tww->Draw((draw_command+">>"+hname+"_ww").c_str(), cut);
   hww->SetFillColor(++color);
   // scale to 100 pb^-1
   hww->Scale(0.116);
   stack->Add(hww);
   legend->AddEntry(hww,"WW");

   hwz = new TH1F((hname+"_wz").c_str(),(hname+"_wz").c_str(),5,0,5);
   twz->Draw((draw_command+">>"+hname+"_wz").c_str(), cut);
   hwz->SetFillColor(++color);
   // scale to 100 pb^-1
   hwz->Scale(0.0526);
   stack->Add(hwz);
   legend->AddEntry(hwz,"WZ");

   hzz = new TH1F((hname+"_zz").c_str(),(hname+"_zz").c_str(),5,0,5);
   tzz->Draw((draw_command+">>"+hname+"_zz").c_str(), cut);
   hzz->SetFillColor(++color);
   // scale to 100 pb^-1
   hzz->Scale(0.0496);
   stack->Add(hzz);
   legend->AddEntry(hzz,"ZZ");

   htt = new TH1F((hname+"_tt").c_str(),(hname+"_tt").c_str(),5,0,5);
   ttt->Draw((draw_command+">>"+hname+"_tt").c_str(), cut);
   htt->SetFillColor(++color);
   // scale to 100 pb^-1
   htt->Scale(0.113);
   stack->Add(htt);
   legend->AddEntry(htt,"TTbar");
   
   hzee = new TH1F((hname+"_zee").c_str(),(hname+"_zee").c_str(),5,0,5);
   tzee->Draw((draw_command+">>"+hname+"_zee").c_str(), cut);
   hzee->SetFillColor(++color);
   // scale to 100 pb^-1
   hzee->Scale(5.15);
   stack->Add(hzee);
   legend->AddEntry(hzee,"Zee");

   hzmm = new TH1F((hname+"_zmm").c_str(),(hname+"_zmm").c_str(),5,0,5);
   tzmm->Draw((draw_command+">>"+hname+"_zmm").c_str(), cut);
   hzmm->SetFillColor(++color);
   // scale to 100 pb^-1
   hzmm->Scale(4.36);
   stack->Add(hzmm);
   legend->AddEntry(hzmm,"Zmumu");

   
   stack->Draw("hist");
   legend->Draw();
   return c1;
}
