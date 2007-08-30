// Description:     muon streamer class 
//
// Original Author: Dmytro Kovalskyi
//
// $Author: dmytro $
// $Date: 2007/08/07 11:13:30 $
// $Revision: 1.1 $
//
#include "DataFormats/MuonReco/interface/Muon.h"
#include "CMS1/Base/interface/MuonStreamer.h"
#include "RecoMuon/MuonIdentification/interface/IdGlobalFunctions.h"

cms1::MuonStreamer::MuonStreamer() 
{
   varNMatches     = addInt("nmatches",      " number of stations with matched segments", -999 );
   varEnergy_em    = addFloat("e_em",        " energy in crossed ECAL crystalls", -999);
   varEnergy_had   = addFloat("e_had",       " energy in crossed HCAL towers", -999);
   varEnergy_ho    = addFloat("e_ho",        " energy in crossed HO towers", -999);
   varEnergy_emS9  = addFloat("e_emS9",      " energy in 3x3 ECAL crystall shape", -999);
   varEnergy_hadS9 = addFloat("e_hadS9",     " energy in 3x3 HCAL towers", -999);
   varEnergy_hoS9  = addFloat("e_hoS9",      " energy in 3x3 HO towers", -999);
   varIsoR03_sumPt = addFloat("iso03_sumPt", " sum of track Pt for cone of 0.3", -999);
   varIsoR03_emEt  = addFloat("iso03_emEt",  " sum of ecal Et for cone of 0.3", -999);
   varIsoR03_hadEt = addFloat("iso03_hadEt", " sum of hcal Et for cone of 0.3", -999);
   varIsoR03_hoEt  = addFloat("iso03_hoEt",  " sum of ho Et for cone of 0.3", -999);
   varIsoR03_ntrk  = addInt("iso03_ntrk",    " number of tracks in the cone of 0.3", -999);
   varIsoR05_sumPt = addFloat("iso05_sumPt", " sum of track Pt for cone of 0.5", -999);
   varIsoR05_emEt  = addFloat("iso05_emEt",  " sum of ecal Et for cone of 0.5", -999);
   varIsoR05_hadEt = addFloat("iso05_hadEt", " sum of hcal Et for cone of 0.5", -999);
   varIsoR05_hoEt  = addFloat("iso05_hoEt",  " sum of ho Et for cone of 0.5", -999);
   varIsoR05_ntrk  = addInt("iso05_ntrk",    " number of tracks in the cone of 0.5", -999);
   var_pid_TMLastStationLoose = 
     addInt("pid_TMLastStationLoose",     " loose tracker muon identification based on muon/hadron penetration depth difference", -999);
   var_pid_TMLastStationTight = 
     addInt("pid_TMLastStationTight",     " tight tracker muon identification based on muon/hadron penetration depth difference", -999);
   var_pid_TM2DCompatibilityLoose = 
     addInt("pid_TM2DCompatibilityLoose", " loose tracker muon likelihood identification based on muon matches and calo depositions", -999);
   var_pid_TM2DCompatibilityTight = 
     addInt("pid_TM2DCompatibilityTight", " tight tracker muon likelihood identification based on muon matches and calo depositions", -999);
   var_gfit_chi2 = addFloat("gfit_chi2",  " chi2 of the global muon fit", -999); 
   var_gfit_ndof = addFloat("gfit_ndof",  " number of degree of freedom of the global muon fit", -999);
   var_gfit_validHits = addInt("gfit_validHits", " number of valid hits of the global muon fit", -999);
}

void cms1::MuonStreamer::fill( const reco::Candidate* candidate, bool reset )
{
   if ( reset ) setDefaults();
   if (! candidate) return;
   TrackStreamer::fill(candidate,false);
   if ( const reco::Muon* aMuon = dynamic_cast<const reco::Muon*>(candidate) )
     {
	*varEnergy_em    = aMuon->isEnergyValid() ? aMuon->getCalEnergy().em: -999;
	*varEnergy_had   = aMuon->isEnergyValid() ? aMuon->getCalEnergy().had: -999;
	*varEnergy_ho    = aMuon->isEnergyValid() ? aMuon->getCalEnergy().ho: -999;
	*varEnergy_emS9  = aMuon->isEnergyValid() ? aMuon->getCalEnergy().emS9: -999;
	*varEnergy_hadS9 = aMuon->isEnergyValid() ? aMuon->getCalEnergy().hadS9: -999;
	*varEnergy_hoS9  = aMuon->isEnergyValid() ? aMuon->getCalEnergy().hoS9: -999;
	
	// hack for 15X
	reco::Muon* muon = const_cast<reco::Muon*>(aMuon);
	
	*varNMatches     = muon->isMatchesValid() ? muon->numberOfMatches(): -999;
	*varIsoR03_ntrk  = muon->isIsolationValid() ? muon->getIsolationR03().nTracks:-999;
	*varIsoR05_ntrk  = muon->isIsolationValid() ? muon->getIsolationR05().nTracks:-999;
	*varIsoR03_sumPt = muon->isIsolationValid() ? muon->getIsolationR03().sumPt:-999;
	*varIsoR05_sumPt = muon->isIsolationValid() ? muon->getIsolationR05().sumPt:-999;
	*varIsoR03_emEt  = muon->isIsolationValid() ? muon->getIsolationR03().emEt:-999;
	*varIsoR05_emEt  = muon->isIsolationValid() ? muon->getIsolationR05().emEt:-999;
	*varIsoR03_hadEt = muon->isIsolationValid() ? muon->getIsolationR03().hadEt:-999;
	*varIsoR05_hadEt = muon->isIsolationValid() ? muon->getIsolationR05().hadEt:-999;
	*varIsoR03_hoEt  = muon->isIsolationValid() ? muon->getIsolationR03().hoEt:-999;
	*varIsoR05_hoEt  = muon->isIsolationValid() ? muon->getIsolationR05().hoEt:-999;
	
	*var_gfit_chi2 = muon->combinedMuon().isNonnull() ? muon->combinedMuon()->chi2() : -999;
	*var_gfit_ndof = muon->combinedMuon().isNonnull() ? muon->combinedMuon()->ndof() : -999;
	*var_gfit_validHits = muon->combinedMuon().isNonnull() ? muon->combinedMuon()->numberOfValidHits() : -999;
	
	// fill muon id flags.
        if ( muon->isMatchesValid() ) {
	   *var_pid_TMLastStationLoose = muonid::isGoodMuon(*muon,muonid::TMLastStationLoose);
	   *var_pid_TMLastStationTight = muonid::isGoodMuon(*muon,muonid::TMLastStationTight);
	   *var_pid_TM2DCompatibilityLoose = muonid::isGoodMuon(*muon,muonid::TM2DCompatibilityLoose);
	   *var_pid_TM2DCompatibilityTight = muonid::isGoodMuon(*muon,muonid::TM2DCompatibilityTight);
	}
     }
}

void cms1::MuonStreamer::fill( const StreamerArguments& args, bool reset )
{
   if ( reset ) setDefaults();
   TrackStreamer::fill(args,false);
   if ( args.candidate ) fill(args.candidate,false);
}

