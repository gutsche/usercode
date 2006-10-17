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
// $Date: 2006/10/11 03:25:19 $
// $Revision: 1.1 $
//

#include <iostream>

#include "GutSoftTools/HistogramFileMerger/interface/HistogramFileMerger.h"

#include "GutSoftTools/StringTools/interface/StringTools.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "TFile.h"
#include "TDirectory.h"
#include "TH1.h"
#include "TH2.h"
#include "TObject.h"
#include "TIterator.h"
#include "TKey.h"

HistogramFileMerger::HistogramFileMerger(std::string outputFileName,
					 std::vector<std::string> inputFileNames) {
  //
  // constructor
  //

  // open input files
  std::vector<TFile*> inputFiles;
  for ( std::vector<std::string>::const_iterator inputFileName = inputFileNames.begin();
	inputFileName != inputFileNames.end();
	++inputFileName ) {
    TFile *file = new TFile(inputFileName->c_str());
    if ( !file->IsZombie() ) {
      inputFiles.push_back(file);
    }
  }

  // copy content of input files into FileContent structure
  for ( std::vector<TFile*>::iterator inputFile = inputFiles.begin();
	inputFile != inputFiles.end();
	++inputFile ) {
    
    std::cout << "Reading in file: " << (*inputFile)->GetName() << std::endl;
    readFile(outputFileContent_,*inputFile);
    
  }

  // write FileContent structure into output file
  TFile *outputFile = new TFile(outputFileName.c_str(),"RECREATE");
  std::cout << "Writing out file: " << outputFile->GetName() << std::endl;
  writeFile(outputFileContent_, outputFile);

  // write and close oututFile
  outputFile->Write();
  delete outputFile;

  // close input files
  for ( std::vector<TFile*>::iterator inputFile = inputFiles.begin();
	inputFile != inputFiles.end();
	++inputFile ) {
    delete *inputFile;
  }

}

HistogramFileMerger::~HistogramFileMerger() {
  //
  // destructor
  //

}

TDirectory* HistogramFileMerger::getDirectory(TFile *file,
					      std::string& directory) {
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
					   std::string& name) {
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
    // edm::LogWarning("GutSoftHistogramFileMerger") 
    std::cout << "Directory " << name 
	      << " could not be found or created in directory " 
	      << base->GetName() 
	      << " and the return directory was set to the input directory!" 
	      << std::endl;
  }

  return output;
}

bool HistogramFileMerger::readFile(FileContent &outputFileContent, 
				   TFile *file) {
  //
  // copy the content of file  into outputFileContent

  // output value
  bool result = false;

  std::vector<std::string> directoryStructure;
  queryDirectoryStructure(directoryStructure,
			  (TDirectory*)file,
			  file->GetPath());

  for (std::vector<std::string>::iterator directory_name = directoryStructure.begin();
       directory_name != directoryStructure.end();
       ++directory_name) {
    TDirectory *directory = (TDirectory*)file->Get(directory_name->c_str());
    if ( outputFileContent.find(*directory_name) == outputFileContent.end() ) {
      DirectoryContent content;
      outputFileContent.insert(std::make_pair(*directory_name,content));
    }
  
    TKey *key    = 0;
    TObject *obj = 0;
    TIter nextkey(directory->GetListOfKeys());
    while ( (key = (TKey*)nextkey()) ) {
      obj = key->ReadObj();
      DirectoryContent &content = outputFileContent[*directory_name];
      if ( content.find(obj->GetName()) == content.end() ) {
	// add if not already existing
	content.insert(std::make_pair(obj->GetName(),obj));
      } else {
	// merge if already existing
	merge(content[obj->GetName()],obj);
      }

      result = true;
    }
  }
  return result;
}

bool HistogramFileMerger::queryDirectoryStructure(std::vector<std::string>& directoryStructure,
						  TDirectory *directory,
						  std::string topLevelPath) {
  //
  // make entry for every folder in file

  // output value
  bool result = false;

  TKey *key    = 0;
  TObject *obj = 0;
  TIter nextkey(directory->GetListOfKeys());
  while ( (key = (TKey*)nextkey()) ) {
    obj = key->ReadObj();
    if (obj->IsA()->InheritsFrom("TDirectory")) {

      TDirectory *directory = (TDirectory*)obj;
      std::string directoryName = directory->GetPath();
      StringTools::ReplaceAll(directoryName,topLevelPath,"");

      directoryStructure.push_back(directoryName);
      queryDirectoryStructure(directoryStructure,
			      directory,
			      topLevelPath);
      result = true;
    }
  }

  return result;
}

bool HistogramFileMerger::writeFile(FileContent &outputFileContent, 
				    TFile *outputFile) {
  //
  // write out FileContent to outptuFile

  // return value
  bool result = false;

  // loop over FileContent structure
  for ( FileContent::iterator content = outputFileContent.begin();
	content != outputFileContent.end();
	++ content ) {
    std::string directory_name = content->first;
    TDirectory *directory = getDirectory(outputFile,directory_name);
    for ( DirectoryContent::iterator object = content->second.begin();
	  object != content->second.end();
	  ++object ) {
      
      // write TH1 inherited classes into new File
      if ((object->second)->IsA()->InheritsFrom("TH1")) {
	TH1 *histogram = (TH1*)(object->second);
	histogram->SetDirectory(directory);
	result = true;
      }
    }
  }

  return result;
}

bool HistogramFileMerger::merge(TObject *existing,
				TObject *toMerge) {
  //
  // tries to merge objects
  //

  // return value
  bool result = false;

  // check if both objects inherit from TH1
  if ( (existing->IsA()->InheritsFrom("TH1") ) &&
       (toMerge->IsA()->InheritsFrom("TH1") ) ) {
    // try to merge histograms
    result = mergeHistograms((TH1*)existing,
			     (TH1*)toMerge);
  } else {
    std::cout << "HistogramFileMerger can currently only handle Histograms!" << std::endl;
    std::cout << "Object: "
	      << toMerge->GetName()
	      << " will not be merged into object: "
	      << existing->GetName()
	      << std::endl;
  }

  return result;
}

bool HistogramFileMerger::mergeHistograms(TH1 *existing,
					  TH1 *toMerge) {
  //
  // try to merge histograms
  //

  // return value
  bool result = false;

  if ( (existing->IsA()->InheritsFrom("TH2") ) &&
       (toMerge->IsA()->InheritsFrom("TH2") ) ) {
    // try to merge 2D histograms
    result = merge2DHistograms((TH2*)existing,
			       (TH2*)toMerge);
  } else {
    // try to merge 1D histograms
    result = merge1DHistograms(existing,
			       toMerge);
  }

  return result;
}


bool HistogramFileMerger::merge1DHistograms(TH1 *existing,
					    TH1 *toMerge) {
  //
  // try to merge 1D histograms
  //

  // return value
  bool result = false;

  // check for compatibility
  if ( check1DHistogramCompatibility(existing,toMerge) ) {
    existing->Add(toMerge);
    result = true;
  }

  return result;
}

bool HistogramFileMerger::check1DHistogramCompatibility(TH1 *histogramOne,
							TH1 *histogramTwo) {
  // 
  // check compatibility of two 1D Histograms
  //

  // return value
  bool result = true;
  
  // check for same number of bins
  if ( histogramOne->GetNbinsX() != histogramTwo->GetNbinsX() ) {
    result = kFALSE;
  }

  // check compatibility of each bin
  for ( Int_t i = 1; i <= histogramOne->GetNbinsX(); ++i ) {
    // check bin width compatibility
    if ( histogramOne->GetBinWidth(i)-histogramTwo->GetBinWidth(i) > 0.00001 ) {
      result = false;
    }
    // check bin center compatibility
    if ( histogramOne->GetBinCenter(i)-histogramTwo->GetBinCenter(i) > 0.00001 ) {
      result = false;
    }
  }

  // check if axis titles are the same
  std::string titleOne = histogramOne->GetXaxis()->GetTitle();
  std::string titleTwo = histogramTwo->GetXaxis()->GetTitle();
  if ( titleOne != titleTwo)
    result = false;
  titleOne = histogramOne->GetYaxis()->GetTitle();
  titleTwo = histogramTwo->GetYaxis()->GetTitle();
  if ( titleOne != titleTwo)
    result = false;

  return result;
}
  
bool HistogramFileMerger::merge2DHistograms(TH2 *existing,
					    TH2 *toMerge) {
  //
  // try to merge 2D histograms
  //

  // return value
  bool result = false;

  // check for compatibility
  if ( check2DHistogramCompatibility(existing,toMerge) ) {
    existing->Add(toMerge);
    result = true;
  }

  return result;
}

bool HistogramFileMerger::check2DHistogramCompatibility(TH2 *histogramOne,
							TH2 *histogramTwo) {
  // 
  // check compatibility of two 2D Histograms
  //

  // return value
  bool result = true;
  
  // check for same number of bins
  if ( histogramOne->GetNbinsX() != histogramTwo->GetNbinsX() ) {
    result = kFALSE;
  }
  if ( histogramOne->GetNbinsY() != histogramTwo->GetNbinsY() ) {
    result = kFALSE;
  }

  // check compatibility of each bin
  for ( Int_t i = 1; i <= histogramOne->GetNbinsX(); ++i ) {
    // check bin width compatibility
    if ( histogramOne->GetXaxis()->GetBinWidth(i)-histogramTwo->GetXaxis()->GetBinWidth(i) > 0.00001 ) {
      result = false;
    }
    // check bin center compatibility
    if ( histogramOne->GetXaxis()->GetBinCenter(i)-histogramTwo->GetXaxis()->GetBinCenter(i) > 0.00001 ) {
      result = false;
    }
  }
  for ( Int_t i = 1; i <= histogramOne->GetNbinsY(); ++i ) {
    // check bin width compatibility
    if ( histogramOne->GetYaxis()->GetBinWidth(i)-histogramTwo->GetYaxis()->GetBinWidth(i) > 0.00001 ) {
      result = false;
    }
    // check bin center compatibility
    if ( histogramOne->GetYaxis()->GetBinCenter(i)-histogramTwo->GetYaxis()->GetBinCenter(i) > 0.00001 ) {
      result = false;
    }
  }

  // check if axis titles are the same
  std::string titleOne = histogramOne->GetXaxis()->GetTitle();
  std::string titleTwo = histogramTwo->GetXaxis()->GetTitle();
  if ( titleOne != titleTwo)
    result = false;
  titleOne = histogramOne->GetYaxis()->GetTitle();
  titleTwo = histogramTwo->GetYaxis()->GetTitle();
  if ( titleOne != titleTwo)
    result = false;

  return result;
}
  
