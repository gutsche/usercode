//
// Package:         GutSoftTools/HistrogramFileMerger
// Class:           GutSoftHistogramFileMerger.cpp
// 
// Description:     executeable to merge histogram files
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Thu Sep 28 22:41:24 UTC 2006
//
// $Author: gutsche $
// $Date: 2006/10/17 00:08:40 $
// $Revision: 1.2 $
//

#include <iostream>
#include <string>
#include <vector>

#include "GutSoftTools/GutSoftHistogramFileMerger/interface/GutSoftHistogramFileMerger.h"

int main(int argc, char* argv[]) {

  if ( argc < 3 ) {
    std::cout << std::endl 
	      << argv[0] 
	      << std::endl << std::endl
	      << "First argument: output file" << std::endl
	      << "Following arguments: input files" << std::endl << std::endl;
  } else {

    std::string outputFileName = argv[1];
    std::vector<std::string> inputFileNames;
    for ( int parameter = 2;
	  parameter < argc;
	  ++parameter) {
      inputFileNames.push_back(argv[parameter]);
    }

    GutSoftHistogramFileMerger merger(outputFileName,
			       inputFileNames);

  }

}
