#include "PluginManager/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "ElectronAnalyzerFW.h"
#include "ElectronIdFW.h"

using cms1::ElectronAnalyzerFW;
using cms1::ElectronIdFW;

DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(ElectronAnalyzerFW);
DEFINE_ANOTHER_FWK_MODULE(ElectronIdFW);

