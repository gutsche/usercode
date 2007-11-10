#include "PluginManager/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "NtupleMakerFW.h"
#include "EdmNtupleMakerFW.h"
using cms1::NtupleMakerFW;
using cms1::EdmNtupleMakerFW;

DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(NtupleMakerFW);
DEFINE_ANOTHER_FWK_MODULE(EdmNtupleMakerFW);
