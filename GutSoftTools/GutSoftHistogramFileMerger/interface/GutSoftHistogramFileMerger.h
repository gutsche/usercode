#ifndef GutSoftHistogramFileMerger_h
#define GutSoftHistogramFileMerger_h

//
// Package:         GutSoftTools/GutSoftHistogramFileMerger
// Class:           GutSoftHistogramFileMerger
// 
// Description:     tools to handle ROOT histogram files
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Wed Oct 11 02:40:58 UTC 2006
//
// $Author: gutsche $
// $Date: 2006/11/27 23:51:38 $
// $Revision: 1.1 $
//

#include <string>
#include <map>
#include <vector>

class TFile;
class TDirectory;
class TObject;
class TH1;
class TH2;

class GutSoftHistogramFileMerger {
 public:

  typedef std::map<std::string, TObject*>         DirectoryContent;
  typedef std::map<std::string,DirectoryContent>  FileContent;

  GutSoftHistogramFileMerger(std::string outputFileName,
		      std::vector<std::string> inputFileNames);
  ~GutSoftHistogramFileMerger();

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
  static bool check1DHistogramCompatibility(TH1 *histogramOne,
					    TH1 *histogramTwo);
  bool merge2DHistograms(TH2 *existing,
			 TH2 *toMerge);
  static bool check2DHistogramCompatibility(TH2 *histogramOne,
					    TH2 *histogramTwo);

  bool deleteInputFiles();

 private:

  FileContent outputFileContent_;
  std::string outputFileName_;
  std::vector<std::string> inputFileNames_;

};

#endif
