{

  // get tree
  TTree *tree = (TTree*)file.Get("Events");

  // create and connect muon collection branch
  std::vector<reco::Muon> muonCollection;
  TBranch *muonBranch = tree->GetBranch("recoMuons_globalMuons__RECO.obj");
  muonBranch->SetAddress(&muonCollection);

  // instanciate cms1 muons object
  cms1::Muons muons;

  for ( unsigned int index = 0; index < tree->GetEntries(); ++index ) {
    std::cout << "event: " << index << std::endl;
    muonBranch->GetEntry(index);

    // print if 2 or more muons are found with pT > 2 GeV
    if ( muons.GlobalMuons(&muonCollection,2,2.) ) {
      std::cout << "Two or more muons found with pT > 2 GeV" << std::endl;
    }

  }
}
