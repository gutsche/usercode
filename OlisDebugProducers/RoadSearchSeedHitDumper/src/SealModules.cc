
#include "PluginManager/ModuleDef.h"

#include "FWCore/Framework/interface/MakerMacros.h"

#include "OlisDebugProducers/RoadSearchSeedHitDumper/interface/RoadSearchSeedHitDumper.h"

using cms::RoadSearchSeedHitDumper;
DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(RoadSearchSeedHitDumper)
