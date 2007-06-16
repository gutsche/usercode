void do_comparisons(std::string date) {

  gROOT->ProcessLine(".L compareHistogramFiles.C++");

  gSystem->mkdir(date.c_str());
  gSystem->cd(date.c_str());
  gSystem->mkdir("single_mu_10");
  gSystem->mkdir("single_mu_100");
  gSystem->mkdir("minbias");
  gSystem->mkdir("hzz4e");
  gSystem->mkdir("hzz4mu");
  gSystem->mkdir("ttbar");
  gSystem->mkdir("jets");

  std::vector<std::string> filenames;
  std::string filename;

  // single_mu_10
  gSystem->cd("single_mu_10");
  filenames.clear();
  filename = "../../";
  filename += date;
  filename += "_02/histograms.root";
  filenames.push_back(filename);
  filename = "../../";
  filename += date;
  filename += "_22/histograms.root";
  filenames.push_back(filename);
  filename = "../../";
  filename += date;
  filename += "_32/histograms.root";
  filenames.push_back(filename);
  compareHistogramFiles(filenames);
  gSystem->cd("..");

  // single_mu_100
  gSystem->cd("single_mu_100");
  filenames.clear();
  filename = "../../";
  filename += date;
  filename += "_03/histograms.root";
  filenames.push_back(filename);
  filename = "../../";
  filename += date;
  filename += "_23/histograms.root";
  filenames.push_back(filename);
  filename = "../../";
  filename += date;
  filename += "_33/histograms.root";
  filenames.push_back(filename);
  compareHistogramFiles(filenames);
  gSystem->cd("..");

  // minbias
  gSystem->cd("minbias");
  filenames.clear();
  filename = "../../";
  filename += date;
  filename += "_04/histograms.root";
  filenames.push_back(filename);
  filename = "../../";
  filename += date;
  filename += "_24/histograms.root";
  filenames.push_back(filename);
  filename = "../../";
  filename += date;
  filename += "_34/histograms.root";
  filenames.push_back(filename);
  compareHistogramFiles(filenames);
  gSystem->cd("..");

  // hzz4e
  gSystem->cd("hzz4e");
  filenames.clear();
  filename = "../../";
  filename += date;
  filename += "_05/histograms.root";
  filenames.push_back(filename);
  filename = "../../";
  filename += date;
  filename += "_25/histograms.root";
  filenames.push_back(filename);
  filename = "../../";
  filename += date;
  filename += "_35/histograms.root";
  filenames.push_back(filename);
  compareHistogramFiles(filenames);
  gSystem->cd("..");

  // hzz4mu
  gSystem->cd("hzz4mu");
  filenames.clear();
  filename = "../../";
  filename += date;
  filename += "_06/histograms.root";
  filenames.push_back(filename);
  filename = "../../";
  filename += date;
  filename += "_26/histograms.root";
  filenames.push_back(filename);
  filename = "../../";
  filename += date;
  filename += "_36/histograms.root";
  filenames.push_back(filename);
  compareHistogramFiles(filenames);
  gSystem->cd("..");

  // ttbar
  gSystem->cd("ttbar");
  filenames.clear();
  filename = "../../";
  filename += date;
  filename += "_07/histograms.root";
  filenames.push_back(filename);
  filename = "../../";
  filename += date;
  filename += "_27/histograms.root";
  filenames.push_back(filename);
  filename = "../../";
  filename += date;
  filename += "_37/histograms.root";
  filenames.push_back(filename);
  compareHistogramFiles(filenames);
  gSystem->cd("..");

  // jets
  gSystem->cd("jets");
  filenames.clear();
  filename = "../../";
  filename += date;
  filename += "_08/histograms.root";
  filenames.push_back(filename);
  filename = "../../";
  filename += date;
  filename += "_28/histograms.root";
  filenames.push_back(filename);
  filename = "../../";
  filename += date;
  filename += "_38/histograms.root";
  filenames.push_back(filename);
  compareHistogramFiles(filenames);
  gSystem->cd("..");


}
