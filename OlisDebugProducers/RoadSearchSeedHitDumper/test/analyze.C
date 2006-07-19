{
  // make canvas
  TCanvas *zrCanvas = new TCanvas("ZR","ZR",700,500);
  TCanvas *xyCanvas = new TCanvas("XY","XY",500,500);

  // book histograms
  TH2F *zr = new TH2F("zr","zr",100,-280,280.,100,0.,115);
  TH2F *xy = new TH2F("xy","xy",100,-115.,115.,100,-115.,115.);

  TTree *tree = (TTree*)file.Get("Events");

  std::vector<RecHitGlobalPosition> innerSeedRecHits;
  std::vector<RecHitGlobalPosition> outerSeedRecHits;
  std::vector<SeedHitsGlobalPositions> seedRecHits;

  TBranch *innerSeedRecHitBranch = tree->GetBranch("RecHitGlobalPositions_GlobalPositions_innerSeedHits_D.obj");
  innerSeedRecHitBranch->SetAddress(&innerSeedRecHits);
  TBranch *outerSeedRecHitBranch = tree->GetBranch("RecHitGlobalPositions_GlobalPositions_outerSeedHits_D.obj");
  outerSeedRecHitBranch->SetAddress(&outerSeedRecHits);
  TBranch *seedRecHitsBranch = tree->GetBranch("SeedHitsGlobalPositionss_GlobalPositions__D.obj");
  seedRecHitsBranch->SetAddress(&seedRecHits);
  //innerSeedRecHitBranch->GetEntry(0);
  //outerSeedRecHitBranch->GetEntry(0);
  //seedRecHitsBranch->GetEntry(0);

  for ( unsigned int index = 0; index < tree->GetEntries(); ++index ) {
    std::cout << "index: " << index << std::endl;

    zrCanvas->cd();
    zr->Draw();
    xyCanvas->cd();
    xy->Draw();

    innerSeedRecHitBranch->GetEntry(index);
    outerSeedRecHitBranch->GetEntry(index);
    seedRecHitsBranch->GetEntry(index);

    std::cout << "inner seed RecHits size: " << innerSeedRecHits.size() << std::endl;
    for ( unsigned int inner = 0; inner < innerSeedRecHits.size(); ++inner ) {
      GlobalPoint innerPoint = innerSeedRecHits[inner].GetGlobalPoint();
      std::cout << "x: " << innerPoint->x() 
		<< " y: " << innerPoint->y() 
		<< " z: " << innerPoint->z() 
		<< " r: " << innerPoint->perp() 
		<< std::endl;
   
      zrCanvas->cd();
      TMarker *zrMarker = new TMarker(innerPoint->z(),innerPoint->perp(),3);
      zrMarker->SetMarkerColor(2);
      zrMarker->Draw("same");

      xyCanvas->cd();
      TMarker *xyMarker = new TMarker(innerPoint->x(),innerPoint->y(),3);
      xyMarker->SetMarkerColor(2);
      xyMarker->Draw("same");
      
    }
    
    std::cout << "outer seed RecHits size: " << outerSeedRecHits.size() << std::endl;
    for ( unsigned int outer = 0; outer < outerSeedRecHits.size(); ++outer ) {
      GlobalPoint outerPoint = outerSeedRecHits[outer].GetGlobalPoint();
//       std::cout << "x: " << outerPoint->x() 
// 		<< " y: " << outerPoint->y() 
// 		<< " z: " << outerPoint->z() 
// 		<< " r: " << outerPoint->perp() 
// 		<< std::endl;
   
      zrCanvas->cd();
      TMarker *zrMarker = new TMarker(outerPoint->z(),outerPoint->perp(),3);
      zrMarker->SetMarkerColor(4);
      zrMarker->Draw("same");

      xyCanvas->cd();
      TMarker *xyMarker = new TMarker(outerPoint->x(),outerPoint->y(),3);
      xyMarker->SetMarkerColor(4);
      xyMarker->Draw("same");
      
    }

    std::cout << "seeds size: " << seedRecHits.size() << std::endl;
    for ( unsigned int seed = 0; seed < seedRecHits.size(); ++seed ) {
      GlobalPoint innerPoint = seedRecHits[seed].GetInnerSeedGlobalPoint();
      GlobalPoint outerPoint = seedRecHits[seed].GetOuterSeedGlobalPoint();
   
      zrCanvas->cd();
      TLine *zrLine = new TLine(innerPoint->z(),innerPoint->perp(),outerPoint->z(),outerPoint->perp());
      zrLine->Draw("same");

      xyCanvas->cd();
      TLine *xyLine = new TLine(innerPoint->x(),innerPoint->y(),outerPoint->x(),outerPoint->y());
      xyLine->Draw("same");
      
    }
    
    zrCanvas->Update();
    xyCanvas->Update();

    char dummy;
    std::cin >> dummy; 
    zrCanvas->cd();
    zr->Draw();
    xyCanvas->cd();
    xy->Draw();
  
  }

}
