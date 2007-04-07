{
   if ( gSystem->Load("libFWCoreFWLite") == 0) std::cout<<"libFWCoreFWLite was successfully loaded" <<std::endl;
   if ( gSystem->Load("libCMS1Base.so") == 0) std::cout<<"libCMS1Base.so was successfully loaded" <<std::endl;
   if ( gSystem->Load("libCMS1BaseAnalyzer.so") == 0) std::cout<<"libCMS1BaseAnalyzer.so was successfully loaded" <<std::endl;
   if ( gSystem->Load("libCMS1Muons.so") == 0) std::cout<<"libCMS1Muons.so was successfully loaded" <<std::endl;
   if ( gSystem->Load("libCMS1SampleAnalyzer.so") == 0) std::cout<<"libCMS1SampleAnalyzer.so was successfully loaded" <<std::endl;
   AutoLibraryLoader::enable();
}
