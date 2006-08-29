{
  // create file and book histograms
  TFile histofile("validate-minbias.root","RECREATE");
  TH1F* h_chi2 = new TH1F("chi2","Track reduced chi^{2}",50,0.,50.);
  TH1F* h_pt   = new TH1F("pt","Track p_{T} (GeV)",40,0.0,20.0);
  TH1F* h_nhit = new TH1F("nhit","Number of Hits",31,-0.5,30.5);
  TH1F* h_eta  = new TH1F("eta","Track #eta",30,-3.,3.);
  TH1F* h_phi  = new TH1F("phi","Track #phi",36,-3.1416,3.1416);
  TH1F* h_transCurv = new TH1F("transCurv","Transverse curvature",50,-0.5,0.5);
  TH1F* h_phiZero = new TH1F("phiZero","phiZero",100,-5.,5.);
  TH1F* h_theta = new TH1F("theta","Theta",40,0.,4.);
  TH1F* h_dZero = new TH1F("dZero","dZero",100,-50.,50.);
  TH1F* h_dZ = new TH1F("dZ","dZ",100,-100.,100.);

  TTree *tree = (TTree*)file.Get("Events");

  std::vector<reco::Track> trackCollection;

  TBranch *branch = tree->GetBranch("recoTracks_rsWithMaterialTracks__P.obj");
  branch->SetAddress(&trackCollection);

  for ( unsigned int index = 0; index < tree->GetEntries(); ++index ) {
    std::cout << "Event: " << index+1 << std::endl;
    branch->GetEntry(index);
    for ( unsigned int bindex = 0; bindex < trackCollection.size(); ++bindex ) {
      reco::Track* track = (reco::Track*)trackCollection[bindex];
      h_chi2->Fill(track->normalizedChi2());
      h_pt->Fill(track->pt());
      h_nhit->Fill(track->found());
      h_eta->Fill(track->momentum().eta());
      h_phi->Fill(track->momentum().phi());
      h_transCurv->Fill(track->transverseCurvature());
      h_phiZero->Fill(track->phi0());
      h_theta->Fill(track->theta());
      h_dZero->Fill(track->d0());
      h_dZ->Fill(track->dz());
    }
  }
  
  // save histograms
  histofile.Write();
  histofile.Close();
}
