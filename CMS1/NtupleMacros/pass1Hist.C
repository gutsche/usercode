//a folder must be created plots that all the plots will be put in
//also all six root file must be present and correctly named
{
	//File type for plot (e.g. eps, pdf, tiff, jpg)
	string fileType(".pdf");
	//prefolder for different samples
	string prefolder("coarse");
	//prefix for each sample (new compilation)
	string prefix("pass1_");
	//postfix for each sample, as well as the folder for the plots (different cuts)
	string postfix("_MET");
	
	//Color Information ID,R,G,B
	TColor *one = new TColor(1000,0,0.8,0);
	TColor *one = new TColor(1001,0.8,0.2,0.6);
	TColor *one = new TColor(1002,0,0,0.6);
	TColor *one = new TColor(1003,1,1,0.2);
	TColor *one = new TColor(1004,1,0,0);
	TColor *one = new TColor(1005,1,0.6,0.2);
	int colors[6]; for(int i=0;i<6;++i) colors[i] = 1000 + i;

	//Histograms that are binned for each number of jets (0,1,2,3,>=4) 
	vector<string> times5;  
	times5.push_back("HT_J");
	times5.push_back("MET_J");
	times5.push_back("Mll_J");
	times5.push_back("PTTight_J");
	times5.push_back("PTLoose_J");
	times5.push_back("PTLeading_J");
	times5.push_back("PTTrailing_J");

	//Histograms that are binned independent of the number of jets
	vector<string> times1; 
	times1.push_back("NJets");

	//Histograms that are binned and are properties of each first four jets (1,2,3,4)
	vector<string> times4; 
	times4.push_back("PTJet_");

	//tags for the different samples
	vector<string> tag; 
	vector<Double_t> scales;

	//List of samples, will be placed on 6-up page, left to right, top to bottom
	tag.push_back("WW"+postfix);    scales.push_back(0.116);
	tag.push_back("WZ"+postfix);    scales.push_back(0.0526);
	tag.push_back("ZZ"+postfix);    scales.push_back(0.0496);
	tag.push_back("TTbar"+postfix); scales.push_back(0.113);
	tag.push_back("Zee"+postfix);   scales.push_back(5.15);
	tag.push_back("Zmumu"+postfix); scales.push_back(4.36);
	
	ostringstream temp,title;
	float scale;
	
	TObjArray f(tag.size());
	for(int i=0;i<tag.size();++i)
	{
		temp << prefolder << "/" << prefix << tag[i] << ".root";
		cout << "Opening File: " << temp.str().c_str() << endl;
		f.Add(new TFile(temp.str().c_str()));
		temp.clear();
		temp.str("");
	}
	
	for(int i=0;i<f.GetEntries();++i)
		if(((TFile*)f[i])->IsZombie())
		{
			cout << "All files must be present, please check for the existence of " << ((TFile*)f[i])->GetPath() << endl;
			return 0;
		}

	TCanvas *c1 = new TCanvas(" "," ",500,650);
	gStyle->SetOptStat(111111);
	gStyle->SetStatW(0.3);
	gStyle->SetStatH(0.22);

	TH1F *hist = 0;

//***************************************************************************************************************
//***************************************************************************************************************
//***********************************   Start Times 5 Hisrograms  ***********************************************
//***************************************************************************************************************
//***************************************************************************************************************
	for(int i=0;i<times5.size();++i) //Loop over all x5 histograms
	{	
		//Array of histograms that represents the integration over all jets for each sample
		TObjArray summed(f.GetEntries());
		TLegend legend(0.75,0.65,0.98,0.98,"Key");
		
		for(int j=0;j<5;++j)         //Loop over each jet (0,1,2,3,>=4)
		{
			//Canvas Stuff
			c1->Clear();
			title <<  times5[i] << j;
			c1->SetCanvasSize(500,650);
			c1->Divide(2,3);

			//New THStack for each set of plots so we can see them all next to each other, plus stacked
			THStack hs("hs",("Stacked "+title.str()).c_str());
			
			//Loop over each sample
			for(int k=0;k<f.GetEntries();k++)
			{
				//Grab specific sample out of appropriate file, assign to pointer for ease
				hist = (TH1F*)((TFile*)f[k])->Get(title.str().c_str());
				
				//Set initial bin values of summed histograms
				temp.clear(); temp.str("");
				temp << "samp" << k;
				if(j == 0)
				{
					summed.Add(new TH1F(temp.str().c_str(),
										temp.str().c_str(),
										hist->GetXaxis()->GetNbins(),
										hist->GetXaxis()->GetXmin(),
										hist->GetXaxis()->GetXmax()) );
					((TH1F*)summed[k])->SetXTitle( hist->GetXaxis()->GetTitle() );
					if(i==0) legend.AddEntry(hist,tag[k].c_str(),"F");
				}
			 	
				//Setting proper pad, and title of histogram
				c1->cd(k+1);
				temp.clear(); temp.str("");
				temp << tag[k] << " " << title.str();
				hist->SetTitle(temp.str().c_str());
				
				//Uncomment to show all normalized
				//if((scale=hist->Integral()) == 0 ) scale = 1;
				//hist->Scale(1.0/scale);

				//Scale to 100 pb^-1
				hist->Scale(scales[k]);

				hist->SetFillColor(colors[k]);

				//Remove Y-Label, it overlaps the numbers?!?
				hist->SetYTitle("Events / 100 pb^{-1}");
				
				hist->Draw("hist");
				hs.Add(hist);
				
				//Add hist to summed histogram at proper index
				((TH1F*)summed[k])->Add(hist);
			}

			//Write to file
			temp.clear(); temp.str("");
			temp << prefolder << "/plots" << postfix << "/" << title.str() << fileType;
			c1->Print(temp.str().c_str());
			temp.clear(); temp.str("");
			
			//Stuff for summed plots
			c1->Clear();
			c1->SetCanvasSize(650,500);
			c1->Divide(1,1); c1->cd(1);
			temp << prefolder << "/plots" << postfix << "/" << title.str() << "_stacked" << fileType;
			hs.Draw("hist");
			legend->Draw();
			hs.GetHistogram()->GetXaxis()->SetTitle( hist->GetXaxis()->GetTitle() );
			hs.GetHistogram()->GetYaxis()->SetTitle( hist->GetYaxis()->GetTitle() );
			c1->Print(temp.str().c_str(),"landscape");
			temp.clear(); temp.str("");
			title.clear(); title.str("");
		}


		c1->Clear();
		title <<  times5[i] << j;
		c1->SetCanvasSize(500,650);
		c1->Divide(2,3);
	
		temp.clear(); temp.str("");
		temp << times5[i] << "ALL Stacked";
		THStack hsum("hsum",temp.str().c_str());
		for(int k=0;k<f.GetEntries();k++) 
		{
			c1->cd(k+1);
			temp.clear(); temp.str("");
			temp << tag[k] << " " << times5[i] << "ALL";
			((TH1F*)summed[k])->SetTitle(temp.str().c_str());
			((TH1F*)summed[k])->SetFillColor(colors[k]);
			((TH1F*)summed[k])->Draw("hist");
			hsum.Add( ((TH1F*)summed[k]) );
		}
	
		temp.clear(); temp.str("");
		temp << prefolder << "/plots" << postfix << "/" << times5[i] << "ALL" << fileType;
		c1->Print(temp.str().c_str());

		c1->Clear();
		c1->SetCanvasSize(650,500);
		c1->Divide(1,1); c1->cd(1);
		hsum->Draw("hist");
		legend->Draw();
		hsum.GetHistogram()->GetXaxis()->SetTitle( ((TH1F*)summed[0])->GetXaxis()->GetTitle() );
		hsum.GetHistogram()->GetYaxis()->SetTitle( ((TH1F*)summed[0])->GetYaxis()->GetTitle() );
		if((scale=hsum->GetHistogram()->Integral()) == 0 ) scale = 1;
		hsum->GetHistogram()->Scale(1.0/scale);

		temp.clear(); temp.str("");
		temp << prefolder << "/plots" << postfix << "/" << times5[i] << "ALL_stacked" << fileType;
		c1->Print(temp.str().c_str(),"landscape");

		temp.clear(); temp.str("");
		title.clear(); title.str("");
	}
//***************************************************************************************************************
//***************************************************************************************************************
//*****************************   End Times 5 Hisrograms; Start Times 4 *****************************************
//***************************************************************************************************************
//***************************************************************************************************************

	ostringstream fileName;

	for(int i=0;i<times4.size();++i) //Loop over all x4 histograms
	{
		TLegend legend(0.75,0.65,0.98,0.98,"Key");
		for(int j=0;j<4;++j)         //Loop over each jet (1,2,3,4)
		{
			//Canvas Stuff
			c1->Clear();
			fileName <<  times4[i] << j;
			title <<  times4[i] << j+1;
			c1->SetCanvasSize(500,650);
			c1->Divide(2,3);

			//New THStack for each set of plots so we can see them all next to each other, plus stacked
			THStack hs("hs",("Stacked "+title.str()).c_str());
			
			//Loop over each sample
			for(int k=0;k<f.GetEntries();k++)
			{
				//Grab specific sample out of appropriate file, assign to pointer for ease
				hist = (TH1F*)((TFile*)f[k])->Get(fileName.str().c_str());
				
				//Setting proper pad, and title of histogram
				c1->cd(k+1);
				temp.clear(); temp.str("");
				temp << tag[k] << " " << title.str();
				hist->SetTitle(temp.str().c_str());
				
				//Uncomment to show all normalized
				//if((scale=hist->Integral()) == 0 ) scale = 1;
				//hist->Scale(1.0/scale);

				//Scale to 100 pb^-1
				hist->Scale(scales[k]);

				hist->SetFillColor(colors[k]);

				//Remove Y-Label, it overlaps the numbers?!?
				hist->SetYTitle("Events / 100 pb^{-1}");
				
				hist->Draw("hist");
				if(i==0) legend.AddEntry(hist,tag[k].c_str(),"F");
				hs.Add(hist);
			}

			//Write to file
			temp.clear(); temp.str("");
			temp << prefolder << "/plots" << postfix << "/" << title.str() << fileType;
			c1->Print(temp.str().c_str());
			temp.clear(); temp.str("");

			//Stuff for summed plots
			c1->Clear();
			c1->SetCanvasSize(650,500);
			c1->Divide(1,1); c1->cd(1);
			temp << prefolder << "/plots" << postfix << "/" << title.str() << "_stacked" << fileType;
			hs.Draw("hist");
			legend->Draw();
			hs.GetHistogram()->GetXaxis()->SetTitle( hist->GetXaxis()->GetTitle() );
			hs.GetHistogram()->GetYaxis()->SetTitle( hist->GetYaxis()->GetTitle() );
			c1->Print(temp.str().c_str(),"landscape");
			temp.clear(); temp.str("");
			title.clear(); title.str("");
			fileName.clear(); fileName.str("");
		}
	}


//***************************************************************************************************************
//***************************************************************************************************************
//*****************************   End Times 4 Hisrograms; Start Times 1 *****************************************
//***************************************************************************************************************
//***************************************************************************************************************
	for(int i=0;i<times1.size();++i) //Loop over all x1 histograms
	{
		//Canvas Stuff
		c1->Clear();
		title <<  times1[i];
		c1->SetCanvasSize(500,650);
		c1->Divide(2,3);
		TLegend legend(0.75,0.65,0.98,0.98,"Key");

		//New THStack for each set of plots so we can see them all next to each other, plus stacked
		THStack hs("hs",("Stacked "+title.str()).c_str());

		//Loop over each sample
		for(int k=0;k<f.GetEntries();k++)
		{
			//Grab specific sample out of appropriate file, assign to pointer for ease
			hist = (TH1F*)((TFile*)f[k])->Get(title.str().c_str());

			//Setting proper pad, and title of histogram
			c1->cd(k+1);
			temp.clear(); temp.str("");
			temp << tag[k] << " " << title.str();
			hist->SetTitle(temp.str().c_str());

			//Uncomment to show all normalized
			//if((scale=hist->Integral()) == 0 ) scale = 1;
			//hist->Scale(1.0/scale);

			//Scale to 100 pb^-1
			hist->Scale(scales[k]);

			hist->SetFillColor(colors[k]);

			//Remove Y-Label, it overlaps the numbers?!?
			hist->SetYTitle("Events / 100 pb^{-1}");

			hist->Draw("hist");
			if(i==0) legend.AddEntry(hist,tag[k].c_str(),"F");
			hs.Add(hist);
		}

		//Write to file
		temp.clear(); temp.str("");
		temp << prefolder << "/plots" << postfix << "/" << title.str() << fileType;
		c1->Print(temp.str().c_str());
		temp.clear(); temp.str("");

		//Stuff for summed plots
		c1->Clear();
		c1->SetCanvasSize(650,500);
		c1->Divide(1,1); c1->cd(1);
		temp << prefolder << "/plots" << postfix << "/" << title.str() << "_stacked" << fileType;
		hs.Draw("hist");
		legend->Draw();
		hs.GetHistogram()->GetXaxis()->SetTitle( hist->GetXaxis()->GetTitle() );
		hs.GetHistogram()->GetYaxis()->SetTitle( hist->GetYaxis()->GetTitle() );
		c1->Print(temp.str().c_str(),"landscape");
		temp.clear(); temp.str("");
		title.clear(); title.str("");
		fileName.clear(); fileName.str("");
	}


}



