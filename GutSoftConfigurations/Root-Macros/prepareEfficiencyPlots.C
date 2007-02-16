void prepareEfficiencyPlots(std::string filename) {
  //
  // prepare efficiency plots and store them in input file
  //
  
  TFile *file = new TFile(filename.c_str(),"UPDATE");

  // list of histograms to calculcate efficiencies and purities from
  divideHistograms(file,
		   "rsTrackingEfficiency_Hits",
		   "eff_eta_trueNrec",
		   "eff_eta_true",
		   "efficiencyEta",
		   "Efficiency in #eta",
		   "Efficiency");
  divideHistograms(file,
		   "rsTrackingEfficiency_Hits",
		   "pur_eta_recNtrue",
		   "pur_eta_rec",
		   "purityEta",
		   "Purity in #eta",
		   "Purity");
  divideHistograms(file,
		   "rsTrackingEfficiency_Chi2",
		   "pur_eta_recNtrue",
		   "pur_eta_rec",
		   "purityEta",
		   "Purity in #eta",
		   "Purity");
  divideHistograms(file,
		   "rsTrackingEfficiency_Chi2",
		   "eff_eta_trueNrec",
		   "eff_eta_true",
		   "efficiencyEta",
		   "Efficiency in #eta",
		   "Efficiency");
  divideHistograms(file,
		   "ckfTrackingEfficiency_Hits",
		   "eff_eta_trueNrec",
		   "eff_eta_true",
		   "efficiencyEta",
		   "Efficiency in #eta",
		   "Efficiency");
  divideHistograms(file,
		   "ckfTrackingEfficiency_Hits",
		   "pur_eta_recNtrue",
		   "pur_eta_rec",
		   "purityEta",
		   "Purity in #eta",
		   "Purity");
  divideHistograms(file,
		   "ckfTrackingEfficiency_Chi2",
		   "pur_eta_recNtrue",
		   "pur_eta_rec",
		   "purityEta",
		   "Purity in #eta",
		   "Purity");
  divideHistograms(file,
		   "ckfTrackingEfficiency_Chi2",
		   "eff_eta_trueNrec",
		   "eff_eta_true",
		   "efficiencyEta",
		   "Efficiency in #eta",
		   "Efficiency");

  divideHistograms(file,
		   "rsTrackingEfficiency_Hits",
		   "eff_pt_trueNrec",
		   "eff_pt_true",
		   "efficiencyPt",
		   "Efficiency in p_{T}",
		   "Efficiency");
  divideHistograms(file,
		   "rsTrackingEfficiency_Hits",
		   "pur_pt_recNtrue",
		   "pur_pt_rec",
		   "purityPt",
		   "Purity in p_{T}",
		   "Purity");
  divideHistograms(file,
		   "rsTrackingEfficiency_Chi2",
		   "pur_pt_recNtrue",
		   "pur_pt_rec",
		   "purityPt",
		   "Purity in p_{T}",
		   "Purity");
  divideHistograms(file,
		   "rsTrackingEfficiency_Chi2",
		   "eff_pt_trueNrec",
		   "eff_pt_true",
		   "efficiencyPt",
		   "Efficiency in p_{T}",
		   "Efficiency");
  divideHistograms(file,
		   "ckfTrackingEfficiency_Hits",
		   "eff_pt_trueNrec",
		   "eff_pt_true",
		   "efficiencyPt",
		   "Efficiency in p_{T}",
		   "Efficiency");
  divideHistograms(file,
		   "ckfTrackingEfficiency_Hits",
		   "pur_pt_recNtrue",
		   "pur_pt_rec",
		   "purityPt",
		   "Purity in p_{T}",
		   "Purity");
  divideHistograms(file,
		   "ckfTrackingEfficiency_Chi2",
		   "pur_pt_recNtrue",
		   "pur_pt_rec",
		   "purityPt",
		   "Purity in p_{T}",
		   "Purity");
  divideHistograms(file,
		   "ckfTrackingEfficiency_Chi2",
		   "eff_pt_trueNrec",
		   "eff_pt_true",
		   "efficiencyPt",
		   "Efficiency in p_{T}",
		   "Efficiency");

  divideHistograms(file,
		   "rsSeedEfficiency",
		   "eff_eta_trueNrec",
		   "eff_eta_rs_true",
		   "rsSeedingEfficiencyEta",
		   "Algorithmic Seeding Efficiency in #eta",
		   "Efficiency");

  divideHistograms(file,
		   "rsSeedEfficiency",
		   "eff_eta_trueNrec",
		   "eff_eta_true",
		   "seedingEfficiencyEta",
		   "Seeding Efficiency in #eta",
		   "Efficiency");

  divideHistograms(file,
		   "rsSeedEfficiency",
		   "eff_eta_rs_true",
		   "eff_eta_true",
		   "trueSeedingEfficiencyEta",
		   "RS Seeding Efficiency in #eta",
		   "Efficiency");



  // write file
  file->Write();
  delete file;

}

bool divideHistograms(TFile *file,
		      std::string directory_name,
		      std::string denominator_name,
		      std::string enumerator_name,
		      std::string output_name,
		      std::string output_title,
		      std::string output_y_title) {
  //
  // divide denominator by enumerator and store new histogram in 
  // same directory as inputs
  //

  TDirectory *directory = (TDirectory*)file->Get(directory_name.c_str());

  if ( directory != 0 ) {
    TH1 *denominator = (TH1*)directory->Get(denominator_name.c_str());
    if ( denominator != 0 ) {
      TH1 *enumerator = (TH1*)directory->Get(enumerator_name.c_str());
      if ( enumerator != 0 ) {
	directory->cd();
	TH1 * output = (TH1*)denominator->Clone();
	output->Divide(enumerator);
	output->SetName(output_name.c_str());
	output->SetTitle(output_title.c_str());
	output->SetYTitle(output_y_title.c_str());
	std::cout << "Divided histogram: " << denominator_name << " by histogram: " << enumerator_name << " in directory: " << directory_name << " and saved it as histogram: " << output_name << std::endl;
      } else {
	std::cout << "Enumerator histogram: " << enumerator << " cannot be found in directory: " << directory_name << std::endl;
      }
    } else {
      std::cout << "Denominator histogram: " << denominator << " cannot be found in directory: " << directory_name << std::endl;
    }
  } else {
    std::cout << "Directory: " << directory_name << " cannot be found in file: " << file->GetName() << std::endl;
  }

}
