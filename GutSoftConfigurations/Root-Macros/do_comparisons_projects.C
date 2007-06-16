void do_comparisons_projects() {

  gROOT->ProcessLine(".L compareHistogramFiles.C++");

  gSystem->mkdir("070222_070220");
  gSystem->cd("070222_070220");
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
  filename = "../../070222_02/histograms.root";
  filenames.push_back(filename);
  filename = "../../070220_02/histograms.root";
  filenames.push_back(filename);
  compareHistogramFiles(filenames);
  gSystem->cd("..");

  // single_mu_100
  gSystem->cd("single_mu_100");
  filenames.clear();
  filename = "../../070222_03/histograms.root";
  filenames.push_back(filename);
  filename = "../../070220_03/histograms.root";
  filenames.push_back(filename);
  compareHistogramFiles(filenames);
  gSystem->cd("..");

  // minbias
  gSystem->cd("minbias");
  filenames.clear();
  filename = "../../070222_04/histograms.root";
  filenames.push_back(filename);
  filename = "../../070220_04/histograms.root";
  filenames.push_back(filename);
  compareHistogramFiles(filenames);
  gSystem->cd("..");

  // hzz4e
  gSystem->cd("hzz4e");
  filenames.clear();
  filename = "../../070222_05/histograms.root";
  filenames.push_back(filename);
  filename = "../../070220_05/histograms.root";
  filenames.push_back(filename);
  compareHistogramFiles(filenames);
  gSystem->cd("..");

  // hzz4mu
  gSystem->cd("hzz4mu");
  filenames.clear();
  filename = "../../070222_06/histograms.root";
  filenames.push_back(filename);
  filename = "../../070220_06/histograms.root";
  filenames.push_back(filename);
  compareHistogramFiles(filenames);
  gSystem->cd("..");

  // ttbar
  gSystem->cd("ttbar");
  filenames.clear();
  filename = "../../070222_07/histograms.root";
  filenames.push_back(filename);
  filename = "../../070220_07/histograms.root";
  filenames.push_back(filename);
  compareHistogramFiles(filenames);
  gSystem->cd("..");

  // jets
  gSystem->cd("jets");
  filenames.clear();
  filename = "../../070222_08/histograms.root";
  filenames.push_back(filename);
  filename = "../../070220_08/histograms.root";
  filenames.push_back(filename);
  compareHistogramFiles(filenames);
  gSystem->cd("..");


}
