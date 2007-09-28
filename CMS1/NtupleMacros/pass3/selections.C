//===========================================================
//
// Various selection functions are kept here
//
//============================================================
#include "Math/LorentzVector.h"
#include "TMath.h"
#include "selections.h"
#include <vector>
#include "CMS1.h"
#include "TDatabasePDG.h"
//----------------------------------------------------------------
// Global Muon selection
//---------------------------------------------------------------
bool goodGlobalMuon(int index) {
  
  bool good = true;
  if (mus_gfit_chi2->at(index)/mus_gfit_ndof->at(index) > 5.) good=false;
  if (abs(mus_d0->at(index))   > 0.25) good=false;
  if (mus_validHits->at(index) < 7)    good = false;
  return good;
}
//--------------------------------------------
// Pass 2 MET selection
//--------------------------------------------
bool pass2Met() {
  // for e-e and mu-mu
  if (hyp_type == 0 || hyp_type == 3) {
    if (hyp_met < 30) return false;
    if ( fabs(hyp_p4->mass()-90.0)<10.0) return false;
    if( hyp_met/hyp_p4->pt()<0.6 && 
       acos(cos(hyp_metPhi-hyp_p4->phi()-3.1416))<0.25 ) return false;
  }
  // for e-mu and mu-e
  if (hyp_type == 1 || hyp_type == 2) {
    if (hyp_met < 20) return false;
  }
  return true;
}
//----------------------------------------
// Pass2 Isolation
//----------------------------------------
bool pass2Iso() { 
  if ( hyp_lt_iso > 5.0 ) return false;
  if ( hyp_ll_iso > 5.0 ) return false;
  return true;
}
//-----------------------------
// Full Pass2 selection
//----------------------------
bool pass2Selection(bool oppSign) {
  if (!pass2Met()) return false;
  if (!pass2Iso()) return false;
  if (oppSign) {
    if ( hyp_lt_id * hyp_ll_id > 0 ) return false;
  } else {
    if ( hyp_lt_id * hyp_ll_id < 0 ) return false;
  }
  return true;
}
//---------------------------------------------
// Pass2 selection with better muon isolation
//--------------------------------------------
bool pass2SelectionWithBetterMuIso(bool oppSign) {
  if (!pass2Met()) return false;
  if (oppSign) {
    if ( hyp_lt_id * hyp_ll_id > 0 ) return false;
  } else {
    if ( hyp_lt_id * hyp_ll_id < 0 ) return false;
  }

  if (abs(hyp_lt_id) == 13) {
    if (!betterMuonIsolation(hyp_lt_index)) return false;
  } else {
    if (hyp_lt_iso > 5) return false;
  }

  if (abs(hyp_ll_id) == 13) {
    if (!betterMuonIsolation(hyp_ll_index)) return false;
  } else {
    if (hyp_ll_iso > 5) return false;
  }

  return true;
}
//-------------------------------------------------
// Better Muon Isolation
//-------------------------------------------------
bool betterMuonIsolation(int index) {
  if (mus_iso03_sumPt->at(index)+mus_iso03_emEt->at(index)+mus_iso03_hadEt->at(index)>6.5)  
    return false;
  return true;
}
//-------------------------------------------------
// Auxiliary function to scan the doc line and 
// identify DY-> ee vs mm vs tt
//-------------------------------------------------
int getDrellYanType() {
  bool foundZ;
  int size = genps_id->size();
  for (int jj=0; jj<size; jj++) {
    if (genps_id->at(jj) == 23) {
      foundZ = true;
      if (jj+3 > size) {
	std::cout << 
         "Found Z but not enough room in doc lines for leptons?" << std::endl;
	return 999;
      }
      if (abs(genps_id->at(jj+1)) == 11) return 0;  //DY->ee
      if (abs(genps_id->at(jj+1)) == 13) return 1;  //DY->mm
      if (abs(genps_id->at(jj+1)) == 15) return 2;  //DY->tautau
    }
  }
  std::cout << "Does not look like a DY event" << std::endl;
  return 999;
}
//--------------------------------------------
// Booleans for DY
//------------------------------------------
bool isDYee() {
  if (getDrellYanType() == 0) return true;
  return false;
}
bool isDYmm() {
  if (getDrellYanType() == 1) return true;
  return false;
}
bool isDYtt() {
  if (getDrellYanType() == 2) return true;
  return false;
}
//------------------------------------------------------------
// Not a selection function per se, but useful nonetheless:
// dumps the documentation lines for this event
//------------------------------------------------------------
void dumpDocLines() {
  int size = genps_id->size();
  // Initialize particle database
  TDatabasePDG *pdg = new TDatabasePDG();
  std::cout << "------------------------------------------" << std::endl;
  for (int j=0; j<size; j++) {
    cout << setw(9) << left << pdg->GetParticle(genps_id->at(j))->GetName() << " "
         << setw(7) << right << setprecision(4) << genps_p4->at(j).pt() << "  "
         << setw(7) << right << setprecision(4) << genps_p4->at(j).phi() << "  "
         << setw(10) << right << setprecision(4) << genps_p4->at(j).eta() << "  "
         << setw(10) << right << setprecision(4) << genps_p4->at(j).mass() << endl;
  }
  // Clean up
  delete pdg;
}
//------------------------------------------------------
// Here come the electron selections
//------------------------------------------------------

// First row loose selection thresholds, second row tight thresholds
float bcutsee[16] = {0.014,  0.0125, 0.0125, 0.0125, 0.0125, 0.0125, 0.0125, 0.0125, 
                     0.014,  0.012,  0.012,  0.012,  0.012,  0.012,  0.012,  0.012};
float bcutdeta[16]= {0.009,  0.0085, 0.0085, 0.0035, 0.0085, 0.0035, 0.0085, 0.0085,  
                     0.009,  0.0055, 0.0055, 0.004,  0.0055, 0.004,  0.0055, 0.0055};
float bcutdphi[16]= {0.06,   0.06,   0.10,   0.02,   0.06,   0.06,   0.06,   0.06,      
                     0.04,   0.04,   0.07,   0.02,   0.04,   0.04,   0.04,   0.04};
float bcuteop2[16]= {0.55,   0.88,   0.88,   0.88,   0.88,   0.88,   0.88,   0.88,      
                     0.55,   0.91,   0.91,   0.91,   0.91,   0.91,   0.91,   0.91};
float bcuthoe[16] = {0.125,  0.055,  0.055,  0.10,   0.055,  0.055,  0.055,  0.055,     
                     0.12,   0.055,  0.055,  0.095,  0.055,  0.055,  0.055,  0.055};
float ecutsee[16] = {0.031,  0.028,  0.028,  0.028,  0.028,  0.028,  0.028,  0.028,     
                     0.027,  0.026,  0.026,  0.026,  0.026,  0.026,  0.026,  0.026};
float ecutdeta[16]= {0.009,  0.009,  0.009,  0.009,  0.009,  0.009,  0.009,  0.009,     
                     0.008,  0.008,  0.008,  0.008,  0.008,  0.008,  0.008,  0.008};
float ecutdphi[16]= {0.06,   0.10,   0.10,   0.02,   0.10,   0.10,   0.10,   0.10,      
                     0.055,  0.085,  0.085,  0.02,   0.085,  0.085,  0.085,  0.085};
float ecuteop2[16]= {0.85,   0.85,   0.85,   0.85,   0.85,   0.85,   0.85,   0.85,      
                     0.85,   0.85,   0.85,   0.85,   0.85,   0.85,   0.85,   0.85};
float ecuthoe[16] = {0.125,  0.10,   0.10,   0.10,   0.10,   0.10,   0.10,   0.10,      
                     0.05,   0.03,   0.03,   0.03,   0.03,   0.03,   0.03,   0.03};

int electron_category(float eOverP, float fBrem) {
    
  int cat=6;
  if(eOverP > 0.9*(1-fBrem)) {
    if(fBrem > 0.06) {
      if(eOverP > 1.5) 
        cat=2;
      else if(eOverP > 1.2) 
        cat=1;
      else if(eOverP > 0.8) 
        cat=0;
      else 
        cat=4;
    }
    else if(eOverP > 1.5) 
      cat=5;
    else 
      cat=3;
  } 
  
  return cat;
}

// type = 0, loose selection
// type = 1, tight selection

bool electron_selection(int index, int type) {

  bool selj;
  int cat = electron_category(els_eOverPIn->at(index), els_fBrem->at(index));
  
  //  Chose cuts from barrel or endcap
  float seecor = els_sigmaEtaEta->at(index);
  float cutsee;
  float cutdeta;
  float cutdphi;
  float cuteop2;
  float cuthoe;
  if(fabs(els_p4->at(index).Eta()) < 1.479) {
    cutsee = bcutsee[cat+type*8]; 
    cutdeta = bcutdeta[cat+type*8]; 
    cutdphi = bcutdphi[cat+type*8]; 
    cuteop2 = bcuteop2[cat+type*8]; 
    cuthoe = bcuthoe[cat+type*8];
  } else {
    cutsee = ecutsee[cat+type*8]; 
    cutdeta = ecutdeta[cat+type*8]; 
    cutdphi = ecutdphi[cat+type*8]; 
    cuteop2 = ecuteop2[cat+type*8];   
    cuthoe = ecuthoe[cat+type*8];
    seecor = seecor-0.02*(fabs(els_p4->at(index).Eta()) - 2.3);       // linear correction for sigma-eta-eta in endcap
  }
  
  //  Apply the 5 cuts
  selj = (seecor < cutsee) && (fabs(els_dEtaIn->at(index)) < cutdeta) && (fabs(els_dPhiIn->at(index)) < cutdphi) && (els_eSeedOverPOut->at(index) > cuteop2) && (els_hOverE->at(index) < cuthoe);
  
  return selj;
}

bool simplified_electron_selection(int index) {
  
  bool selj;
  
  selj = (fabs(els_p4->at(index).Eta()) > 1.479 || els_sigmaEtaEta->at(index) < 0.012) && els_sigmaEtaEta->at(index) < 0.03 && 
    fabs(els_dEtaIn->at(index)) < 0.012 && els_hOverE->at(index) < 0.1 && els_eOverPIn->at(index) > 0.9 && 
    sqrt(pow(els_dEtaIn->at(index), 2) + pow(els_dPhiIn->at(index), 2)) < 0.1;
  
  return selj;
}
//-------------------------------------------------------
// Done with electron selections
//-------------------------------------------------------
