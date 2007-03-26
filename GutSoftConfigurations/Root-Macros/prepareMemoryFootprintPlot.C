{
  // includes
#include <sstream>
  
  // read in file
  ifstream in("test_6.memory");
  
  double dummy, temp_rss, temp_size;
  string dummy_string;
  
  const unsigned int nBins = 100000;
  double timeArray[nBins];
  double rssArray[nBins];
  double sizeArray[nBins];
  unsigned int counter = 0;

  while ( in >> dummy >> dummy_string 
          >> temp_rss >> temp_size ) {
    cout << "counter: " << counter
    << " rss: " << temp_rss/1024
    << " size: " << temp_size/1024
    << endl;
    timeArray[counter] = counter*10;
    rssArray[counter] = temp_rss/1024;
		sizeArray[counter] = temp_size/1024;
    ++counter;
  }
  
  
  TGraph *graph = new TGraph(counter,timeArray,rssArray);
  
  graph->Draw("ACP");
  
  
  
}