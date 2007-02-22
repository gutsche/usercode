#include "PluginManager/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "CMS1/TableMaker/interface/TableMaker.h"
using cms1::TableMaker;

DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(TableMaker);
