{
   gStyle->SetOptStat(0);
   c1 = new TCanvas("c1","c1",1200,800);
   c1->Divide(3,2);
   
   int i=0;
   c1->cd(++i);
   tzmm->Draw("hyp_met/hyp_p4.pt():fabs(fabs(hyp_metPhi-hyp_p4.phi())-3.1416)>>hzmm(40,0,3.14,40,0,5)",TCut("hyp_met>20"),"colz");
   hzmm->SetTitle("Zmumu");
   hzmm->GetXaxis()->SetTitle("|dPhi|");
   hzmm->GetYaxis()->SetTitle("MET/Pt");
   hzmm->Draw("colz");

   c1->cd(++i);
   tzee->Draw("hyp_met/hyp_p4.pt():fabs(fabs(hyp_metPhi-hyp_p4.phi())-3.1416)>>hzee(40,0,3.14,40,0,5)",TCut("hyp_met>20"),"colz");
   hzee->SetTitle("Zee");
   hzee->GetXaxis()->SetTitle("|dPhi|");
   hzee->GetYaxis()->SetTitle("MET/Pt");
   hzee->Draw("colz");

   c1->cd(++i);
   ttt->Draw("hyp_met/hyp_p4.pt():fabs(fabs(hyp_metPhi-hyp_p4.phi())-3.1416)>>htt(40,0,3.14,40,0,5)",TCut("hyp_met>20"),"colz");
   htt->SetTitle("TTbar");
   htt->GetXaxis()->SetTitle("|dPhi|");
   htt->GetYaxis()->SetTitle("MET/Pt");
   htt->Draw("colz");

   c1->cd(++i);
   tww->Draw("hyp_met/hyp_p4.pt():fabs(fabs(hyp_metPhi-hyp_p4.phi())-3.1416)>>hww(40,0,3.14,40,0,5)",TCut("hyp_met>20"),"colz");
   hww->SetTitle("WW");
   hww->GetXaxis()->SetTitle("|dPhi|");
   hww->GetYaxis()->SetTitle("MET/Pt");
   hww->Draw("colz");
   
   c1->cd(++i);
   twz->Draw("hyp_met/hyp_p4.pt():fabs(fabs(hyp_metPhi-hyp_p4.phi())-3.1416)>>hwz(40,0,3.14,40,0,5)",TCut("hyp_met>20"),"colz");
   hwz->SetTitle("WZ");
   hwz->GetXaxis()->SetTitle("|dPhi|");
   hwz->GetYaxis()->SetTitle("MET/Pt");
   hwz->Draw("colz");
   
   c1->cd(++i);
   tzz->Draw("hyp_met/hyp_p4.pt():fabs(fabs(hyp_metPhi-hyp_p4.phi())-3.1416)>>hzz(40,0,3.14,40,0,5)",TCut("hyp_met>20"),"colz");
   hzz->SetTitle("ZZ");
   hzz->GetXaxis()->SetTitle("|dPhi|");
   hzz->GetYaxis()->SetTitle("MET/Pt");
   hzz->Draw("colz");

}
   
