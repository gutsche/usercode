{
  //Need this to allow ROOT to be able to use a ThingsHiggsSelector
  gSystem->Load("libFWCoreFWLite");
  AutoLibraryLoader::enable();
  // Have to load library manually since Proof does not use the 
  // mechanism used by TFile to find class dictionaries and therefore
  // the AutoLibraryLoader can not help
  gSystem->Load("libGutscheHiggsSelector");
  
  TSelector *selector = new HiggsSelector();
  //This holds the list of files and 'TTree' to process
  TChain c("Events");
  c.Add("dcache:/pnfs/cms/WAX/resilient/gutsche/tutorial/July06CMSSWtutorial/tutorial-higgs.root");

  //This actually processes the data
  c.Process(selector);

}
