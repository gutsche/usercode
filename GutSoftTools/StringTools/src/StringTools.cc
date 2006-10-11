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

#include "GutSoftTools/StringTools/interface/StringTools.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

StringTools::StringTools() {
  //
  // constructor
  //

}

StringTools::~StringTools() {
  //
  // destructor
  //

}

std::vector<std::string> StringTools::split(const std::string& str, 
					    const std::string& splitter) {
  std::vector<std::string> result;
  if (!str.empty())
    {
      for(std::string::size_type offset = 0;;)
	{
	  std::string::size_type found = str.find(splitter, offset);
	  if (found != std::string::npos)
	    {
	      result.push_back(str.substr(offset, found-offset));
	      offset = found + splitter.size();
	    }
	  else
	    {
	      result.push_back(str.substr(offset, str.size()-offset));
	      break;
	    }
	}
    }
  return result;
}
