//
// Package:         GutSoftTools/HistogramFileMerger
// Class:           HistogramFileMerger
// 
// Description:     tools to handle ROOT histogram files
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Wed Oct 11 02:40:58 UTC 2006
//
// $Author: gutsche $
// $Date: 2006/10/06 14:42:01 $
// $Revision: 1.1 $
//

#include <iostream>

#include "GutSoftTools/HistogramFileMerger/interface/HistogramFileMerger.h"

#include "GutSoftTools/StringTools/interface/StringTools.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "TFile.h"
#include "TDirectory.h"

HistogramFileMerger::HistogramFileMerger() {
  //
  // constructor
  //

}

HistogramFileMerger::~HistogramFileMerger() {
  //
  // destructor
  //

}

TDirectory* HistogramFileMerger::getDirectory(TFile *file,
					      const std::string& directory) {
  //
  // return pointer to directory
  // if not existing, create
  // subdirectories are separated by "/"

  // parse directory string
  std::vector<std::string> directory_array = StringTools::split(directory,"/");

  // return value
  TDirectory *output = (TDirectory*)(file);

  if ( directory_array.size() != 0 ) {
    for ( std::vector<std::string>::iterator name = directory_array.begin();
	  name != directory_array.end();
	  ++name) {
      output = getDirectory(output,*name);
    }
  }

  return output;

}

TDirectory* HistogramFileMerger::getDirectory(TDirectory *base, 
					   const std::string& name) {
  //
  // return pointer to directory in base directory
  // if not existing, create
  // single directory, not subdirectories


  // try to lookup directory from base
  TDirectory *output = (TDirectory*)(base->FindObject(name.c_str()));

  // if not existing, create
  if ( !output ) {
    output = base->mkdir(name.c_str(),name.c_str());
  }

  // if problems during creation, set return to file_
  if ( !output ) {
    output = base;
    edm::LogWarning("GutSoftHistogramFileMerger") << "Directory " << name 
					       << " could not be found or created in directory " 
					       << base->GetName() 
					       << " and the return directory was set to the input directory!";
  }

  return output;
}
