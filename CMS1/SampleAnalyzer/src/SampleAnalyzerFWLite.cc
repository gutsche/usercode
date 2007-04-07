//
// Package:         CMS1/Base
// Class:           SampleAnalyzerFWLite
// 
// Description:     FWLite code, hardly anything should be done here
//
// Original Author: Dmytro Kovalskyi
//
// $Author: dmytro $
// $Date: 2007/03/16 07:00:21 $
// $Revision: 1.1 $
//
#include <vector>
#include "CMS1/SampleAnalyzer/interface/SampleAnalyzerFWLite.h"
#include "TFile.h"

cms1::SampleAnalyzerFWLite::SampleAnalyzerFWLite( )
{ 
   theDataFW = &theData;
}
void cms1::SampleAnalyzerFWLite::analyze( bool initialization )
{
   // process default stuff (load data in the EventData)
   BaseAnalyzerFWLite::analyze( initialization );
   
   if (initialization) return;
   
   // run user code
   processEvent();
}
void cms1::SampleAnalyzerFWLite::setup( const edm::ParameterSet& iConfig )
{
   configure(iConfig);
}
   
	
