
#include "PluginManager/ModuleDef.h"

#include "FWCore/Framework/interface/MakerMacros.h"

#include "Gutsche/RoadSearchSeedHitDumper/interface/RoadSearchSeedHitDumper.h"

using cms::RoadSearchSeedHitDumper;
DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(RoadSearchSeedHitDumper)
