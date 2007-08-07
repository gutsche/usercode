#ifndef CMS1_UserDataBlocks_h
#define CMS1_UserDataBlocks_h
// Description:     blocks that define user data structure of various objects
//
// Original Author: Dmytro Kovalskyi
//
// $Author: dmytro $
// $Date: 2007/05/22 07:12:39 $
// $Revision: 1.2 $
//
#include "CMS1/Base/interface/UserData.h"
#include "CMS1/Base/interface/EventData.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include <string>
namespace cms1 {
   struct BaseUserBlock
     {
	template <class T> void addEntry(std::vector<UserData<T>* >& container, 
					 UserData<T>*& var,
					 const std::string& name,
					 const std::string& title )
	  {
	     container.push_back( new UserData<T>(name, title, isCandidate_) );
	     var = container.back();
	  }
	BaseUserBlock():isCandidate_(false){}
	void setCandidateFlag( bool flag ){ isCandidate_ = flag; }
	bool isCandidate() { return isCandidate_; }
      protected:
	bool isCandidate_;
     };
   
   template <class T> struct ScalarUserBlock: public BaseUserBlock
     {
	typedef std::pair<std::string,std::string> VarName;
	void registerBlock(EventData& event, const std::string& name, const std::string& title)
	  {
	     // get names from the streamer
	     std::vector<VarName> ints   = streamer.getIntNames();
	     std::vector<VarName> floats = streamer.getFloatNames();
	     std::vector<VarName> p4s    = streamer.getP4Names();
	     
	     for(std::vector<VarName>::const_iterator str = ints.begin(); str != ints.end(); ++str) {
		UserDataInt* var(0);
	        addEntry(event.intUserData, var, name + str->first, title + str->second );
		intUserData.push_back(var);
	     }
	     for(std::vector<VarName>::const_iterator str = floats.begin(); str != floats.end(); ++str) {
		UserDataFloat* var(0);
	        addEntry(event.floatUserData, var, name + str->first, title + str->second );
		floatUserData.push_back(var);
	     }
	     for(std::vector<VarName>::const_iterator str = p4s.begin(); str != p4s.end(); ++str) {
		UserDataP4* var(0);
	        addEntry(event.p4UserData, var, name + str->first, title + str->second );
		p4UserData.push_back(var);
	     }
	  }
	
	template <class Z> void fill( const Z data ) {
	   streamer.fill(data);
	   for(unsigned int i=0; i < intUserData.size(); ++i)
	     intUserData[i]->addData( streamer.getInt(i) );
	   for(unsigned int i=0; i < floatUserData.size(); ++i)
	     floatUserData[i]->addData( streamer.getFloat(i) );
	   for(unsigned int i=0; i < p4UserData.size(); ++i)
	     p4UserData[i]->addData( streamer.getP4(i) );
	}
	
      private:
	std::vector<UserDataInt*>    intUserData;
	std::vector<UserDataFloat*>  floatUserData;
	std::vector<UserDataP4*>     p4UserData;
	T                            streamer;
     };
   
   template <class T> struct VectorUserBlock: public BaseUserBlock
     {
	typedef std::pair<std::string,std::string> VarName;
	void registerBlock(EventData& event, const std::string& name, const std::string& title)
	  {
	     // get names from the streamer
	     std::vector<VarName> ints   = streamer.getIntNames();
	     std::vector<VarName> floats = streamer.getFloatNames();
	     std::vector<VarName> p4s    = streamer.getP4Names();
	     
	     for(std::vector<VarName>::const_iterator str = ints.begin(); str != ints.end(); ++str) {
		UserDataInt1D* var(0);
	        addEntry(event.intUserData1D, var, name + str->first, title + str->second );
		intUserData1D.push_back(var);
	     }
	     for(std::vector<VarName>::const_iterator str = floats.begin(); str != floats.end(); ++str) {
		UserDataFloat1D* var(0);
	        addEntry(event.floatUserData1D, var, name + str->first, title + str->second );
		floatUserData1D.push_back(var);
	     }
	     for(std::vector<VarName>::const_iterator str = p4s.begin(); str != p4s.end(); ++str) {
		UserDataP41D* var(0);
	        addEntry(event.p4UserData1D, var, name + str->first, title + str->second );
		p4UserData1D.push_back(var);
	     }
	  }
	
	// template <class Z> void fill( const std::vector<const Z>& data ) {
	template <class Z> void fill( const Z data ) {
	   // the code below can be optimized ( who would doubt this? :)
	   
	   // first vector corresponds to input vector dimension and the second
	   // to number of instances of the base type in Z. Since the 2D array
	   // has fixed dimensions, it can be read out in transposed order.
	   std::vector<std::vector<int> >            intData2D; 
	   std::vector<std::vector<float> >          floatData2D;
	   std::vector<std::vector<LorentzVector> >  p4Data2D;
	   
	   // for(typename std::vector<const Z>::const_iterator itr = data.begin(); itr != data.end(); ++itr) {
	   for(typename Z::const_iterator itr = data.begin(); itr != data.end(); ++itr) {
	      streamer.fill(*itr);
	      std::vector<int>            intData1D;
	      std::vector<float>          floatData1D;
	      std::vector<LorentzVector>  p4Data1D;
	      
	      for(unsigned int i=0; i < intUserData1D.size(); ++i)
		intData1D.push_back( streamer.getInt(i) );
	      for(unsigned int i=0; i < floatUserData1D.size(); ++i)
		floatData1D.push_back( streamer.getFloat(i) );
	      for(unsigned int i=0; i < p4UserData1D.size(); ++i)
		p4Data1D.push_back( streamer.getP4(i) );
	      
	      intData2D.push_back(intData1D);
	      floatData2D.push_back(floatData1D);
	      p4Data2D.push_back(p4Data1D);
	   }
	   
	   // now we can fill ntuples
	   for(unsigned int i=0; i < intUserData1D.size(); ++i)
	     {
		std::vector<int>            intData1D;
		for(unsigned int j=0; j < data.size(); ++j)
		  intData1D.push_back(intData2D[j][i]);
		intUserData1D[i]->addData( intData1D );
	     }
	   for(unsigned int i=0; i < floatUserData1D.size(); ++i)
	     {
		std::vector<float>          floatData1D;
		for(unsigned int j=0; j < data.size(); ++j)
		  floatData1D.push_back(floatData2D[j][i]);
		floatUserData1D[i]->addData( floatData1D );
	     }
	   for(unsigned int i=0; i < p4UserData1D.size(); ++i)
	     {
		std::vector<LorentzVector>  p4Data1D;
		for(unsigned int j=0; j < data.size(); ++j)
		  p4Data1D.push_back(p4Data2D[j][i]);
		p4UserData1D[i]->addData( p4Data1D );
	     }
	}
	
      private:
	std::vector<UserDataInt1D*>    intUserData1D;
	std::vector<UserDataFloat1D*>  floatUserData1D;
	std::vector<UserDataP41D*>     p4UserData1D;
	T                              streamer;
     };
   
}

#endif
