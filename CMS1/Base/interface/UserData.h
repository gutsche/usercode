#ifndef CMS1_BaseTypes_h
#define CMS1_BaseTypes_h
// Description:     user data class
//
// Original Author: Dmytro Kovalskyi
//
// $Author: dmytro $
// $Date: 2007/04/17 04:43:13 $
// $Revision: 1.5 $
//
#include "DataFormats/Math/interface/LorentzVector.h"
#include <string>
#include <memory>
#include <vector>

namespace cms1 {
   typedef math::XYZTLorentzVector LorentzVector;
   template <class T> class UserData
     {
      public:
	// If alias prefix is not set, no alias is created
	UserData(const std::string& name, const std::string& name_prefix, const std::string& alias_prefix="")
	  {
	     theName = name_prefix+name;
	     thePtrToData = &theData;
	     theAlias = alias_prefix;
	     if (theAlias.length()>0) theAlias += name;
	  }
	T* get() { return thePtrToData; }     // get access to data
	T** getAddress() { return &thePtrToData; }     // ROOT needs something like that
	const std::string& name()  { return theName; }
	const std::string& alias() { return theAlias; }
      private:
	T theData;
	T* thePtrToData;
	std::string theName;
	std::string theAlias;
     };
   
   // Define some useful user data types
   typedef UserData<int>                         UserDataInt;
   typedef UserData<std::vector<int> >           UserDataVInt;
   typedef UserData<float>                       UserDataFloat;
   typedef UserData<std::vector<float> >         UserDataVFloat;
   typedef UserData<LorentzVector>               UserDataP4;
   typedef UserData<std::vector<LorentzVector> > UserDataVP4;
}
#endif
