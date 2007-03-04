#include "PluginManager/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "CMS1/MCInfo/interface/testMCInfo.h"
using cms1::testMCInfo;


DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(testMCInfo);

