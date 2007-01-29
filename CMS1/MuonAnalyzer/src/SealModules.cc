#include "PluginManager/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "CMS1/MuonAnalyzer/interface/MuonAnalyzer.h"
using cms1::MuonAnalyzer;

DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(MuonAnalyzer);
