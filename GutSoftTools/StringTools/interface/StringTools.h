#ifndef StringTools_h
#define StringTools_h

//
// Package:         GutSoftTools/StringTools
// Class:           StringTools
// 
// Description:     string tools
//
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Wed Oct 11 02:40:58 UTC 2006
//
// $Author: gutsche $
// $Date: 2006/10/06 14:42:01 $
// $Revision: 1.1 $
//

#include <string>
#include <vector>

class StringTools {
 public:

  StringTools();
  ~StringTools();

  static std::vector<std::string> split(const std::string& str, 
					const std::string& splitter);
 private:

};

#endif
