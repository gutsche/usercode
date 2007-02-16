TCanvas *canvas = new TCanvas;

void compareHistogramFiles(std::string *filenames, unsigned int num) {

  // open files
  TFile *files[10];
  for ( unsigned int counter = 0;
	counter < num;
	++counter ) {
    TFile *file = new TFile(filenames[counter].c_str());
    if ( file->IsZombie() ) {
      std::cout << "File: " << filenames[counter] << " cannot be opened!" << std::endl;
      return;
    } else {
      files[counter] = file;
    }
  }

  // define histograms to compare
  std::vector<std::string> histonames;
//   histonames.push_back("rsTracks/eta");
//   histonames.push_back("rsTracks/nTracks");
//   histonames.push_back("rsTracks/chi2");
//   histonames.push_back("rsTracks/nhit");
//   histonames.push_back("rsTracks/phi");
//   histonames.push_back("rsTracks/pt");
  histonames.push_back("rsTrackingEfficiency_Hits/efficiencyEta");
//   histonames.push_back("rsTrackingEfficiency_Hits/purityEta");


  for ( unsigned int counter = 0;
	counter < histonames.size();
	++counter ) {
    TH1 *histos[10];
    for ( unsigned int counter2 = 0;
	  counter2 < num;
	  ++counter2 ) {
      TH1 *histo = (TH1*)files[counter2]->Get(histonames[counter].c_str());
      if ( !histo ) {
	std::cout << "Histogram: " << histonames[counter] << " cannot be read from file: " << filenames[counter2] << std::endl;
	return;
      } else {
	histos[counter2] = histo;
      }
    }

    // make comparison plot
    compareHistograms(histonames[counter],histos,num);
    
  }

}

void compareHistograms(std::string name, TH1 **histos, unsigned int num) {

  canvas->Clear();

  canvas->SetRightMargin(0.21);

  double maximum = 0;

  for ( unsigned int counter = 0;
	counter < num;
	++counter ) {
    double temp = histos[counter]->GetMaximum();
    if ( temp > maximum ) {
      maximum = temp;
    }
    histos[counter]->SetLineColor((counter+1)*2);
    histos[counter]->SetLineWidth(2);
    std::string histoname = "File ";
    histoname += counter;
    histoname.append(" ");
    histoname += name;
    histos[counter]->SetName(histoname.c_str());
  }

  bool first = true;
  for ( unsigned int counter = 0;
	counter < num;
	++counter ) {
    histos[counter]->SetMaximum(maximum*1.1);
    if ( first ) {
      histos[counter]->Draw("HIST");
      first = false;
    } else {
      histos[counter]->Draw("HIST,SAMES");
    }
  }

  canvas->Update();

  for ( unsigned int counter = 0;
	counter < num;
	++counter ) {
    TPaveStats *stats = (TPaveStats*)(histos[counter]->GetListOfFunctions()->FindObject("stats"));
    stats->SetX1NDC(0.80);
    stats->SetY1NDC(0.80-counter*0.2);
    stats->SetX2NDC(0.995);
    stats->SetY2NDC(0.995-counter*0.2);
    stats->SetOptStat(111111);
    stats->SetLineColor(2*(counter+1));
    stats->SetTextColor(2*(counter+1));
  }

  first = true;
  for ( unsigned int counter = 0;
	counter < num;
	++counter ) {
    if ( first ) {
      histos[counter]->Draw("HIST");
      first = false;
    } else {
      histos[counter]->Draw("HIST,SAMES");
    }
  }

  // clean name for picture filename
  TString filename(name.c_str());
  filename.ReplaceAll("/","_");
  filename.Append(".png");

  canvas->SaveAs(filename);
  
}
