#include "PluginManager/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "MCTruthAnalyzerFW.h"
#include "MCInfoAnalyzerFW.h"

using cms1::MCTruthAnalyzerFW;
using cms1::MCInfoAnalyzerFW;

DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(MCTruthAnalyzerFW);
DEFINE_ANOTHER_FWK_MODULE(MCInfoAnalyzerFW);
