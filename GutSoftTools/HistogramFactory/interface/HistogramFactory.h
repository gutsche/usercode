#ifndef HistogramFactory_h
#define HistogramFactory_h

//
// Package:         GutSoftTools/HistrogramFactory
// Class:           HistogramFactory
// 
// Description:     standardized histogram creation
//
//                  histograms are booked and filled all uses their UNIQUE name
//                  and arguments to booking and filling has to adapted to the type by the user
//                  sub-directories are automatically created, directory separation by "/"
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Thu Sep 28 22:41:24 UTC 2006
//
// $Author: gutsche $
// $Date: 2006/10/11 03:26:04 $
// $Revision: 1.2 $
//

#include <string>
#include <vector>
#include <map>

class TFile;
class TDirectory;
class TH1;

class HistogramFactory {
 public:

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
	    const double& input_x);

  bool fill(std::string name, 
	    std::string input);

  bool fill(std::string name, 
	    const double& input_x, 
	    const double& input_y);

  bool fillWeighted(std::string name, 
		    const double& input_x, 
		    const double& weight);

  bool fillWeighted(std::string name, 
		    std::string input, 
		    double weight);

  bool fillWeighted(std::string name, 
		    const double& input_x, 
		    const double& input_y, 
		    const double& weight);

 private:

  TFile *file_;
  std::map<std::string,TH1*> histograms_;

};

#endif
