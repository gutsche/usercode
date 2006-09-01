{
  // create file and book histograms
  TFile histofile("validate-minbias.root","RECREATE");
  TH1F* rs_chi2               = new TH1F("rs_chi2","Track reduced chi^{2}",50,0.,50.);
  TH1F* rs_pt                 = new TH1F("rs_pt","Track p_{T} (GeV)",40,0.0,20.0);
  TH1F* rs_nhit               = new TH1F("rs_nhit","Number of Hits",31,-0.5,30.5);
  TH1F* rs_eta                = new TH1F("rs_eta","Track #eta",30,-3.,3.);
  TH1F* rs_phi                = new TH1F("rs_phi","Track #phi",36,-3.1416,3.1416);
  TH1F* rs_transCurv          = new TH1F("rs_transCurv","Transverse curvature",50,-0.5,0.5);
  TH1F* rs_phiZero            = new TH1F("rs_phiZero","phiZero",100,-5.,5.);
  TH1F* rs_theta              = new TH1F("rs_theta","Theta",40,0.,4.);
  TH1F* rs_dZero              = new TH1F("rs_dZero","dZero",100,-50.,50.);
  TH1F* rs_dZ                 = new TH1F("rs_dZ","dZ",100,-100.,100.);
  TH1I* rs_numSeeds           = new TH1I("rs_numSeeds","Number of Seeds per event",100000,0,100000);
  TH1I* rs_numRawClouds       = new TH1I("rs_numRawClouds","Number of raw clouds per event",100000,0,100000);
  TH1I* rs_numCleanClouds     = new TH1I("rs_numCleanClouds","Number of clean clouds per event",100000,0,100000);
  TH1I* rs_numTrackCandidates = new TH1I("rs_numTrackCandidates","Number of track candidates per event",100000,0,100000);
  TH1I* rs_numTracks          = new TH1I("rs_numTracks","Number of tracks per event",100000,0,100000);

  TH1F* ckf_chi2               = new TH1F("ckf_chi2","Track reduced chi^{2}",50,0.,50.);
  TH1F* ckf_pt                 = new TH1F("ckf_pt","Track p_{T} (GeV)",40,0.0,20.0);
  TH1F* ckf_nhit               = new TH1F("ckf_nhit","Number of Hits",31,-0.5,30.5);
  TH1F* ckf_eta                = new TH1F("ckf_eta","Track #eta",30,-3.,3.);
  TH1F* ckf_phi                = new TH1F("ckf_phi","Track #phi",36,-3.1416,3.1416);
  TH1F* ckf_transCurv          = new TH1F("ckf_transCurv","Transverse curvature",50,-0.5,0.5);
  TH1F* ckf_phiZero            = new TH1F("ckf_phiZero","phiZero",100,-5.,5.);
  TH1F* ckf_theta              = new TH1F("ckf_theta","Theta",40,0.,4.);
  TH1F* ckf_dZero              = new TH1F("ckf_dZero","dZero",100,-50.,50.);
  TH1F* ckf_dZ                 = new TH1F("ckf_dZ","dZ",100,-100.,100.);
  TH1I* ckf_numSeeds           = new TH1I("ckf_numSeeds","Number of Seeds per event",100000,0,100000);
  TH1I* ckf_numRawClouds       = new TH1I("ckf_numRawClouds","Number of raw clouds per event",100000,0,100000);
  TH1I* ckf_numCleanClouds     = new TH1I("ckf_numCleanClouds","Number of clean clouds per event",100000,0,100000);
  TH1I* ckf_numTrackCandidates = new TH1I("ckf_numTrackCandidates","Number of track candidates per event",100000,0,100000);
  TH1I* ckf_numTracks          = new TH1I("ckf_numTracks","Number of tracks per event",100000,0,100000);

  // Number of entries in chain
  int   nevent = chain.GetEntries();

  // Tell root we only want some branches
  chain.SetBranchStatus("*",0);
  chain.SetBranchStatus("TrajectorySeeds_roadSearchSeeds__P.obj*",1);
  chain.SetBranchStatus("RoadSearchClouds_rawRoadSearchClouds__P.obj*",1);
  chain.SetBranchStatus("RoadSearchClouds_cleanRoadSearchClouds__P.obj*",1);
  chain.SetBranchStatus("TrackCandidates_rsTrackCandidates__P.obj*",1);
  chain.SetBranchStatus("recoTracks_rsWithMaterialTracks__P.obj*",1);
  chain.SetBranchStatus("TrajectorySeeds_globalMixedSeeds__P.obj*",1);
  chain.SetBranchStatus("TrackCandidates_ckfTrackCandidates__P.obj*",1);
  chain.SetBranchStatus("recoTracks_ctfWithMaterialTracks__P.obj*",1);

  std::vector<TrajectorySeed>  rsSeedCollection;
  std::vector<RoadSearchCloud> rsRawCloudCollection;
  std::vector<RoadSearchCloud> rsCleanCloudCollection;
  std::vector<TrackCandidate>  rsTrackCandidateCollection;
  std::vector<reco::Track>     rsTrackCollection;

  std::vector<TrajectorySeed>  ckfSeedCollection;
  std::vector<TrackCandidate>  ckfTrackCandidateCollection;
  std::vector<reco::Track>     ckfTrackCollection;

  // Loop over events
  for ( int index = 0; index < nevent; ++index ) {

    // Load the TTree corresponding to event index and return the entry with respect to that tree.
    int current = chain.LoadTree(index);

    // Check if we are on the first entry in the tree
    if (current==0) {

      //Read the first entry in this new tree, needed to set branch address.
      chain.GetEvent(index); 

      //Set the branch address for this new tree
      chain.SetBranchAddress("TrajectorySeeds_roadSearchSeeds__P.obj",&rsSeedCollection);
      chain.SetBranchAddress("RoadSearchClouds_rawRoadSearchClouds__P.obj",&rsRawCloudCollection);
      chain.SetBranchAddress("RoadSearchClouds_cleanRoadSearchClouds__P.obj",&rsCleanCloudCollection);
      chain.SetBranchAddress("TrackCandidates_rsTrackCandidates__P.obj",&rsTrackCandidateCollection);
      chain.SetBranchAddress("recoTracks_rsWithMaterialTracks__P.obj",&rsTrackCollection);

      chain.SetBranchAddress("TrajectorySeeds_globalMixedSeeds__P.obj",&ckfSeedCollection);
      chain.SetBranchAddress("TrackCandidates_ckfTrackCandidates__P.obj",&ckfTrackCandidateCollection);
      chain.SetBranchAddress("recoTracks_ctfWithMaterialTracks__P.obj",&ckfTrackCollection);
      
    }
    
    // Read the event.
    chain.GetEvent(index);

    if ( index%100 == 99 ) {
      std::cout << "Event: " << index+1 << std::endl;
    }

    for ( unsigned int bindex = 0; bindex < rsTrackCollection.size(); ++bindex ) {
      reco::Track* track = (reco::Track*)rsTrackCollection[bindex];
      rs_chi2->Fill(track->normalizedChi2());
      rs_pt->Fill(track->pt());
      rs_nhit->Fill(track->found());
      rs_eta->Fill(track->momentum().eta());
      rs_phi->Fill(track->momentum().phi());
      rs_transCurv->Fill(track->transverseCurvature());
      rs_phiZero->Fill(track->phi0());
      rs_theta->Fill(track->theta());
      rs_dZero->Fill(track->d0());
      rs_dZ->Fill(track->dz());
    }

    rs_numSeeds->Fill(rsSeedCollection.size());
    rs_numRawClouds->Fill(rsRawCloudCollection.size());
    rs_numCleanClouds->Fill(rsCleanCloudCollection.size());
    rs_numTrackCandidates->Fill(rsTrackCandidateCollection.size());
    rs_numTracks->Fill(rsTrackCollection.size());

    for ( unsigned int bindex = 0; bindex < ckfTrackCollection.size(); ++bindex ) {
      reco::Track* track = (reco::Track*)ckfTrackCollection[bindex];
      ckf_chi2->Fill(track->normalizedChi2());
      ckf_pt->Fill(track->pt());
      ckf_nhit->Fill(track->found());
      ckf_eta->Fill(track->momentum().eta());
      ckf_phi->Fill(track->momentum().phi());
      ckf_transCurv->Fill(track->transverseCurvature());
      ckf_phiZero->Fill(track->phi0());
      ckf_theta->Fill(track->theta());
      ckf_dZero->Fill(track->d0());
      ckf_dZ->Fill(track->dz());
    }

    ckf_numSeeds->Fill(ckfSeedCollection.size());
    ckf_numTrackCandidates->Fill(ckfTrackCandidateCollection.size());
    ckf_numTracks->Fill(ckfTrackCollection.size());

  }
  
  // save histograms
  histofile.Write();
  histofile.Close();
}
