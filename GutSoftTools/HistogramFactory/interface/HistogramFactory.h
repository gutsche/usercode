#ifndef HistogramFactory_h
#define HistogramFactory_h

//
// Package:         GutSoftTools/HistrogramFactory
// Class:           HistogramFactory
// 
// Description:     standardized histogram creation
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Thu Sep 28 22:41:24 UTC 2006
//
// $Author: gutsche $
// $Date: 2006/09/05 21:58:43 $
// $Revision: 1.2 $
//

#include <string>
#include <vector>

#include "TH1D.h"
#include "TH2D.h"
#include "TH1I.h"
#include "TH2I.h"


class HistogramFactory {
 public:
  HistogramFactory() {}
  ~HistogramFactory() {}
  
  TH1D* TH1DFactory(std::string name, std::string title,
		    int nbin, double low, double high,
		    std::string xtitle, std::string ytitle);

  TH1D* TH1DVarFactory(std::string name, std::string title,
		       int nbin, double* array,
		       std::string xtitle, std::string ytitle);

  TH1D* TH1DLabelFactory(std::string name, std::string title,
			 std::string labels,
			 std::string xtitle, std::string ytitle);

  TH2D* TH2DFactory(std::string name, std::string title,
		    int nbinx, double lowx, double highx,
		    int nbiny, double lowy, double highy,
		    std::string xtitle, std::string ytitle, std::string ztitle);

  TH2D* TH2DVarFactory(std::string name, std::string title,
		       int nbinx, double* xarray,
		       int nbiny, double lowy, double highy,
		       std::string xtitle, std::string ytitle, std::string ztitle);

  TH2D* TH2DVarFactory(std::string name, std::string title,
		       int nbinx, double lowx, double highx,
		       int nbiny, double* yarray,
		       std::string xtitle, std::string ytitle, std::string ztitle);

  TH2D* TH2DVarFactory(std::string name, std::string title,
		       int nbinx, double* xarray,
		       int nbiny, double* yarray,
		       std::string xtitle, std::string ytitle, std::string ztitle);

  TH1I* TH1IFactory(std::string name, std::string title,
		    int nbin, double low, double high,
		    std::string xtitle, std::string ytitle);

  TH1I* TH1IVarFactory(std::string name, std::string title,
		       int nbin, double* array,
		       std::string xtitle, std::string ytitle);

  TH1I* TH1ILabelFactory(std::string name, std::string title,
			 std::string labels,
			 std::string xtitle, std::string ytitle);

  TH2I* TH2IFactory(std::string name, std::string title,
		    int nbinx, double lowx, double highx,
		    int nbiny, double lowy, double highy,
		    std::string xtitle, std::string ytitle, std::string ztitle);

  TH2I* TH2IVarFactory(std::string name, std::string title,
		       int nbinx, double* xarray,
		       int nbiny, double lowy, double highy,
		       std::string xtitle, std::string ytitle, std::string ztitle);

  TH2I* TH2IVarFactory(std::string name, std::string title,
		       int nbinx, double lowx, double highx,
		       int nbiny, double* yarray,
		       std::string xtitle, std::string ytitle, std::string ztitle);

  TH2I* TH2IVarFactory(std::string name, std::string title,
		       int nbinx, double* xarray,
		       int nbiny, double* yarray,
		       std::string xtitle, std::string ytitle, std::string ztitle);

  std::vector<std::string> split(const std::string& str, const std::string& splitter);

 private:

};

#endif
