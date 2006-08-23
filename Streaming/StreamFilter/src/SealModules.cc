
#include "PluginManager/ModuleDef.h"

#include "FWCore/Framework/interface/MakerMacros.h"

#include "Streaming/StreamFilter/interface/StreamFilter.h"

using cms::StreamFilter;
DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(StreamFilter)
