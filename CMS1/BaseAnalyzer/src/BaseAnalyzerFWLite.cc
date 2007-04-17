//
// Package:         CMS1/BaseAnalyzer
// Class:           BaseAnalyzerFWLite

// 
// Description:     FWLite analyzer abstract class providing the most basic ROOT related support
//
// Original Author: Dmytro Kovalskyi
//
// $Author: dmytro $
// $Date: 2007/04/08 06:39:29 $
// $Revision: 1.2 $
//

#include "CMS1/BaseAnalyzer/interface/BaseAnalyzerFWLite.h"
#include "FWCore/ParameterSet/interface/MakeParameterSets.h"
#include "FWCore/Utilities/interface/Exception.h"
#include <fstream>
#include <iostream>

void cms1::BaseAnalyzerFWLite::processData(TString configFile)
{
   // initialize a TChain
   // 
   theChain = 0;
   theChain = new TChain("Events");
   
   // read the config file
   std::ifstream file(configFile.Data());
   if( not file) {
      std::cout<<"problems opening file "<<configFile<<std::endl;
      return;
   }
   std::string line;
   std::string configstring;
   while(std::getline(file,line)) {
      configstring += line;
      configstring += "\n";
   }

   try {
      // parse the config file
      boost::shared_ptr<edm::ParameterSet> main;
      boost::shared_ptr<std::vector<edm::ParameterSet> > services;
      edm::makeParameterSets(configstring, main, services);
   
      // get input information
      edm::ParameterSet mainInput( main->getParameter<edm::ParameterSet>("@main_input") );
      maxEvents = mainInput.getUntrackedParameter<int>("maxEvents");
      skipEvents = mainInput.getUntrackedParameter<uint32_t>("skipEvents");
   
      // chain input files
      std::vector<std::string> files( mainInput.getUntrackedParameter<std::vector<std::string> >("fileNames") );
      
      // chain files
      for(std::vector<std::string>::const_iterator itr = files.begin(); itr != files.end(); ++itr)
	theChain->AddFile(itr->c_str());

      // get module PSet
      std::vector<std::string> modules( main->getParameter<std::vector<std::string> >("@all_modules") );
      if (modules.empty()) return;
      if (modules.size() > 1) std::cout << "WARNING: config file contains more than one module. " << 
	"Read parameters of the first one - the rest is ignored" << std::endl;
   
      // setup job 
      edm::ParameterSet pSet( main->getParameter<edm::ParameterSet>(modules.front()) );
      setup(pSet);
   }
   catch ( cms::Exception& e) {
      std::cout <<"Caught exception\n"<<e<<std::endl;
      return;
   }
   
   if ( maxEvents==-1 ) maxEvents = 1000000000;
   if ( maxEvents < 0 ) maxEvents = 0;
   
   // process data
   theChain->Process(this,"",maxEvents,skipEvents);
   
   delete theChain;
   theChain = 0;
}

