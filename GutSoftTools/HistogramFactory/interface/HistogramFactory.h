#ifndef HistogramFactory_h
#define HistogramFactory_h

//
// Package:         GutSoftTools/HistrogramFactory
// Class:           HistogramFactory
// 
// Description:     standardized histogram creation
//
//                  histograms are identified by their TH1 name
//                  which has to include the type (1d,1i,2d,..) appended with "_"
//                  booking and fillling all uses this name
//                  and arguments to booking and filling has to adapted to the type by the user
//                  sub-directories are automatically created, directory separation by "/"
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Thu Sep 28 22:41:24 UTC 2006
//
// $Author: gutsche $
// $Date: 2006/10/06 14:42:01 $
// $Revision: 1.1 $
//

#include <string>
#include <vector>
#include <map>

class TFile;
class TDirectory;
class TH1;

class HistogramFactory {
 public:

  enum HistogramType {
    OneD,
    OneDLabel,
    TwoD,
    inValid
  };


  HistogramFactory(std::string filename);
  ~HistogramFactory();

  bool bookHistogram(std::string name,
		     std::string title,
		     std::string directory,
		     const int& nbin,
		     const double& low,
		     const double& high,
		     std::string xtitle,
		     std::string ytitle);

  
  bool bookHistogram(std::string name, 
		     std::string title,
		     std::string directory,
		     const int& nbin, 
		     double* array,
		     std::string xtitle, 
		     std::string ytitle);

  bool bookHistogram(std::string name, 
		     std::string title,
		     std::string directory,
		     std::string labels,
		     std::string xtitle, 
		     std::string ytitle);

  bool bookHistogram(std::string name, 
		     std::string title,
		     std::string directory,
		     const int& nbinx, 
		     const double& lowx, 
		     const double& highx,
		     const int& nbiny, 
		     const double& lowy, 
		     const double& highy,
		     std::string xtitle, 
		     std::string ytitle, 
		     std::string ztitle);

  bool bookHistogram(std::string name, 
		     std::string title,
		     std::string directory,
		     const int& nbinx, 
		     double* xarray,
		     const int& nbiny, 
		     const double& lowy, 
		     const double& highy,
		     std::string xtitle, 
		     std::string ytitle, 
		     std::string ztitle);

  bool bookHistogram(std::string name, 
		     std::string title,
		     std::string directory,
		     const int& nbinx, 
		     const double& lowx, 
		     const double& highx,
		     const int& nbiny, 
		     double* yarray,
		     std::string xtitle, 
		     std::string ytitle, 
		     std::string ztitle);

  bool bookHistogram(std::string name, 
		     std::string title,
		     std::string directory,
		     const int& nbinx, 
		     double* xarray,
		     const int& nbiny, 
		     double* yarray,
		     std::string xtitle, 
		     std::string ytitle, 
		     std::string ztitle);

  bool fill(std::string name, 
	    std::string input, 
	    double weight = 1.);

  bool fill(std::string name, 
	    const double& input_x, 
	    const double& input_y = 0., 
	    const double& weight = 1.);

  HistogramType extractHistogramType(const std::string& name);
  bool isValidHistogramType(HistogramType type);

 private:

  TFile *file_;
  std::map<std::string,TH1*> histograms_;

};

#endif
