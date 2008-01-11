TCanvas canvas("canvas","canvas",450,450);

unsigned int color(unsigned int index) {
	unsigned int numCol = 5;
	index += unsigned int(index/numCol);
	while (index > numCol-1) index -= numCol;
	index = numCol-1 - index;
	static bool first = true;
	static vector<unsigned int> colors;
	if (first) {
		colors.push_back(1);
		colors.push_back(2);
		colors.push_back(4);
		colors.push_back(6);
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

void compareDistributions() {
	gStyle->SetOptStat(0);
	gStyle->SetPalette(1,0);

	TFile *output = new TFile("distributions.root","RECREATE");

	vector<TString> samples;

	samples.push_back(TString("qcd_pt_15_20"));
	samples.push_back(TString("qcd_pt_20_30"));
	samples.push_back(TString("qcd_pt_120_170"));
	samples.push_back(TString("qcd_pt_300_380"));
	samples.push_back(TString("qcd_pt_3000_3500"));

	// makeComparisonDistributionPlot(output,
	// 	samples,
	// 	"tkIso_uncut",
	// 	true,
	// 	false);
	// makeComparisonDistributionPlot(output,
	// 	samples,
	// 	"tkIso",
	// 	true,
	// 	false,
	// 	0.,
	// 	11.);
	// makeComparisonDistributionPlot(output,
	// 	samples,
	// 	"EOverp_uncut",
	// 	true,
	// 	false,
	//	0.3,
	// 	1.5);
	// makeComparisonDistributionPlot(output,
	// 	samples,
	// 	"EOverp",
	// 	true,
	// 	false,
	// 	0.3,
	// 	1.5);
	 makeComparisonDistributionPlot(output,
	 	samples,
	 	"HOverE_uncut",
	 	true,
	 	false,
	 	-0.6,
	 	0.5);
	// makeComparisonDistributionPlot(output,
	// 	samples,
	// 	"HOverE",
	// 	true,
	// 	false,
	// 	-0.2,
	// 	0.3);
	// makeComparisonDistributionPlot(output,
	// 	samples,
	// 	"jetpt_corr",
	// 	true,
	// 	true);
	// makeComparisonDistributionPlot(output,
	// 	samples,
	// 	"jeteta_corr",
	// 	true,
	// 	false);
	// makeComparisonDistributionPlot(output,
	//	samples,
	//	"njets_corr",
	//	false,
	//	false);

	output->Write();

}

void makeComparisonDistributionPlot(TFile *output,
					   				vector<TString> samples,
					   				TString &histogram_name,
					   				bool logy,
					   				bool logx,
									double userRangeMin = -999999.99,
									double userRangeMax = -999999.99) {

	canvas.Clear();
	canvas.SetLogx(logx);
	canvas.SetLogy(logy);
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

			TH1F *histogram = (TH1F*)file->Get(histogram_name);
			histogram->SetMarkerStyle(marker(sample));
			histogram->SetMarkerSize(0.6);
			histogram->SetMarkerColor(color(sample));
			histogram->SetLineColor(color(sample));
			histogram->SetDirectory(output);
			double rangeMin = histogram->GetXaxis()->GetXmin();
			double rangeMax = histogram->GetXaxis()->GetXmax();
			if ( userRangeMin > -999999.99 ) {
				rangeMin = userRangeMin;
			}
			if ( userRangeMax > -999999.99 ) {
				rangeMax = userRangeMax;
			}
			histogram->GetXaxis()->SetRangeUser(rangeMin,rangeMax);

			legend->AddEntry(histogram,samples[sample]);
			
			if ( first ) {
				histogram->DrawNormalized("HIST");
				first = false;
			} else {
				histogram->DrawNormalized("HIST,SAME");		
			}
			canvas.SetLeftMargin(0.15);
			canvas.SetRightMargin(0.1);
			canvas.SetBottomMargin(0.17);
			canvas.SetTopMargin(0.1);
			canvas.Update();
			legend->Draw();

		}
	}
	TString picture_name = histogram_name;
	picture_name.Append(".gif");

	canvas.Print(picture_name);
}
