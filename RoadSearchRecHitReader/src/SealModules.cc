
#include "PluginManager/ModuleDef.h"

#include "FWCore/Framework/interface/MakerMacros.h"

#include "Gutsche/RoadSearchRecHitReader/interface/RoadSearchRecHitReader.h"

using cms::RoadSearchRecHitReader;
DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(RoadSearchRecHitReader)
