{

  gSystem->Load("libFWCoreFWLite.so"); 
  AutoLibraryLoader::enable();
  TChain chain("Events");
  chain.Add("*.root");
  std::cout << std::endl << "Loaded chain of " << chain.GetListOfFiles()->GetEntries() << " with " << chain.GetEntries() << " events." << std::endl << std::endl;

}
