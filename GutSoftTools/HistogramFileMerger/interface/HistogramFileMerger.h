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
// $Date: 2006/10/17 00:08:40 $
// $Revision: 1.2 $
//

#include <string>
#include <map>
#include <vector>

class TFile;
class TDirectory;
class TObject;
class TH1;
class TH2;

class HistogramFileMerger {
 public:

  typedef std::map<std::string, TObject*>         DirectoryContent;
  typedef std::map<std::string,DirectoryContent>  FileContent;

  HistogramFileMerger(std::string outputFileName,
		      std::vector<std::string> inputFileNames);
  ~HistogramFileMerger();

  static TDirectory* getDirectory(TFile *file,
				  std::string& directory);
  static TDirectory* getDirectory(TDirectory *base, 
				  std::string& directory);

  bool readFile(FileContent &outputFileContent, 
		TFile *file);
  bool writeFile(FileContent &outputFileContent, 
		 TFile *outputFile);
  bool queryDirectoryStructure(std::vector<std::string>& directoryStructure,
			       TDirectory *directory,
			       std::string topLevelPath);

  bool merge (TObject *existing,
	      TObject *toMerge);

  bool mergeHistograms(TH1 *existing,
		       TH1 *toMerge);
  bool merge1DHistograms(TH1 *existing,
			 TH1 *toMerge);
  bool check1DHistogramCompatibility(TH1 *histogramOne,
				     TH1 *histogramTwo);
  bool merge2DHistograms(TH2 *existing,
			 TH2 *toMerge);
  bool check2DHistogramCompatibility(TH2 *histogramOne,
				     TH2 *histogramTwo);

  bool deleteInputFiles();

 private:

  FileContent outputFileContent_;
  std::string outputFileName_;
  std::vector<std::string> inputFileNames_;

};

#endif
