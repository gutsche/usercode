//
// Package:         GutSoftTools/HistrogramFactory
// Class:           HistogramFactory
// 
// Description:     standardized histogram creation
//
//                  histograms are identified by their TH1 name
//                  which has to include the type (1d,1dlabel,2d) appended with "_"
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

#include <iostream>

#include "GutSoftTools/HistogramFactory/interface/HistogramFactory.h"

#include "GutSoftTools/HistogramFileMerger/interface/HistogramFileMerger.h"

#include "GutSoftTools/StringTools/interface/StringTools.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "TFile.h"
#include "TDirectory.h"
#include "TH1.h"
#include "TH1D.h"
#include "TH2D.h"

HistogramFactory::HistogramFactory(std::string filename) {
  //
  // constructor, requires filename for ROOT output file
  //

  file_ = new TFile(filename.c_str(),"RECREATE");

}

HistogramFactory::~HistogramFactory() {
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

bool HistogramFactory::bookHistogram(std::string name,
				     std::string title,
				     std::string directory,
				     const int& nbin,
				     const double& low,
				     const double& high,
				     std::string xtitle,
				     std::string ytitle) {
  // 
  // book TH1D histogram
  // in given directory in file
  //

  // return value
  bool success = false;

  HistogramType type = extractHistogramType(name);

  if ( type == OneD ) {
    // create directory
    HistogramFileMerger::getDirectory(file_,directory)->cd();

    // create histogram
    TH1D *dummy = new TH1D(name.c_str(),title.c_str(),nbin,low,high);
    dummy->SetXTitle(xtitle.c_str());
    dummy->SetYTitle(ytitle.c_str());
    dummy->Sumw2();

    // add to map
    histograms_[name] = (TH1*)dummy;

    success = true;
    
  } else {
    edm::LogWarning("GutSoftHistogramFactory") << "Histogram " << name << " could not be booked because the HistogramType does not correspond to the chosen parameters. Please make sure that name appendix corresponds to the parameters of the booking function";

  }

  return success;
}
  
bool HistogramFactory::bookHistogram(std::string name, 
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
  bool success = false;

  HistogramType type = extractHistogramType(name);

  if ( type == OneD ) {
    // create directory
    HistogramFileMerger::getDirectory(file_,directory)->cd();

    // create histogram
    TH1D *dummy = new TH1D(name.c_str(),title.c_str(),nbin,array);
    dummy->SetXTitle(xtitle.c_str());
    dummy->SetYTitle(ytitle.c_str());
    dummy->Sumw2();

    // add to map
    histograms_[name] = (TH1*)dummy;

    success = true;
    
  } else {
    edm::LogWarning("GutSoftHistogramFactory") << "Histogram " << name << " could not be booked because the HistogramType does not correspond to the chosen parameters. Please make sure that name appendix corresponds to the parameters of the booking function";

  }

  return success;
}

bool HistogramFactory::bookHistogram(std::string name, 
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
  bool success = false;

  HistogramType type = extractHistogramType(name);

  if ( type == OneDLabel ) {
    // create directory
    HistogramFileMerger::getDirectory(file_,directory)->cd();

    // create histogram
    // fill string array for bins
    std::vector<std::string> labelArray = StringTools::split(labels,",");

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

    success = true;
    
  } else {
    edm::LogWarning("GutSoftHistogramFactory") << "Histogram " << name << " could not be booked because the HistogramType does not correspond to the chosen parameters. Please make sure that name appendix corresponds to the parameters of the booking function";

  }

  return success;
}

bool HistogramFactory::bookHistogram(std::string name, 
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
  bool success = false;

  HistogramType type = extractHistogramType(name);

  if ( type == TwoD ) {
    // create directory
    HistogramFileMerger::getDirectory(file_,directory)->cd();

    // create histogram
    TH2D *dummy = new TH2D(name.c_str(),title.c_str(),nbinx,lowx,highx,nbiny,lowy,highy);
    dummy->SetXTitle(xtitle.c_str());
    dummy->SetYTitle(ytitle.c_str());
    dummy->SetZTitle(ztitle.c_str());
    dummy->Sumw2();

    // add to map
    histograms_[name] = (TH1*)dummy;

    success = true;
    
  } else {
    edm::LogWarning("GutSoftHistogramFactory") << "Histogram " << name << " could not be booked because the HistogramType does not correspond to the chosen parameters. Please make sure that name appendix corresponds to the parameters of the booking function";

  }

  return success;
}

bool HistogramFactory::bookHistogram(std::string name, 
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
  bool success = false;

  HistogramType type = extractHistogramType(name);

  if ( type == TwoD ) {
    // create directory
    HistogramFileMerger::getDirectory(file_,directory)->cd();

    // create histogram
    TH2D *dummy = new TH2D(name.c_str(),title.c_str(),nbinx,xarray,nbiny,lowy,highy);
    dummy->SetXTitle(xtitle.c_str());
    dummy->SetYTitle(ytitle.c_str());
    dummy->SetZTitle(ztitle.c_str());
    dummy->Sumw2();

    // add to map
    histograms_[name] = (TH1*)dummy;

    success = true;
    
  } else {
    edm::LogWarning("GutSoftHistogramFactory") << "Histogram " << name << " could not be booked because the HistogramType does not correspond to the chosen parameters. Please make sure that name appendix corresponds to the parameters of the booking function";

  }

  return success;
}

bool HistogramFactory::bookHistogram(std::string name, 
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
  bool success = false;

  HistogramType type = extractHistogramType(name);

  if ( type == TwoD ) {
    // create directory
    HistogramFileMerger::getDirectory(file_,directory)->cd();

    // create histogram
    TH2D *dummy = new TH2D(name.c_str(),title.c_str(),nbinx,lowx,highx,nbiny,yarray);
    dummy->SetXTitle(xtitle.c_str());
    dummy->SetYTitle(ytitle.c_str());
    dummy->SetZTitle(ztitle.c_str());
    dummy->Sumw2();

    // add to map
    histograms_[name] = (TH1*)dummy;

    success = true;
    
  } else {
    edm::LogWarning("GutSoftHistogramFactory") << "Histogram " << name << " could not be booked because the HistogramType does not correspond to the chosen parameters. Please make sure that name appendix corresponds to the parameters of the booking function";

  }

  return success;
}

bool HistogramFactory::bookHistogram(std::string name, 
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
  bool success = false;

  HistogramType type = extractHistogramType(name);

  if ( type == TwoD ) {
    // create directory
    HistogramFileMerger::getDirectory(file_,directory)->cd();

    // create histogram
    TH2D *dummy = new TH2D(name.c_str(),title.c_str(),nbinx,xarray,nbiny,yarray);
    dummy->SetXTitle(xtitle.c_str());
    dummy->SetYTitle(ytitle.c_str());
    dummy->SetZTitle(ztitle.c_str());
    dummy->Sumw2();

    // add to map
    histograms_[name] = (TH1*)dummy;

    success = true;
    
  } else {
    edm::LogWarning("GutSoftHistogramFactory") << "Histogram " << name << " could not be booked because the HistogramType does not correspond to the chosen parameters. Please make sure that name appendix corresponds to the parameters of the booking function";

  }

  return success;
}

bool HistogramFactory::fill(std::string name, 
			    std::string input, 
			    double weight){
  //
  // fill histogram by looking it up by its name in the histograms_ map
  //

  // return value
  bool success = false;

  // determine type
  HistogramType type = extractHistogramType(name);

  if ( type == OneDLabel ) {

    TH1D *histo = (TH1D*)(histograms_[name]);

    if ( histo ) {
      histo->Fill(input.c_str(),weight);
      success = true;
    } else {
      edm::LogWarning("GutSoftHistogramFactory") << "Histogram " << name << " could not be found. Please book the histogram before filling";
    }
  } else {
    edm::LogWarning("GutSoftHistogramFactory") << "Histogram " << name << " could not be filled because the HistogramType does not correspond to the chosen parameters. Please make sure that name appendix corresponds to the parameters of the filling function";

  }

  return success;
  
}

bool HistogramFactory::fill(std::string name, 
			    const double& input_x, 
			    const double& input_y, 
			    const double& weight){
  //
  // fill histogram by looking it up by its name in the histograms_ map
  //

  // return value
  bool success = false;

  // determine type
  HistogramType type = extractHistogramType(name);

  if ( type == OneD ) {

    TH1D *histo = (TH1D*)(histograms_[name]);

    if ( histo ) {
      histo->Fill(input_x,weight);
      success = true;
    } else {
      edm::LogWarning("GutSoftHistogramFactory") << "Histogram " << name << " could not be found. Please book the histogram before filling";
    }
  } else if ( type == TwoD ) {

    TH2D *histo = (TH2D*)(histograms_[name]);

    if ( histo ) {
      histo->Fill(input_x,input_y,weight);
      success = true;
    } else {
      edm::LogWarning("GutSoftHistogramFactory") << "Histogram " << name << " could not be found. Please book the histogram before filling";
    }
  } else {
    edm::LogWarning("GutSoftHistogramFactory") << "Histogram " << name << " could not be filled because the HistogramType does not correspond to the chosen parameters. Please make sure that name appendix corresponds to the parameters of the filling function";

  }

  return success;
  
}

HistogramFactory::HistogramType HistogramFactory::extractHistogramType(const std::string& name) {
  //
  // extract HistogramType from name
  // type has to be appended to name separated by "_"
  //

  // return value
  HistogramType type = inValid;

  // parse name
  std::vector<std::string> array = StringTools::split(name,"_");
  std::string typeString = *(array.end()-1);
  
  if ( typeString == "1d") {
    type = OneD;
  } else if ( typeString == "1dlabel") {
    type = OneDLabel;
    } else if ( typeString == "2d") {
    type = TwoD;
  }

  return type;
    
}

bool HistogramFactory::isValidHistogramType(HistogramType type) {
  //
  // checks if HistogramType is not inValid
  //

  // return value
  bool status = false;

  if ( type != inValid ) {
    status = true;
  }

  return status;
}
