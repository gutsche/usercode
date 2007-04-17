#ifndef CMS1_BaseAnalyzerFWLite_h
#define CMS1_BaseAnalyzerFWLite_h

//
// Package:         CMS1/BaseAnalyzer
// Class:           BaseAnalyzerFWLite

// 
// Description:     FWLite analyzer abstract class providing the most basic ROOT related support
//
// Original Author: Dmytro Kovalskyi
//
// $Author: dmytro $
// $Date: 2007/04/07 17:54:23 $
// $Revision: 1.1 $
//

#include "TChain.h"
#include "TString.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/TFWLiteSelector/interface/TFWLiteSelectorBasic.h"
namespace cms1 {

   class BaseAnalyzerFWLite: public TFWLiteSelectorBasic
     {
      public:
	// class constructors/destructors
	BaseAnalyzerFWLite(){}
	virtual ~BaseAnalyzerFWLite(){}
	
	// ROOT user entry point
	virtual void processData(TString configFile);
	
	// TFWLiteSelectorBasic stuff (useless)
	virtual void begin( TList*& ){}
	virtual void preProcessing( const TList*, TList& ){}
	virtual void postProcessing( TList& ){}
	virtual void terminate( TList& ){}

      protected:
	// the following methods should call corresponding 
	// BaseAnalyzer methods. In order to avoid multiple inheritance
	// these calls have to be implemented in derived classes.
	// 
	// external call to run user code (comes from event 
	// loop inside TChain).
	virtual void process(const edm::Event& event) = 0;
	// external call to configure user code
	virtual void setup(const edm::ParameterSet&) = 0;
	
	// chain of data files
	TChain* theChain;
	
	int maxEvents;
	int skipEvents;
     };
}

#endif
