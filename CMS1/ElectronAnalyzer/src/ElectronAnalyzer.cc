#include "CMS1/ElectronAnalyzer/interface/ElectronAnalyzer.h"

#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Utilities/interface/EDMException.h"

#include "CMS1/Base/interface/EventData.h"
#include "CMS1/Electrons/interface/Electrons.h"
#include "CLHEP/HepMC/GenParticle.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"

#include "DataFormats/EgammaReco/interface/BasicClusterShapeAssociation.h"
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"

#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/TrackReco/interface/Track.h"

#include "CMS1/CommonTools/interface/MCTruth.h"

#include "DataFormats/Math/interface/LorentzVector.h"
#include <Math/VectorUtil.h>
#include <Math/Point3D.h>

#include "TFile.h"
#include "TH1D.h"
#include "TH1I.h"
#include "TH2D.h"
#include "TProfile.h"

using namespace std;
using namespace edm;
using namespace reco;
using namespace cms1;

void cms1::ElectronAnalyzer::configure(const ParameterSet& pset) {

  barrelClusterShapeAssocProducer_ = pset.getParameter<InputTag>("barrelClusterShapeAssociation");
  endcapClusterShapeAssocProducer_ = pset.getParameter<InputTag>("endcapClusterShapeAssociation");
  std::cout << barrelClusterShapeAssocProducer_.label() << "  " << barrelClusterShapeAssocProducer_.instance() << " " << 
    barrelClusterShapeAssocProducer_.process() << std::endl;

  nEvents = 0;

  hCharge = new TH1D("Charge", "Charge", 20, -10, 10);
  hGenDeltaR = new TH1D("Gen Delta R", "Gen Delta R", 100, 0, 0.1);
  hMass = new TH1D("Mass", "Mass", 100, 0, 0.1);
  hEnergy = new TH1D("Energy", "Energy", 100, 0, 100);
  hEt = new TH1D("Et", "Et", 100, 0, 100);
  hEta = new TH1D("Eta", "Eta", 50, 0, 2.5);
  hPhi = new TH1D("Phi", "Phi", 200, -4., 4.);  
  hS1overS9 = new TH1D("S1overS9", "S1overS9", 100, 0, 1);
  hEpE = new TH2D("Eoverp vs E", "Eoverp vs E", 100, 0, 2.5, 100, 0, 2.2); 
  hEpP = new TH2D("Eoverp vs p", "Eoverp vs p", 100, 0, 2.5, 100, 0, 2.2); 
  
  pEcal = new TProfile("prova1", "prova1", 50, 0, 50, 0, 1);
  pTk = new TProfile("prova2", "prova2", 50, 0, 50, 0, 1);
  pComb = new TProfile("prova3", "prova3", 50, 0, 50, 0, 1);

  hPin = new TH1D("Pin", "Pin", 100, 0, 100);
  hPout = new TH1D("Pout", "Pout", 100, 0, 100);
  hDeltaPhi = new TH1D("DeltaPhi", "DeltaPhi", 200, 0, 0.2);
  hEoverP = new TH1D("E over P", "E over P", 200, 0, 2);
  hHcaloverEcal = new TH1D("Hcal ove Ecal", "Hcal over Ecal", 200, -0.3, 0.3);

  hNCluster = new TH1I("NSuperCluster", "NSuperCluster", 20, 0, 20);

  char a[30];
  for(int i=0; i<4; i++) { 
    sprintf(a, "Reso type %d", i+1);
    pReso[i] = new TProfile(a, a, 50, 0, 50, 0, 1);
    sprintf(a, "e type %d", i+1);
    hClassEta[i] = new TH1D(a, a, 100, 0, 2.5);
    sprintf(a, "ErecEgen %d", i+1);
    hEEgen[i] = new TH1D(a, a, 150, 0.8, 1.1);
    //sprintf(a, "Reso e type %d", i+1);
    //hEReso[i] = new TH1D(a, a, 100, -10, 10);
  }
}

void cms1::ElectronAnalyzer::finishProcessing() {

  TFile* file = new TFile("pixelMatch.root", "recreate");
  hPin->Write();
  hPout->Write();
  hDeltaPhi->Write();
  hEoverP->Write();
  pTk->Write();
  pEcal->Write();
  pComb->Write(); 
  hHcaloverEcal->Write();

  hEpE->Write();
  hEpP->Write();

  hCharge->Write();
  hGenDeltaR->Write();
  hMass->Write();
  hEnergy->Write();
  hEt->Write();
  hEta->Write();
  hPhi->Write();
  hS1overS9->Write();
  hNCluster->Write();
  
  for(int i=0; i<4; i++) {
    pReso[i]->Write();
    hClassEta[i]->Write();
    hEEgen[i]->Write();
  }
  file->Close();
}

void cms1::ElectronAnalyzer::processEvent(const Event & event) {

  cout << "Run: " << event.id().run() << " Event: " << event.id().event() << endl;
  nEvents++;

  Cuts cuts;
  vector<const reco::Candidate*>::const_iterator it;
  vector<const reco::Candidate*> el[4];

  el[0] = theElectrons.getElectrons(Electrons::Golden, cuts);
  el[1] = theElectrons.getElectrons(Electrons::BigBrem, cuts);
  el[2] = theElectrons.getElectrons(Electrons::Narrow, cuts);
  el[3] = theElectrons.getElectrons(Electrons::Showering, cuts);
  
  // to define a Custom electron you must define an object ElectronDef setting the cuts (default values are those for "Golden")
  //ElectronDef def;
  //ElectronDef def(numberOfSCSeed, eOverP, deltaPhi, fBrem, hOverE, R9);
  // Then ask for Custom electrons...
  //el[3] = theElectrons.getElectrons(Electrons::Custom, cuts, Cuts::NotIsolated, def);

  for(int j=0; j<4; j++) {
    for(it = el[j].begin(); it != el[j].end(); ++it) {

      const reco::PixelMatchGsfElectron* iele = (const reco::PixelMatchGsfElectron*)*it; 
      hMass->Fill(iele->mass());
      hEnergy->Fill(iele->energy());
      hEt->Fill(iele->et());
      hEta->Fill(iele->eta());
      hPhi->Fill(iele->phi());

      reco::SuperClusterRef sclRef=iele->superCluster();

      // Get association maps linking BasicClusters to ClusterShape
      edm::Handle<reco::BasicClusterShapeAssociationCollection> barrelClShpHandle = 
        theData.getHandle<reco::BasicClusterShapeAssociationCollection>(barrelClusterShapeAssocProducer_.label(), barrelClusterShapeAssocProducer_.instance());
      
      //event.getByLabel(barrelClusterShapeAssocProducer_, barrelClShpHandle);
      edm::Handle<reco::BasicClusterShapeAssociationCollection> endcapClShpHandle =
        theData.getHandle<reco::BasicClusterShapeAssociationCollection>(endcapClusterShapeAssocProducer_.label(), endcapClusterShapeAssocProducer_.instance());;
      //event.getByLabel(endcapClusterShapeAssocProducer_, endcapClShpHandle);

      reco::BasicClusterShapeAssociationCollection::const_iterator seedShpItr;

      // Find the entry in the map corresponding to the seed BasicCluster of the SuperCluster
      DetId id = sclRef->seed()->getHitsByDetId()[0];
      if (id.subdetId() == EcalBarrel) {
        seedShpItr = barrelClShpHandle->find(sclRef->seed());
      } else {
        seedShpItr = endcapClShpHandle->find(sclRef->seed());
      }
      // Get the ClusterShapeRef corresponding to the BasicCluster
      const reco::ClusterShapeRef& seedShapeRef = seedShpItr->val;
      double ratio = seedShapeRef->eMax()/seedShapeRef->e3x3();
      hS1overS9->Fill(ratio);

      hNCluster->Fill(iele->numberOfClusters());

      math::XYZVector pin = iele->trackMomentumAtVtx();
      math::XYZVector pout = iele->trackMomentumAtCalo();
      
      //double deltaPhi = ROOT::Math::VectorUtil::DeltaPhi(pin, pout);
      hPin->Fill(pin.rho());
      hPout->Fill(pout.rho());
      hDeltaPhi->Fill(iele->deltaPhiSuperClusterTrackAtVtx());
      hEoverP->Fill(iele->eSuperClusterOverP());
      hHcaloverEcal->Fill(iele->hadronicOverEm());

      hClassEta[j]->Fill(fabs(iele->eta())); 

      MCTruth mc;
      mc.setEventData(&theData);
      
      const HepMC::GenParticle* temp = mc.matchCandToGen(*iele, MCInfo::Electrons, Cuts());
      
      if (temp) {
      
        //hGenDeltaR->Fill(dRmin);
        //ratio = (double)e.charge() / (double)output->pdg_id();
        //hCharge->Fill(ratio); 

        double r = sqrt(temp->momentum().px()*temp->momentum().px() + temp->momentum().py()*temp->momentum().py() + 
                        temp->momentum().pz()*temp->momentum().pz());
        
        if (fabs(temp->momentum().eta()) < 2.5 && abs(temp->pdg_id()) == 11) {
          hEpE->Fill(iele->eSuperClusterOverP(), iele->energy()/ temp->momentum().e());
          hEpP->Fill(iele->eSuperClusterOverP(), iele->trackMomentumAtVtx().R()/r);
        }
        
        hEEgen[j]->Fill(iele->energy()/ temp->momentum().e());
        double reso = fabs(iele->energy() - temp->momentum().e()) / temp->momentum().e();
        pReso[j]->Fill(temp->momentum().e(), reso); 
        
        reso = fabs(iele->caloEnergy() - temp->momentum().e()) / temp->momentum().e();
        pEcal->Fill(temp->momentum().e(), reso); 
        
        double tkE = sqrt(iele->p() * iele->p() + 0.0005*0.0005); 
        reso = fabs(tkE - temp->momentum().e()) / temp->momentum().e();
        pTk->Fill(temp->momentum().e(), reso); 
        
        reso = fabs(iele->energy() - temp->momentum().e()) / temp->momentum().e();
        pComb->Fill(temp->momentum().e(), reso); 
      }
    }
  } 
}

