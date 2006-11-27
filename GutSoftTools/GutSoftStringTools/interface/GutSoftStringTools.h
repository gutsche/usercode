#ifndef GutSoftStringTools_h
#define GutSoftStringTools_h

//
// Package:         GutSoftTools/GutSoftStringTools
// Class:           GutSoftStringTools
// 
// Description:     string tools
//
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Wed Oct 11 02:40:58 UTC 2006
//
// $Author: gutsche $
// $Date: 2006/10/25 02:06:03 $
// $Revision: 1.2 $
//

#include <string>
#include <vector>

class GutSoftStringTools {
 public:

  GutSoftStringTools();
  ~GutSoftStringTools();

  static std::vector<std::string> split(const std::string& str, 
					const std::string& splitter);

  static bool ReplaceAll(std::string& source, 
			 const std::string find, 
			 const std::string replace );
 private:

};

#endif
