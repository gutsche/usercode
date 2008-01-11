//
// Package:         CMS1/FakeRateNtupleMaker
// Class:           FakeRateNtupleMaker
//
// $Author: dmytro $
// $Date: 2007/08/30 14:16:16 $
// $Revision: 1.1 $
//

#include <vector>
#include <algorithm> 
#include <cmath>
#include <utility>
#include <sstream>
#include <iomanip>

#include "CMS1/FakeRateNtupleMaker/interface/FakeRateNtupleMaker.h"
#include "CMS1/Base/interface/Dump.h"
#include "CMS1/EventHyp/interface/EventHyp.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/EgammaCandidates/interface/Electron.h"
#include "DataFormats/EgammaCandidates/interface/ElectronFwd.h"

#include "DataFormats/JetReco/interface/CaloJet.h"

#include "DataFormats/METReco/interface/CaloMETCollection.h"

#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "Utilities/Timing/interface/TimingReport.h"
#include "Utilities/Timing/interface/TimerStack.h" 

#include "CMS1/CommonTools/interface/UserDataTools.h"

void cms1::FakeRateNtupleMaker::finishProcessing()
{
  // let the base analyzer finish its work
  BaseAnalyzer::finishProcessing();
}

void
cms1::FakeRateNtupleMaker::processEvent( const edm::Event& iEvent )
{

  // add AllJets to EventData
  std::vector<const reco::Candidate*> jets = theJets.getJets( referenceJetType_ );
  theData.addBBCollection("AllJets",  jets );
  std::vector<const reco::Candidate*> correctedJets = theJets.getJets( correctedJetType_ );
  theData.addBBCollection("CorrectedJets", correctedJets  );

  jetsUserBlock_.fill( getStreamerArguments(&theData, jets) );
  nJets_->addData( jets.size() );
  correctedJetsUserBlock_.fill( getStreamerArguments(&theData, correctedJets) );
  nCorrectedJets_->addData( correctedJets.size() );

}

void cms1::FakeRateNtupleMaker::configure(const edm::ParameterSet& iConfig)
{

  // parameters
  correctedJetType_ =  iConfig.getUntrackedParameter<string>("correctedJetType");
  referenceJetType_ =  iConfig.getUntrackedParameter<string>("referenceJetType");

  // register jets block
  jetsUserBlock_.registerBlock( theData, "jets_", "reference jets,");
  theData.intUserData.push_back( new UserData<int>("evt_njets", "number of jets in the reference collection", false) );
  nJets_ = theData.intUserData.back();

  // register corrected jets block
  correctedJetsUserBlock_.registerBlock( theData, "corrected_jets_", "corrected reference jets,");
  theData.intUserData.push_back( new UserData<int>("evt_ncorrectedjets", "number jets in the corrected jets reference collection", false) );
  nCorrectedJets_ = theData.intUserData.back();
  

}
