
#include "PluginManager/ModuleDef.h"

#include "FWCore/Framework/interface/MakerMacros.h"

#include "Streaming/StreamDecider/interface/StreamDecider.h"

using cms::StreamDecider;
DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(StreamDecider)
