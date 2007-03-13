{
  // includes
#include <sstream>
  
  // read in file
  ifstream in("log.1");
  
  vector<double> rss;
  vector<double> size;
  
  double dummy, temp_rss, temp_size;
  string dummy_string;
  
  while ( in >> dummy >> dummy_string 
          >> temp_rss >> temp_size ) {
    rss.push_back(temp_rss);
    size.push_back(temp_size);
  }
  
  const unsigned int nBins = rss.size();
  double timeArray[nBins];
  double rssArray[nBins];
  double sizeArray[nBins];
  
  for ( unsigned int i = 0;
        i < nBins;
        ++i ) {
    timeArray[i] = i*10;
    rssArray[i] = rss[i]/1024;
    sizeArray[i] = size[i]/1024;
  }
  
  TGraph *graph = new TGraph(nBins,timeArray,rssArray);
  
  graph->Draw("ACP");
  
  
  
}