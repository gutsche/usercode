#ifndef HistogramFileMerger_h
#define HistogramFileMerger_h

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

#include <string>
class TFile;
class TDirectory;

class HistogramFileMerger {
 public:

  HistogramFileMerger();
  ~HistogramFileMerger();

  static TDirectory* getDirectory(TFile *file,
				  const std::string& directory);
  static TDirectory* getDirectory(TDirectory *base, 
				  const std::string& directory);

 private:

};

#endif
