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

#include "GutSoftTools/GutSoftStringTools/interface/GutSoftStringTools.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

GutSoftStringTools::GutSoftStringTools() {
  //
  // constructor
  //

}

GutSoftStringTools::~GutSoftStringTools() {
  //
  // destructor
  //

}

std::vector<std::string> GutSoftStringTools::split(const std::string& str, 
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

bool GutSoftStringTools::ReplaceAll(std::string& source, 
			     const std::string find, 
			     const std::string replace ) { 
  //
  // replace all occurances of find in source with replace
  //

  // output value
  bool result = false;
  
  size_t j; 
  for (;(j = source.find( find )) != std::string::npos;) { 
    source.replace( j, find.length(), replace ); 
    result = true;
  } 
  
  return result;
} 
