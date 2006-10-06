//
// Package:         GutSoftTools/HistrogramFileMerger
// Class:           HistogramFileMerger.cpp
// 
// Description:     executeable to merge histogram files
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Thu Sep 28 22:41:24 UTC 2006
//
// $Author: gutsche $
// $Date: 2006/09/05 21:58:43 $
// $Revision: 1.2 $
//

#include <iostream>

int main(int argc, char* argv[]) {

  std::cout << "Number of parameters:" << argc << std::endl;

  for (int index = 0; index < argc; ++index) {
    std::cout << "Parameter " << index << ": " << argv[index] << std::endl;
  }

}
