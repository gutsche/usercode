#ifndef ElectronAnalyzer_H
#define ElectronAnalyzer_H

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "CLHEP/HepMC/GenParticle.h"
#include "CMS1/Electrons/interface/Electrons.h"

namespace edm {
  class ParameterSet;
  class Event;
  class EventSetup;
}

class TFile;
class TH1D;
class TH1I;
class TH2D;
class TProfile;

class ElectronAnalyzer: public edm::EDAnalyzer {
public:

  ElectronAnalyzer(const edm::ParameterSet& pset);
  virtual ~ElectronAnalyzer();
    
  HepMC::GenParticle match(reco::PixelMatchGsfElectron e, std::vector<HepMC::GenParticle> genParticles);

  void analyze(const edm::Event & event, const edm::EventSetup& eventSetup);
  void beginJob(const edm::EventSetup& eventSetup);
  void endJob();

private:

  edm::InputTag barrelClusterShapeAssocProducer_;
  edm::InputTag endcapClusterShapeAssocProducer_;
  std::string theElectronLabel, theMCLabel;

  int nEvents, nGenJets, nRecoJets;
  double seedThr, towerThr, coneSize;

  TProfile *pReso[4], *pEcal, *pTk, *pComb;
  //TH1I *hHits;
  TH1D *hPin, *hPout, *hDeltaPhi, *hEoverP, *hHcaloverEcal;
  TH1D *hClassEta[4], *hEEgen[4];
  TH1D *hCharge, *hGenDeltaR, *hMass, *hEnergy, *hEt, *hEta, *hPhi, *hS1overS9;
  TH2D *hEpE, *hEpP;
  TH1I *hNCluster;

};
#endif
