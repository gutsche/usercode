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
// $Author: gutsche $
// $Date: 2006/10/25 02:06:35 $
// $Revision: 1.3 $
//

#include <iostream>

#include "GutSoftTools/GutSoftHistogramFactory/interface/GutSoftHistogramFactory.h"

#include "GutSoftTools/GutSoftHistogramFileMerger/interface/GutSoftHistogramFileMerger.h"

#include "GutSoftTools/GutSoftStringTools/interface/GutSoftStringTools.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "TFile.h"
#include "TDirectory.h"
#include "TH1.h"
#include "TH1D.h"
#include "TH2D.h"

GutSoftHistogramFactory::GutSoftHistogramFactory(std::string filename) {
  //
  // constructor, requires filename for ROOT output file
  //

  file_ = new TFile(filename.c_str(),"RECREATE");

}

GutSoftHistogramFactory::~GutSoftHistogramFactory() {
  //
  // destructor, takes care that file is written and closed
  //

  if ( file_ != 0 ) {
    file_->cd();
    file_->Write();
    delete file_;
    file_ = 0;
  }

}

bool GutSoftHistogramFactory::bookHistogram(std::string name,
				     std::string title,
				     std::string directory,
				     const int& nbin,
				     const double& low,
				     const double& high,
				     std::string xtitle,
				     std::string ytitle) {
  // 
  // book TH1D histogram
  // in given directory
  //

  // return value
  bool result = false;

  // create directory
  GutSoftHistogramFileMerger::getDirectory(file_,directory)->cd();

  // create histogram
  TH1D *dummy = new TH1D(name.c_str(),title.c_str(),nbin,low,high);
  dummy->SetXTitle(xtitle.c_str());
  dummy->SetYTitle(ytitle.c_str());
  dummy->Sumw2();

  // add to map
  histograms_[name] = (TH1*)dummy;

  result = true;
    
  return result;
}
  
bool GutSoftHistogramFactory::bookHistogram(std::string name, 
				     std::string title,
				     std::string directory,
				     const int& nbin, 
				     double* array,
				     std::string xtitle, 
				     std::string ytitle) {
  // 
  // book TH1D histogram
  // in given directory in file
  //

  // return value
  bool result = false;

  // create directory
  GutSoftHistogramFileMerger::getDirectory(file_,directory)->cd();

  // create histogram
  TH1D *dummy = new TH1D(name.c_str(),title.c_str(),nbin,array);
  dummy->SetXTitle(xtitle.c_str());
  dummy->SetYTitle(ytitle.c_str());
  dummy->Sumw2();

  // add to map
  histograms_[name] = (TH1*)dummy;
  
  result = true;
    
  return result;
}

bool GutSoftHistogramFactory::bookHistogram(std::string name, 
				     std::string title,
				     std::string directory,
				     std::string labels,
				     std::string xtitle, 
				     std::string ytitle) {
  // 
  // book TH1D histogram
  // in given directory in file
  //

  // return value
  bool result = false;

  // create directory
  GutSoftHistogramFileMerger::getDirectory(file_,directory)->cd();

  // create histogram
  // fill string array for bins
  std::vector<std::string> labelArray = GutSoftStringTools::split(labels,",");
  
  TH1D *dummy = new TH1D(name.c_str(),title.c_str(),labelArray.size(),0.,labelArray.size());
  dummy->SetXTitle(xtitle.c_str());
  dummy->SetYTitle(ytitle.c_str());

  // set bin labels
  for ( unsigned int i = 1; i <= labelArray.size(); ++i)
    dummy->GetXaxis()->SetBinLabel(i,labelArray[i-1].c_str());

  // draw vertical labels
  dummy->LabelsOption("v");
  
  // add to map
  histograms_[name] = (TH1*)dummy;
  
  result = true;
    
  return result;
}

bool GutSoftHistogramFactory::bookHistogram(std::string name, 
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
				     std::string ztitle) {
  // 
  // book TH2D histogram
  // in given directory in file
  //

  // return value
  bool result = false;

  // create directory
  GutSoftHistogramFileMerger::getDirectory(file_,directory)->cd();

  // create histogram
  TH2D *dummy = new TH2D(name.c_str(),title.c_str(),nbinx,lowx,highx,nbiny,lowy,highy);
  dummy->SetXTitle(xtitle.c_str());
  dummy->SetYTitle(ytitle.c_str());
  dummy->SetZTitle(ztitle.c_str());
  dummy->Sumw2();

  // add to map
  histograms_[name] = (TH1*)dummy;

  result = true;
    
  return result;
}

bool GutSoftHistogramFactory::bookHistogram(std::string name, 
				     std::string title,
				     std::string directory,
				     const int& nbinx, 
				     double* xarray,
				     const int& nbiny, 
				     const double& lowy, 
				     const double& highy,
				     std::string xtitle, 
				     std::string ytitle, 
				     std::string ztitle) {
  // 
  // book TH2D histogram
  // in given directory in file
  //

  // return value
  bool result = false;

  // create directory
  GutSoftHistogramFileMerger::getDirectory(file_,directory)->cd();

  // create histogram
  TH2D *dummy = new TH2D(name.c_str(),title.c_str(),nbinx,xarray,nbiny,lowy,highy);
  dummy->SetXTitle(xtitle.c_str());
  dummy->SetYTitle(ytitle.c_str());
  dummy->SetZTitle(ztitle.c_str());
  dummy->Sumw2();
  
  // add to map
  histograms_[name] = (TH1*)dummy;
  
  result = true;
    
  return result;
}

bool GutSoftHistogramFactory::bookHistogram(std::string name, 
				     std::string title,
				     std::string directory,
				     const int& nbinx, 
				     const double& lowx, 
				     const double& highx,
				     const int& nbiny, 
				     double* yarray,
				     std::string xtitle, 
				     std::string ytitle, 
				     std::string ztitle) {
  // 
  // book TH2D histogram
  // in given directory in file
  //

  // return value
  bool result = false;

  // create directory
  GutSoftHistogramFileMerger::getDirectory(file_,directory)->cd();

  // create histogram
  TH2D *dummy = new TH2D(name.c_str(),title.c_str(),nbinx,lowx,highx,nbiny,yarray);
  dummy->SetXTitle(xtitle.c_str());
  dummy->SetYTitle(ytitle.c_str());
  dummy->SetZTitle(ztitle.c_str());
  dummy->Sumw2();

  // add to map
  histograms_[name] = (TH1*)dummy;

  result = true;
    
  return result;
}

bool GutSoftHistogramFactory::bookHistogram(std::string name, 
				     std::string title,
				     std::string directory,
				     const int& nbinx, 
				     double* xarray,
				     const int& nbiny, 
				     double* yarray,
				     std::string xtitle, 
				     std::string ytitle, 
				     std::string ztitle) {
  // 
  // book TH2D histogram
  // in given directory in file
  //

  // return value
  bool result = false;

  // create directory
  GutSoftHistogramFileMerger::getDirectory(file_,directory)->cd();

  // create histogram
  TH2D *dummy = new TH2D(name.c_str(),title.c_str(),nbinx,xarray,nbiny,yarray);
  dummy->SetXTitle(xtitle.c_str());
  dummy->SetYTitle(ytitle.c_str());
  dummy->SetZTitle(ztitle.c_str());
  dummy->Sumw2();

  // add to map
  histograms_[name] = (TH1*)dummy;

  result = true;
    
  return result;
}

bool GutSoftHistogramFactory::fill(std::string name, 
			    const double& input_x){
  //
  // fill TH1D histogram
  //

  // return value
  bool result = false;
  
  TH1 *org_histo = histograms_[name];
  if ( org_histo ) {
    if (org_histo->IsA()->InheritsFrom("TH1D")) {
      TH1D *histo = (TH1D*)org_histo;
      histo->Fill(input_x);
      result = true;
    } else {
      edm::LogWarning("GutSoftHistogramFactory") << "Histogram " << name << " is not a TH1D histograms and therefore cannot be filled using this fillFunction.";
    }
  } else {
    edm::LogWarning("GutSoftHistogramFactory") << "Histogram " << name << " could not be found. Please book the histogram before filling";
  }

  return result;
  
}

bool GutSoftHistogramFactory::fill(std::string name, 
			    std::string input){
  //
  // fill labeled TH1D histogram
  //

  // return value
  bool result = false;
  
  TH1 *org_histo = histograms_[name];
  if ( org_histo ) {
    if (org_histo->IsA()->InheritsFrom("TH1D")) {
      TH1D *histo = (TH1D*)org_histo;
      histo->Fill(input.c_str(),1.);
      result = true;
    } else {
      edm::LogWarning("GutSoftHistogramFactory") << "Histogram " << name << " is not a Label-TH1D histogram and therefore cannot be filled using this fillFunction.";
    }
  } else {
    edm::LogWarning("GutSoftHistogramFactory") << "Histogram " << name << " could not be found. Please book the histogram before filling";
  }

  return result;
  
}

bool GutSoftHistogramFactory::fill(std::string name, 
			    const double& input_x, 
			    const double& input_y){
  //
  // fill TH2D histogram
  //

  // return value
  bool result = false;
  
  TH1 *org_histo = histograms_[name];
  if ( org_histo ) {
    if (org_histo->IsA()->InheritsFrom("TH2D")) {
      TH2D *histo = (TH2D*)org_histo;
      histo->Fill(input_x,input_y);
      result = true;
    } else {
      edm::LogWarning("GutSoftHistogramFactory") << "Histogram " << name << " is not a TH2D histogram and therefore cannot be filled using this fillFunction.";
    }
  } else {
    edm::LogWarning("GutSoftHistogramFactory") << "Histogram " << name << " could not be found. Please book the histogram before filling";
  }

  return result;
  

}

bool GutSoftHistogramFactory::fillWeighted(std::string name, 
				    const double& input_x, 
				    const double& weight){
  //
  // fill TH1D histogram  with weighted content
  //

  // return value
  bool result = false;
  
  TH1 *org_histo = histograms_[name];
  if ( org_histo ) {
    if (org_histo->IsA()->InheritsFrom("TH1D")) {
      TH1D *histo = (TH1D*)org_histo;
      histo->Fill(input_x,weight);
      result = true;
    } else {
      edm::LogWarning("GutSoftHistogramFactory") << "Histogram " << name << " is not a TH1D histograms and therefore cannot be filled using this fillFunction.";
    }
  } else {
    edm::LogWarning("GutSoftHistogramFactory") << "Histogram " << name << " could not be found. Please book the histogram before filling";
  }

  return result;
  
}

bool GutSoftHistogramFactory::fillWeighted(std::string name, 
				    std::string input, 
				    double weight){
  //
  // fill labeled TH1D histogram with weighted content
  //

  // return value
  bool result = false;
  
  TH1 *org_histo = histograms_[name];
  if ( org_histo ) {
    if (org_histo->IsA()->InheritsFrom("TH1D")) {
      TH1D *histo = (TH1D*)org_histo;
      histo->Fill(input.c_str(),weight);
      result = true;
    } else {
      edm::LogWarning("GutSoftHistogramFactory") << "Histogram " << name << " is not a Label-TH1D histogram and therefore cannot be filled using this fillFunction.";
    }
  } else {
    edm::LogWarning("GutSoftHistogramFactory") << "Histogram " << name << " could not be found. Please book the histogram before filling";
  }

  return result;
  
}

bool GutSoftHistogramFactory::fillWeighted(std::string name, 
				    const double& input_x, 
				    const double& input_y, 
				    const double& weight){
  //
  // fill TH2D histogram  with weighted content
  //

  // return value
  bool result = false;
  
  TH1 *org_histo = histograms_[name];
  if ( org_histo ) {
    if (org_histo->IsA()->InheritsFrom("TH2D")) {
      TH2D *histo = (TH2D*)org_histo;
      histo->Fill(input_x,input_y,weight);
      result = true;
    } else {
      edm::LogWarning("GutSoftHistogramFactory") << "Histogram " << name << " is not a TH2D histogram and therefore cannot be filled using this fillFunction.";
    }
  } else {
    edm::LogWarning("GutSoftHistogramFactory") << "Histogram " << name << " could not be found. Please book the histogram before filling";
  }

  return result;
  

}
