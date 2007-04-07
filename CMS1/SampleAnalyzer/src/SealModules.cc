#include "PluginManager/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "CMS1/SampleAnalyzer/interface/SampleAnalyzerFW.h"
using cms1::SampleAnalyzerFW;

DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(SampleAnalyzerFW);
