TCanvas canvas("canvas","canvas",900,450);

unsigned int color(unsigned int index) {
	index += unsigned int(index/4);
	while (index > 3) index -= 4;
	index = 3 - index;
	static bool first = true;
	static vector<unsigned int> colors;
	if (first) {
		colors.push_back(1);
		colors.push_back(2);
		colors.push_back(4);
		colors.push_back(8);
		first = false;
	}
	return colors[index];
}

unsigned int marker(unsigned int index) {
	while (index > 3) index-=4;
	static bool first = true;
	static vector<unsigned int> markers;
	if (first) {
		markers.push_back(20);
		markers.push_back(21);
		markers.push_back(22);
		markers.push_back(23);
		first = false;
	}
	return markers[index];
}

void compareFractions() {
	gStyle->SetOptStat(0);
	gStyle->SetPalette(1,0);

	TFile *output = new TFile("fractions.root","RECREATE");

	vector<TString> samples;

	samples.push_back(TString("qcd_pt_0_15"));
	samples.push_back(TString("qcd_pt_15_20"));
	samples.push_back(TString("qcd_pt_20_30"));
	samples.push_back(TString("qcd_pt_30_50"));
	samples.push_back(TString("qcd_pt_50_80"));
	samples.push_back(TString("qcd_pt_80_120"));
	samples.push_back(TString("qcd_pt_120_170"));

	makeComparisonFractionPlots(output,
		samples,
		"num_ell",
		"den_ele",
		"#eta",    
		"p_{T} [GeV]",    
		"Fraction",
		"first");

	makeComparisonFractionPlots(output,
		samples,
		"num_elt",
		"den_ele",
		"#eta",    
		"p_{T} [GeV]",    
		"Fraction",
		"first");

	makeComparisonFractionPlots(output,
		samples,
		"num_wo_leading_ell",
		"den_wo_leading_ele",
		"#eta",    
		"p_{T} [GeV]",    
		"Fraction",
		"first_wo_leading");

	makeComparisonFractionPlots(output,
		samples,
		"num_wo_leading_elt",
		"den_wo_leading_ele",
		"#eta",    
		"p_{T} [GeV]",    
		"Fraction",
		"first_wo_leading");

	makeComparisonFractionPlots(output,
		samples,
		"num_wo_second_leading_ell",
		"den_wo_second_leading_ele",
		"#eta",    
		"p_{T} [GeV]",    
		"Fraction",
		"first_wo_second_leading");

	makeComparisonFractionPlots(output,
		samples,
		"num_wo_second_leading_elt",
		"den_wo_second_leading_ele",
		"#eta",    
		"p_{T} [GeV]",    
		"Fraction",
		"first_wo_second_leading");

	samples.clear();
	samples.push_back(TString("qcd_pt_170_230"));
	samples.push_back(TString("qcd_pt_230_300"));
	samples.push_back(TString("qcd_pt_300_380"));
	samples.push_back(TString("qcd_pt_380_470"));
	samples.push_back(TString("qcd_pt_470_600"));
	samples.push_back(TString("qcd_pt_600_800"));
	samples.push_back(TString("qcd_pt_800_1000"));

	makeComparisonFractionPlots(output,
		samples,
		"num_ell",
		"den_ele",
		"#eta",    
		"p_{T} [GeV]",    
		"Fraction",
		"second");

	makeComparisonFractionPlots(output,
		samples,
		"num_elt",
		"den_ele",
		"#eta",    
		"p_{T} [GeV]",    
		"Fraction",
		"second");

	makeComparisonFractionPlots(output,
		samples,
		"num_wo_leading_ell",
		"den_wo_leading_ele",
		"#eta",    
		"p_{T} [GeV]",    
		"Fraction",
		"second_wo_leading");

	makeComparisonFractionPlots(output,
		samples,
		"num_wo_leading_elt",
		"den_wo_leading_ele",
		"#eta",    
		"p_{T} [GeV]",    
		"Fraction",
		"second_wo_leading");

	makeComparisonFractionPlots(output,
		samples,
		"num_wo_second_leading_ell",
		"den_wo_second_leading_ele",
		"#eta",    
		"p_{T} [GeV]",    
		"Fraction",
		"second_wo_second_leading");

	makeComparisonFractionPlots(output,
		samples,
		"num_wo_second_leading_elt",
		"den_wo_second_leading_ele",
		"#eta",    
		"p_{T} [GeV]",    
		"Fraction",
		"second_wo_second_leading");

	samples.clear();
	samples.push_back(TString("qcd_pt_1000_1400"));
	samples.push_back(TString("qcd_pt_1400_1800"));
	samples.push_back(TString("qcd_pt_1800_2200"));
	samples.push_back(TString("qcd_pt_2200_2600"));
	samples.push_back(TString("qcd_pt_2600_3000"));
	samples.push_back(TString("qcd_pt_3000_3500"));
	samples.push_back(TString("qcd_pt_3500_inf"));

	makeComparisonFractionPlots(output,
		samples,
		"num_ell",
		"den_ele",
		"#eta",    
		"p_{T} [GeV]",    
		"Fraction",
		"third");

	makeComparisonFractionPlots(output,
		samples,
		"num_elt",
		"den_ele",
		"#eta",    
		"p_{T} [GeV]",    
		"Fraction",
		"third");

	makeComparisonFractionPlots(output,
		samples,
		"num_wo_leading_ell",
		"den_wo_leading_ele",
		"#eta",    
		"p_{T} [GeV]",    
		"Fraction",
		"third_wo_leading");

	makeComparisonFractionPlots(output,
		samples,
		"num_wo_leading_elt",
		"den_wo_leading_ele",
		"#eta",    
		"p_{T} [GeV]",    
		"Fraction",
		"third_wo_leading");

	makeComparisonFractionPlots(output,
		samples,
		"num_wo_second_leading_ell",
		"den_wo_second_leading_ele",
		"#eta",    
		"p_{T} [GeV]",    
		"Fraction",
		"third_wo_second_leading");

	makeComparisonFractionPlots(output,
		samples,
		"num_wo_second_leading_elt",
		"den_wo_second_leading_ele",
		"#eta",    
		"p_{T} [GeV]",    
		"Fraction",
		"third_wo_second_leading");

	samples.clear();
	samples.push_back(TString("qcd_pt_15_20"));
	samples.push_back(TString("qcd_pt_20_30"));
	samples.push_back(TString("qcd_pt_120_170"));
	samples.push_back(TString("qcd_pt_300_380"));
	samples.push_back(TString("qcd_pt_3000_3500"));

	makeComparisonFractionPlots(output,
		samples,
		"num_ell",
		"den_ele",
		"#eta",    
		"p_{T} [GeV]",    
		"Fraction",
		"fourth");

	makeComparisonFractionPlots(output,
		samples,
		"num_elt",
		"den_ele",
		"#eta",    
		"p_{T} [GeV]",    
		"Fraction",
		"fourth");

	makeComparisonFractionPlots(output,
		samples,
		"num_wo_leading_ell",
		"den_wo_leading_ele",
		"#eta",    
		"p_{T} [GeV]",    
		"Fraction",
		"fourth_wo_leading");

	makeComparisonFractionPlots(output,
		samples,
		"num_wo_leading_elt",
		"den_wo_leading_ele",
		"#eta",    
		"p_{T} [GeV]",    
		"Fraction",
		"fourth_wo_leading");

	makeComparisonFractionPlots(output,
		samples,
		"num_wo_second_leading_ell",
		"den_wo_second_leading_ele",
		"#eta",    
		"p_{T} [GeV]",    
		"Fraction",
		"fourth_wo_second_leading");

	makeComparisonFractionPlots(output,
		samples,
		"num_wo_second_leading_elt",
		"den_wo_second_leading_ele",
		"#eta",    
		"p_{T} [GeV]",    
		"Fraction",
		"fourth_wo_second_leading");

	output->Write();

}

void makeComparisonFractionPlots(TFile *output,
					   vector<TString> samples,
					   TString &numerator,
 					   TString &denominator,
  					   TString &x_axis_title,    
					   TString &y_axis_title,    
 				       TString &z_axis_title,
			  		   TString picture_filename) {

	canvas.Clear();
	canvas.Divide(2,1);
	bool first = true;
	
	TLegend *legend = new TLegend(0.7,0.75,0.99,0.99);
	legend->SetBorderSize(1);
	legend->SetFillColor(0);

	for ( unsigned int sample = 0;
	sample < samples.size();
	++sample ) {
		TString filename(samples[sample]);
		filename.Append(".root");
		TFile *file = new TFile(filename);
		if ( file->IsZombie() ) {
			cout << "File: " << filename << " cannot be opened and is skipped!" << endl;
			return;
		} else {

			TH2F *num = (TH2F*)file->Get(numerator);
			TH2F *den = (TH2F*)file->Get(denominator);

			TH1D *fake_rate_project_x = num->ProjectionX();
			fake_rate_project_x->Sumw2();
			fake_rate_project_x->Divide(den->ProjectionX());
			TString name = "fraction_";
			name.Append(numerator);
			name.Append("_");
			name.Append(sample);
			name.Append("_eta");
			name.ReplaceAll("num_","");
			TString title = "Fraction ";
			title.Append(numerator);
			title.Append(" ");
			title.Append(sample);
			title.Append(" eta");
			title.ReplaceAll("num_","");
			fake_rate_project_x->SetName(name);
			fake_rate_project_x->SetTitle(title);
			fake_rate_project_x->SetXTitle(x_axis_title);
			fake_rate_project_x->SetYTitle(z_axis_title);
			fake_rate_project_x->SetMarkerStyle(marker(sample));
			fake_rate_project_x->SetMarkerColor(color(sample));
			fake_rate_project_x->SetDirectory(output);
			fake_rate_project_x->SetMinimum(0.0);
			fake_rate_project_x->SetMaximum(0.5);

			TH1D *fake_rate_project_y = num->ProjectionY();
			fake_rate_project_y->Sumw2();
			fake_rate_project_y->Divide(den->ProjectionY());
			name = "fraction_";
			name.Append(numerator);
			name.Append("_");
			name.Append(sample);
			name.Append("_pt");
			name.ReplaceAll("num_","");
			title = "Fraction ";
			title.Append(numerator);
			title.Append(" ");
			title.Append(sample);
			title.Append(" pt");
			title.ReplaceAll("num_","");
			fake_rate_project_y->SetName(name);
			fake_rate_project_y->SetTitle(title);
			fake_rate_project_y->SetXTitle(y_axis_title);
			fake_rate_project_y->SetYTitle(z_axis_title);
			fake_rate_project_y->SetMarkerStyle(marker(sample));
			fake_rate_project_y->SetMarkerColor(color(sample));
			fake_rate_project_y->SetDirectory(output);
			fake_rate_project_y->SetMinimum(0.0);
			fake_rate_project_y->SetMaximum(0.5);
			
			legend->AddEntry(fake_rate_project_y,samples[sample]);

			TPad *pad1 = canvas.cd(1);
			if ( first ) {
				fake_rate_project_x->Draw("PE");
			} else {
				fake_rate_project_x->Draw("PE,SAME");		
			}
			pad1->SetLeftMargin(0.15);
			pad1->SetRightMargin(0.1);
			pad1->SetBottomMargin(0.17);
			pad1->SetTopMargin(0.1);
			pad1->Update();

			TPad *pad2 = canvas.cd(2);
			if ( first ) {
				fake_rate_project_y->Draw("PE");
				first = false;
			} else {
				fake_rate_project_y->Draw("PE,SAME");
			}
			pad2->SetLeftMargin(0.15);
			pad2->SetRightMargin(0.1);
			pad2->SetBottomMargin(0.17);
			pad2->SetTopMargin(0.1);
			pad2->Update();
			legend->Draw();

		}
	}
	TString picture_name = "fake_rate_";
	picture_name.Append(picture_filename);
	picture_name.Append("_");
	picture_name.Append(numerator);
	picture_name.Append(".gif");
	picture_name.ReplaceAll("num_","");

	canvas.Print(picture_name);
}