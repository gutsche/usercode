#ifndef GutSoftHistogramFactory_h
#define GutSoftHistogramFactory_h

//
// Package:         GutSoftTools/HistrogramFactory
// Class:           GutSoftHistogramFactory
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
// $Author: latb $
// $Date: 2007/06/14 19:40:03 $
// $Revision: 1.1.2.1 $
//

#include <string>
#include <vector>
#include <map>

class TFile;
class TDirectory;
class TH1;

class GutSoftHistogramFactory {
 public:

  GutSoftHistogramFactory(std::string filename);
  ~GutSoftHistogramFactory();

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

  inline void setBaseDirectory(std::string baseDirectory) { baseDirectory_ = baseDirectory; }

  std::string concatenateName(std::string name);

  bool divideHistograms(std::string denominator_name,
			std::string enumerator_name,
			std::string output_name,
			std::string title,
			std::string xtitle,
			std::string ytitle);

 private:

  TFile *file_;
  std::map<std::string,TH1*> histograms_;
  std::string baseDirectory_;

};

#endif
