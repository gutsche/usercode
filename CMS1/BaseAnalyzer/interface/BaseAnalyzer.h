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
// $Author: dmytro $
// $Date: 2007/08/31 01:56:48 $
// $Revision: 1.9 $
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
#include "TH2F.h"

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
       
      // What time of ntuples to create (EDM or plain ROOT)
      virtual bool isPlainRootNtuple() { return true; }
       
      // fill ntuple and userdata
      void fillUserData( EventData& event );
      TFile* theRootFile;
      TTree* theTree;
      TH2F* legoPtr;
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
      std::string   referenceMuonType;
      std::string   referenceElectronType;
      bool          fillMuons;
      bool          fillElectrons;
      bool          fillTracks;
      bool          fillJets;
      bool          fillMET;
      bool          fillMCInfo;
      bool          storePlots;
       

    private:
      edm::InputTag genJetInputTag_;
      
    };
}

   

#endif
