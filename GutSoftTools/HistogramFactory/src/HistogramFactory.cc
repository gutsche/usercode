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

#include "GutSoftTools/HistogramFactory/interface/HistogramFactory.h"

TH1D* HistogramFactory::TH1DFactory(std::string name, std::string title,
				    int nbin, double low, double high,
				    std::string xtitle, std::string ytitle) {
  //
  // TH1D factory
  //

  TH1D *dummy = new TH1D(name.c_str(),title.c_str(),nbin,low,high);
  dummy->SetXTitle(xtitle.c_str());
  dummy->SetYTitle(ytitle.c_str());
  dummy->Sumw2();

  return dummy;

}

TH1D* HistogramFactory::TH1DVarFactory(std::string name, std::string title,
                                    int nbin, double* array,
                                    std::string xtitle, std::string ytitle) {
  //
  // TH1D factory with variable bin sizes
  // 


  TH1D *dummy = new TH1D(name.c_str(),title.c_str(),nbin,array);
  dummy->SetXTitle(xtitle.c_str());
  dummy->SetYTitle(ytitle.c_str());
  dummy->Sumw2();

  return dummy;

}

TH1D* HistogramFactory::TH1DLabelFactory(std::string name, std::string title,
                                      std::string labels,
                                      std::string xtitle, std::string ytitle) {
  //
  // TH1D factory for labeled bins, derived from labels char string, separate labes by comma
  //

  

  // fill string array for bins
  std::vector<std::string> labelArray = split(labels,",");

  TH1D *dummy = new TH1D(name.c_str(),title.c_str(),labelArray.size(),0.,labelArray.size());
  dummy->SetXTitle(xtitle.c_str());
  dummy->SetYTitle(ytitle.c_str());

  // set bin labels
  for ( unsigned int i = 1; i <= labelArray.size(); ++i)
    dummy->GetXaxis()->SetBinLabel(i,labelArray[i-1].c_str());

  // draw vertical labels
  dummy->LabelsOption("v");

  return dummy;
}

TH2D* HistogramFactory::TH2DFactory(std::string name, std::string title,
                                 int nbinx, double lowx, double highx,
                                 int nbiny, double lowy, double highy,
                                 std::string xtitle, std::string ytitle, std::string ztitle) {
  //
  // TH2D factory
  //

  gROOT->cd();

  TH2D *dummy = new TH2D(name.c_str(),title.c_str(),nbinx,lowx,highx,nbiny,lowy,highy);
  dummy->SetXTitle(xtitle.c_str());
  dummy->SetYTitle(ytitle.c_str());
  dummy->SetZTitle(ztitle.c_str());
  dummy->Sumw2();

  return dummy;

}

TH2D* HistogramFactory::TH2DVarFactory(std::string name, std::string title,
				    int nbinx, double* xarray,
				    int nbiny, double lowy, double highy,
				    std::string xtitle, std::string ytitle, std::string ztitle) {
  //
  // TH2D factory with variable bin sizes in x
  //

  TH2D *dummy = new TH2D(name.c_str(),title.c_str(),nbinx,xarray,nbiny,lowy,highy);
  dummy->SetXTitle(xtitle.c_str());
  dummy->SetYTitle(ytitle.c_str());
  dummy->SetZTitle(ztitle.c_str());
  dummy->Sumw2();
  
  return dummy;

}

//_____________________________________________________________

TH2D* HistogramFactory::TH2DVarFactory(std::string name, std::string title,
				    int nbinx, double lowx, double highx,
				    int nbiny, double* yarray,
				    std::string xtitle, std::string ytitle, std::string ztitle) {
  //
  // TH2D factory with variable bin sizes in y
  //

  TH2D *dummy = new TH2D(name.c_str(),title.c_str(),nbinx,lowx,highx,nbiny,yarray);
  dummy->SetXTitle(xtitle.c_str());
  dummy->SetYTitle(ytitle.c_str());
  dummy->SetZTitle(ztitle.c_str());
  dummy->Sumw2();

  return dummy;

}

TH2D* HistogramFactory::TH2DVarFactory(std::string name, std::string title,
				    int nbinx, double* xarray,
				    int nbiny, double* yarray,
				    std::string xtitle, std::string ytitle, std::string ztitle) {
  //
  // TH2D factory with variable bin sizes in x and y
  //

  TH2D *dummy = new TH2D(name.c_str(),title.c_str(),nbinx,xarray,nbiny,yarray);
  dummy->SetXTitle(xtitle.c_str());
  dummy->SetYTitle(ytitle.c_str());
  dummy->SetZTitle(ztitle.c_str());
  dummy->Sumw2();

  return dummy;

}

std::vector<std::string> HistogramFactory::split(const std::string& str, const std::string& splitter) {
  std::vector<std::string> result;
  if (!str.empty())
  {
    for(std::string::size_type offset = 0;;)
    {
      std::string::size_type found = str.find(splitter, offset);
      if (found != std::string::npos)
      {
        result.push_back(str.substr(offset, found-offset));
        offset = found + splitter.size();
      }
      else
      {
        result.push_back(str.substr(offset, str.size()-offset));
        break;
      }
    }
  }
  return result;
}

TH1I* HistogramFactory::TH1IFactory(std::string name, std::string title,
                                 int nbin, double low, double high,
                                 std::string xtitle, std::string ytitle) {
  //
  // TH1I factory
  //

  TH1I *dummy = new TH1I(name.c_str(),title.c_str(),nbin,low,high);
  dummy->SetXTitle(xtitle.c_str());
  dummy->SetYTitle(ytitle.c_str());
  dummy->Sumw2();

  return dummy;

}

TH1I* HistogramFactory::TH1IVarFactory(std::string name, std::string title,
                                    int nbin, double* array,
                                    std::string xtitle, std::string ytitle) {
  //
  // TH1I factory with variable bin sizes
  // 


  TH1I *dummy = new TH1I(name.c_str(),title.c_str(),nbin,array);
  dummy->SetXTitle(xtitle.c_str());
  dummy->SetYTitle(ytitle.c_str());
  dummy->Sumw2();

  return dummy;

}

TH1I* HistogramFactory::TH1ILabelFactory(std::string name, std::string title,
                                      std::string labels,
                                      std::string xtitle, std::string ytitle) {
  //
  // TH1I factory for labeled bins, derived from labels char string, separate labes by comma
  //

  

  // fill string array for bins
  std::vector<std::string> labelArray = split(labels,",");

  TH1I *dummy = new TH1I(name.c_str(),title.c_str(),labelArray.size(),0.,labelArray.size());
  dummy->SetXTitle(xtitle.c_str());
  dummy->SetYTitle(ytitle.c_str());

  // set bin labels
  for ( unsigned int i = 1; i <= labelArray.size(); ++i)
    dummy->GetXaxis()->SetBinLabel(i,labelArray[i-1].c_str());

  // draw vertical labels
  dummy->LabelsOption("v");

  return dummy;
}

TH2I* HistogramFactory::TH2IFactory(std::string name, std::string title,
                                 int nbinx, double lowx, double highx,
                                 int nbiny, double lowy, double highy,
                                 std::string xtitle, std::string ytitle, std::string ztitle) {
  //
  // TH2I factory
  //

  gROOT->cd();

  TH2I *dummy = new TH2I(name.c_str(),title.c_str(),nbinx,lowx,highx,nbiny,lowy,highy);
  dummy->SetXTitle(xtitle.c_str());
  dummy->SetYTitle(ytitle.c_str());
  dummy->SetZTitle(ztitle.c_str());
  dummy->Sumw2();

  return dummy;

}

TH2I* HistogramFactory::TH2IVarFactory(std::string name, std::string title,
				    int nbinx, double* xarray,
				    int nbiny, double lowy, double highy,
				    std::string xtitle, std::string ytitle, std::string ztitle) {
  //
  // TH2I factory with variable bin sizes in x
  //

  TH2I *dummy = new TH2I(name.c_str(),title.c_str(),nbinx,xarray,nbiny,lowy,highy);
  dummy->SetXTitle(xtitle.c_str());
  dummy->SetYTitle(ytitle.c_str());
  dummy->SetZTitle(ztitle.c_str());
  dummy->Sumw2();
  
  return dummy;

}

//_____________________________________________________________

TH2I* HistogramFactory::TH2IVarFactory(std::string name, std::string title,
				    int nbinx, double lowx, double highx,
				    int nbiny, double* yarray,
				    std::string xtitle, std::string ytitle, std::string ztitle) {
  //
  // TH2I factory with variable bin sizes in y
  //

  TH2I *dummy = new TH2I(name.c_str(),title.c_str(),nbinx,lowx,highx,nbiny,yarray);
  dummy->SetXTitle(xtitle.c_str());
  dummy->SetYTitle(ytitle.c_str());
  dummy->SetZTitle(ztitle.c_str());
  dummy->Sumw2();

  return dummy;

}

TH2I* HistogramFactory::TH2IVarFactory(std::string name, std::string title,
				    int nbinx, double* xarray,
				    int nbiny, double* yarray,
				    std::string xtitle, std::string ytitle, std::string ztitle) {
  //
  // TH2I factory with variable bin sizes in x and y
  //

  TH2I *dummy = new TH2I(name.c_str(),title.c_str(),nbinx,xarray,nbiny,yarray);
  dummy->SetXTitle(xtitle.c_str());
  dummy->SetYTitle(ytitle.c_str());
  dummy->SetZTitle(ztitle.c_str());
  dummy->Sumw2();

  return dummy;

}
