#include <iostream>
#include <vector>
#include "TH1D.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TPaveStats.h"
#include "TROOT.h"

unsigned int highestBinWithNonZeroContent(TH1 *histo) {
  //
  // determine highest bin with non_zero entries
  //

  // return value
  unsigned int bins = histo->GetNbinsX();
  unsigned int result = bins;

  for ( unsigned int bin = bins; 
	bin >= 1;
	--bin ) {
    result = bin;
    if ( histo->GetBinContent(bin) > 0 ) {
      break;
    }
  }

  return result;

}

unsigned int lowestBinWithNonZeroContent(TH1 *histo) {
  //
  // determine lowest bin with non_zero entries
  //

  // return value
  unsigned int bins = histo->GetNbinsX();
  unsigned int result = 1;

  for ( unsigned int bin = 1; 
	bin <= bins;
	++bin ) {
    result = bin;
    if ( histo->GetBinContent(bin) > 0 ) {
      break;
    }
  }

  return result;
}


void compareHistograms(std::string name, std::vector<TH1D*> histos) {

  TCanvas canvas;

  canvas.SetRightMargin(0.21);

  double maximum = 0;
  double minimum = 9999.99;
  unsigned int counter = 0;
  for (std::vector<TH1D*>::iterator histo = histos.begin(),
	 histoEnd = histos.end();
       histo != histoEnd;
       ++histo ) {
    double tempMaximum = (*histo)->GetMaximum();
    double tempMinimum = (*histo)->GetMinimum();
    if ( tempMaximum > maximum ) {
      maximum = tempMaximum;
    }
    if ( tempMinimum < minimum ) {
      minimum = tempMinimum;
    }

    (*histo)->SetLineColor((counter+1)*2);
    (*histo)->SetLineWidth(2);
    std::string histoname = "File ";
    histoname += counter;
    histoname.append(" ");
    histoname += name;
    (*histo)->SetName(histoname.c_str());
    ++counter;
  }

  // rescale
  unsigned int maxBin = 9999;
  unsigned int minBin = 0;
  for (std::vector<TH1D*>::iterator histo = histos.begin(),
	 histoEnd = histos.end();
       histo != histoEnd;
       ++histo ) {
    unsigned int tmpMaxBin = highestBinWithNonZeroContent(*histo);
    unsigned int tmpMinBin = lowestBinWithNonZeroContent(*histo);
    if ( tmpMaxBin < maxBin ) {
      maxBin = tmpMaxBin;
    }
    if ( tmpMinBin > minBin ) {
      minBin = tmpMinBin;
    }
  }
  double max = (*(histos.begin()))->GetBinCenter(maxBin) + (*(histos.begin()))->GetBinWidth(maxBin)/2;
  double maxMax = ((*(histos.begin()))->GetBinCenter((*(histos.begin()))->GetNbinsX())+(*(histos.begin()))->GetBinWidth((*(histos.begin()))->GetNbinsX())/2);
  double min = (*(histos.begin()))->GetBinCenter(minBin) - (*(histos.begin()))->GetBinWidth(minBin)/2;
  double minMin = ((*(histos.begin()))->GetBinCenter(1)-(*(histos.begin()))->GetBinWidth(1)/2);
  double fraction = 0.5;
  if ( max <= fraction * maxMax ) {
    if ( min >= fraction * minMin ) {
      for (std::vector<TH1D*>::iterator histo = histos.begin(),
	     histoEnd = histos.end();
	   histo != histoEnd;
	   ++histo ) {
	(*histo)->GetXaxis()->SetRangeUser(min,max);
      }
    } else {
      for (std::vector<TH1D*>::iterator histo = histos.begin(),
	     histoEnd = histos.end();
	   histo != histoEnd;
	   ++histo ) {
	(*histo)->GetXaxis()->SetRangeUser(minMin,max);
      }
    }
  } else if ( min >= fraction * minMin ) {
    for (std::vector<TH1D*>::iterator histo = histos.begin(),
	   histoEnd = histos.end();
	 histo != histoEnd;
	   ++histo ) {
      (*histo)->GetXaxis()->SetRangeUser(min,maxMax);
    }
  }
  
  bool first = true;
  for (std::vector<TH1D*>::iterator histo = histos.begin(),
	 histoEnd = histos.end();
       histo != histoEnd;
       ++histo ) {
    (*histo)->SetMaximum(maximum*1.1);
    (*histo)->SetMinimum(minimum);
    if ( first ) {
      (*histo)->Draw("HIST");
      first = false;
    } else {
      (*histo)->Draw("HIST,SAMES");
    }
  }

  canvas.Update();

  counter = 0;
  for (std::vector<TH1D*>::iterator histo = histos.begin(),
	 histoEnd = histos.end();
       histo != histoEnd;
       ++histo ) {
    TPaveStats *stats = (TPaveStats*)((*histo)->GetListOfFunctions()->FindObject("stats"));
    stats->SetX1NDC(0.80);
    stats->SetY1NDC(0.80-counter*0.2);
    stats->SetX2NDC(0.995);
    stats->SetY2NDC(0.995-counter*0.2);
    stats->SetOptStat(111111);
    stats->SetLineColor((counter+1)*2);
    stats->SetTextColor((counter+1)*2);
    ++counter;
  }

  first = true;
  for (std::vector<TH1D*>::iterator histo = histos.begin(),
	 histoEnd = histos.end();
       histo != histoEnd;
       ++histo ) {
    if ( first ) {
      (*histo)->Draw("HIST");
      first = false;
    } else {
      (*histo)->Draw("HIST,SAMES");
    }
  }
  
  // clean name for picture filename
  TString filename(name.c_str());
  filename.ReplaceAll("/","_");
  filename.Append(".png");

  canvas.SaveAs(filename);
  
}

void compareHistogramFiles(std::vector<std::string> filenames) {

  gROOT->SetBatch(1);

  std::vector<TFile*> files;
  for ( std::vector<std::string>::iterator filename = filenames.begin(),
	  filenameEnd = filenames.end();
	filename != filenameEnd;
	++filename ) {
    std::cout << "File with name: " << *filename << " cannot be opened!" << std::endl;
    TFile *file = new TFile(filename->c_str());
    if ( file->IsZombie() ) {
      std::cout << "File with name: " << *filename << " cannot be opened!" << std::endl;
    } else {
      files.push_back(file);
    }
  }

  // define histograms to compare
  std::vector<std::string> histonames;
  histonames.push_back("rsSeedEfficiency/trueSeedingEfficiencyEta");

//   histonames.push_back("rsTracks/eta");
//   histonames.push_back("rsTracks/nTracks");
//   histonames.push_back("rsTracks/chi2");
//   histonames.push_back("rsTracks/nhit");
//   histonames.push_back("rsTracks/phi");
//   histonames.push_back("rsTracks/pt");
//   histonames.push_back("rsTrackingEfficiency_Hits/efficiencyEta");
//   histonames.push_back("rsTrackingEfficiency_Hits/purityEta");
//   histonames.push_back("rsSeedEfficiency/trueSeedingEfficiencyEta");
//   histonames.push_back("rsSeedEfficiency/seedingEfficiencyEta");
//   histonames.push_back("rsSeedEfficiency/rsSeedingEfficiencyEta");
  


  for ( std::vector<std::string>::iterator histoname = histonames.begin(),
	  histonameEnd = histonames.end();
	histoname != histonameEnd;
	++histoname ) {
    std::vector<TH1D*> histos;
    for ( std::vector<TFile*>::iterator file = files.begin(),
	    fileEnd = files.end();
	  file != fileEnd;
	  ++file ) {
      TH1D *histo = (TH1D*)(*file)->Get(histoname->c_str());
      if ( !histo ) {
	std::cout << "Histogram: " << *histoname << " cannot be read from file: " << (*file)->GetName() << std::endl;
	return;
      } else {
	histos.push_back(histo);
      }
    }

    // make comparison plot
    compareHistograms(*histoname,histos);
    
  }

  gROOT->SetBatch(0);

}

