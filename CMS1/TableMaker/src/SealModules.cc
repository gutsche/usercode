#include "PluginManager/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "CMS1/TableMaker/interface/TableMakerFW.h"
using cms1::TableMakerFW;

DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(TableMakerFW);
