{

  TH1D *histo = (TH1D*)gFile->Get("rsTracks/nTracks");

  for ( unsigned int index = 1;
	index <= 6;
	++index ) {
    cout << histo->GetBinContent(index) << " ";
  }

  cout << endl;

}
