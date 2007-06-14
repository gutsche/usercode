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
// $Author: latb $
// $Date: 2007/06/14 19:40:06 $
// $Revision: 1.1.2.1 $
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
