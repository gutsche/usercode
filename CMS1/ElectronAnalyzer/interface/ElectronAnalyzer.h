#ifndef ElectronAnalyzer_H
#define ElectronAnalyzer_H

//#include "FWCore/Framework/interface/EDAnalyzer.h"
//#include "FWCore/Framework/interface/Event.h"
//#include "DataFormats/Common/interface/Handle.h"
//#include "FWCore/Framework/interface/ESHandle.h"
//#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "CLHEP/HepMC/GenParticle.h"
#include "CMS1/Electrons/interface/Electrons.h"

#include "CMS1/BaseAnalyzer/interface/BaseAnalyzer.h"

//namespace edm {
//  class ParameterSet;
//  class Event;
//  class EventSetup;
//}

class TFile;
class TH1D;
class TH1I;
class TH2D;
class TProfile;

namespace cms1 {
  class ElectronAnalyzer: public BaseAnalyzer {
  public:
    ElectronAnalyzer() {};
    virtual ~ElectronAnalyzer() {};
    
  protected:
    virtual void configure(const edm::ParameterSet& pset);
    //const HepMC::GenParticle* match(reco::PixelMatchGsfElectron e);  
    virtual void processEvent(const edm::Event & event);
    virtual void finishProcessing();

  private:
    edm::InputTag barrelClusterShapeAssocProducer_;
    edm::InputTag endcapClusterShapeAssocProducer_;
    std::string theElectronLabel, theMCLabel;
    
    int nEvents, nGenJets, nRecoJets;
    double seedThr, towerThr, coneSize;
    
    TProfile *pReso[4], *pEcal, *pTk, *pComb;
    TH1D *hPin, *hPout, *hDeltaPhi, *hEoverP, *hHcaloverEcal;
    TH1D *hClassEta[4], *hEEgen[4];
    TH1D *hCharge, *hGenDeltaR, *hMass, *hEnergy, *hEt, *hEta, *hPhi, *hS1overS9;
    TH2D *hEpE, *hEpP;
    TH1I *hNCluster;
  };
}
#endif
