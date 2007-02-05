TCanvas *canvas = new TCanvas;

void compareTwoHistograms(std::string fileName1) {

  // open files
  TFile *file1 = new TFile(fileName1.c_str());
  if ( file1->IsZombie() ) {
    std::cout << "File: " << fileName1 << " cannot be opened!" << std::endl;
    return;
  }

  // define histograms to compare
  std::vector<std::string> histos1;
  std::vector<std::string> histos2;
//  histos1.push_back("rsTrackingEfficiency_Hits/efficiencyEta");
//  histos2.push_back("ckfTrackingEfficiency_Hits/efficiencyEta");

  histos1.push_back("rsSeeds/nSeed");
  histos2.push_back("ckfSeeds/nSeed");

//  histos1.push_back("rsTracks/nTracks");
//  histos2.push_back("ckfTracks/nTracks");

//  histos1.push_back("rsTracks/eta");
//  histos2.push_back("ckfTracks/eta");

//  histos1.push_back("rsTracks/pt");
//  histos2.push_back("ckfTracks/pt");

//  histos1.push_back("rsTracks/nhit");
//  histos2.push_back("ckfTracks/nhit");

  for ( unsigned int i = 0;
	i < histos1.size();
	++i ) {

    std::cout << "compare " << histos1[i] << " and " << histos2[i] << std::endl;

    TH1 *histo1 = (TH1*)file1->Get(histos1[i].c_str());
    if ( !histo1 ) {
      std::cout << "Histogram: " << histos1[i] << " cannot be read from file: " << fileName1 << std::endl;
      continue;
    }

    TH1 *histo2 = (TH1*)file1->Get(histos2[i].c_str());
    if ( histo2 == 0 ) {
      std::cout << "Histogram: " << histos2[i] << " cannot be read from file: " << fileName1 << std::endl;
      continue;
    }

    // make comparison plot
    compareTwoHistograms(histos1[i],histos2[i], histo1,histo2,"HIST");
    
  }

}

void compareTwoHistograms(std::string name1, 
			  std::string name2, 
			  TH1 *histo1, 
			  TH1* histo2,
			  std::string option) {

  canvas->Clear();

  canvas->SetRightMargin(0.21);

  // rescale
  TString rootName(name1.c_str());
  if ( rootName.Contains("nSeed") ) {
     histo1->GetXaxis()->SetRange(0,15000);
     histo2->GetXaxis()->SetRange(0,15000);
//    histo1->GetXaxis()->SetRange(0,40);
//    histo2->GetXaxis()->SetRange(0,40);
  } else if ( rootName.Contains("nTracks") ) {
     histo1->GetXaxis()->SetRange(0,100);
     histo2->GetXaxis()->SetRange(0,100);
//    histo1->GetXaxis()->SetRange(0,4);
//    histo2->GetXaxis()->SetRange(0,4);
  }

  histo1->SetLineColor(2);
  histo1->SetLineWidth(2);
  std::string histo1Name = "File 1 ";
  histo1Name += name1;
  histo1->SetName(histo1Name.c_str());
  histo2->SetLineColor(4);
  histo2->SetLineWidth(2);
  std::string histo2Name = "File 2 ";
  histo2Name += name2;
  histo2->SetName(histo2Name.c_str());

  TString option1(option.c_str());
  option1.Append(".SAMES");
  TString option2(option.c_str());

  if ( histo1->GetMaximum() > histo2->GetMaximum() ) {
    histo2->SetMaximum(histo1->GetMaximum());
  }

  histo2->Draw(option2);
  histo1->Draw(option1);

  canvas->Update();

  TPaveStats *stats1 = (TPaveStats*)(histo1->GetListOfFunctions()->FindObject("stats"));
  stats1->SetX1NDC(0.80);
  stats1->SetY1NDC(0.80);
  stats1->SetX2NDC(0.995);
  stats1->SetY2NDC(0.995);
  stats1->SetOptStat(111111);
  stats1->SetLineColor(2);
  stats1->SetTextColor(2);

  TPaveStats *stats2 = (TPaveStats*)(histo2->GetListOfFunctions()->FindObject("stats"));
  stats2->SetX1NDC(0.80);
  stats2->SetY1NDC(0.60);
  stats2->SetX2NDC(0.995);
  stats2->SetY2NDC(0.795);
  stats2->SetOptStat(111111);
  stats2->SetLineColor(4);
  stats2->SetTextColor(4);

  histo2->Draw(option2);
  histo1->Draw(option1);

  // clean name for picture filename
  TString filename(name1.c_str());
  filename.Append("_");
  filename += name2.c_str();
  filename.ReplaceAll("/","_");
  filename.Append(".png");

  canvas->SaveAs(filename);

}
