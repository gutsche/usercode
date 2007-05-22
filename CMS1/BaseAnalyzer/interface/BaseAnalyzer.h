#ifndef CMS1_BaseAnalyzer_h
#define CMS1_BaseAnalyzer_h
//
// Package:         CMS1/BaseAnalyzer
// Class:           BaseAnalyzer
// 
// Description:     abstract base class for TnS analysis
//
// Original Author: Dmytro Kovalskyi
//
// $Author: sani $
// $Date: 2007/05/14 16:10:17 $
// $Revision: 1.5 $
//
#include <vector>
#include "CMS1/Base/interface/EventData.h"
#include "CMS1/Base/interface/UserData.h"
#include "CMS1/Base/interface/UserDataBlocks.h"
#include "CMS1/Muons/interface/Muons.h"
#include "CMS1/Tracks/interface/Tracks.h"
#include "CMS1/Electrons/interface/Electrons.h"
#include "CMS1/Jets/interface/Jets.h"
#include "CMS1/MET/interface/MET.h"

#include "CMS1/MCInfo/interface/MCInfo.h"

#include "TFile.h"
#include "TTree.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
namespace cms1 {
   
  class BaseAnalyzer 
    {
    public:
      BaseAnalyzer(){}
      virtual ~BaseAnalyzer(){}
    protected:
      // User configuration code 
      // In this class basic stuff is setup (black boxes)
      virtual void configure(const edm::ParameterSet& );
      
      // User code to process event 
      // In this class basic stuff is done (black boxes)
      virtual void processEvent(const edm::Event& );

      // finish event processing
	    virtual void finishEvent();

      // User code to finilize data processing
      virtual void finishProcessing();
    
      // fill ntuple and userdata
      void fillUserData( EventData& event );
      TFile* theRootFile;
      TTree* theTree;
      bool branchesInitialized;
      bool makeNtuples;
      bool candidateBasedNtuples;
	  
      // event data container
      EventData theData;
      
      // black boxes
      Muons         theMuons;
      Electrons     theElectrons;
      Jets          theJets;
      MET           theMET;
      MCInfo        theMCInfo;
      Tracks        theTracks;

    private:
      edm::InputTag genJetInputTag_;
    };
}

   

#endif
