{

  TH1D *histo = (TH1D*)gFile->Get("rsTracks/nTracks");

  for ( unsigned int index = 1;
	index <= 6;
	++index ) {
    cout << histo->GetBinContent(index) << " ";
  }

  TH1D *histo2 = (TH1D*)gFile->Get("rsSeeds/nSeed");

  cout << histo2->GetMean() << endl;

}
