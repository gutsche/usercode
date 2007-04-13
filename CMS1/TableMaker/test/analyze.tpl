process A = {

  source = PoolSource
  {
    untracked vstring fileNames = 
    {
      "file:/afs/fnal.gov/files/home/room2/dmytro/public/datasamples/zmumu_reco100_1000.root"
    }
    untracked int32  maxEvents  = -1
    untracked uint32 skipEvents = 0
  }

include "CONDORDATASETCFF"
replace PoolSource.maxEvents  = CONDORMAXEVENTS
replace PoolSource.skipEvents = CONDORSKIPEVENTS

  # cms1 table maker
  include "CMS1/TableMaker/data/tableMaker.cff"
  # Description for created histograms
  replace tableMaker.fileTag = "DATANAME_CONDOR"
  
  path p = {
    tableMaker
  }

}
