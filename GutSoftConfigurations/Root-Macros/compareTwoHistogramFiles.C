TCanvas *canvas = new TCanvas;

void compareTwoHistogramFiles(std::string fileName1, 
			      std::string fileName2) {

  // open files
  TFile *file1 = new TFile(fileName1.c_str());
  if ( file1->IsZombie() ) {
    std::cout << "File: " << fileName1 << " cannot be opened!" << std::endl;
  }
  TFile *file2 = new TFile(fileName2.c_str());
  if ( file2->IsZombie() ) {
    std::cout << "File: " << fileName2 << " cannot be opened!" << std::endl;
  }

  // define histograms to compare
  std::vector<std::string> histos;
  histos.push_back("rsTracks/eta");
  histos.push_back("rsTracks/nTracks");
  histos.push_back("rsTracks/chi2");
  histos.push_back("rsTracks/nhit");
  histos.push_back("rsTracks/phi");
  histos.push_back("rsTracks/pt");
  histos.push_back("rsTrackingEfficiency_Hits/efficiencyEta");
  histos.push_back("rsTrackingEfficiency_Hits/purityEta");

  for ( unsigned int i = 0;
	i < histos.size();
	++i ) {

    TH1 *histo1 = (TH1*)file1->Get(histos[i].c_str());
    if ( !histo1 ) {
      std::cout << "Histogram: " << histos[i] << " cannot be read from file: " << fileName1 << std::endl;
      continue;
    }

    TH1 *histo2 = (TH1*)file2->Get(histos[i].c_str());
    if ( histo2 == 0 ) {
      std::cout << "Histogram: " << histos[i] << " cannot be read from file: " << fileName2 << std::endl;
      continue;
    }

    // make comparison plot
    compareTwoHistograms(histos[i], histo1,histo2);
    
  }

}

void compareTwoHistograms(std::string name, TH1 *histo1, TH1* histo2) {

  canvas->Clear();

  canvas->SetRightMargin(0.21);

  // scale x axis of histograms if number of tracks
  TString rootName(name.c_str());
  if ( rootName.Contains("nTracks") ) {
    histo1->GetXaxis()->SetRange(0,100);
    histo2->GetXaxis()->SetRange(0,100);
  }

  histo1->SetLineColor(2);
  histo1->SetLineWidth(2);
  std::string histo1Name = "File 1 ";
  histo1Name += name;
  histo1->SetName(histo1Name.c_str());
  histo2->SetLineColor(4);
  histo2->SetLineWidth(2);
  std::string histo2Name = "File 2 ";
  histo2Name += name;
  histo2->SetName(histo2Name.c_str());

  histo1->Draw();
  histo2->Draw("SAMES");

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

  histo1->Draw();
  histo2->Draw("SAMES");

  // clean name for picture filename
  TString filename(name.c_str());
  filename.ReplaceAll("/","_");
  filename.Append(".png");

  canvas->SaveAs(filename);

  


  

}
