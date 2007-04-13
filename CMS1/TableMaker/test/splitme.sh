#!/bin/bash

n=0
for i in $( ls /pnfs/cms/WAX/8/cms1/DiLepton/samples/CMSSW_1_2_2/DiLepton_$1/store/unmerged/PreProd/2007/2/*/DiLepton_$1/FEVT/0000/ ) ; do

let n=$n+1

done

let events=$n*400

let jobs=$events/$2+1

echo "Process "$1
echo "Estimate "$events" Events" 
echo "Submitting "$jobs" Jobs"

mkdir -p $CMSSW_BASE/src/CMS1/TableMaker/test/results/$1/

cp $CMSSW_BASE/src/CMS1/TableMaker/test/analyze.tpl $CMSSW_BASE/src/CMS1/TableMaker/test/results/$1/$1.cfg

sed -i s/DATANAME/$1/g $CMSSW_BASE/src/CMS1/TableMaker/test/results/$1/$1.cfg

cat > $CMSSW_BASE/src/CMS1/TableMaker/test/results/$1/$1.condor <<EOF
universe              = vanilla
Executable            = $CMSSW_BASE/src/CMS1/TableMaker/test/analysis
should_transfer_files = NO
Output                = $CMSSW_BASE/src/CMS1/TableMaker/test/results/$1/\$(cluster)_\$(process).stdout
Error                 = $CMSSW_BASE/src/CMS1/TableMaker/test/results/$1/\$(cluster)_\$(process).stderr
Log                   = $CMSSW_BASE/src/CMS1/TableMaker/test/results/$1/\$(cluster)_\$(process).condor
Requirements          = Memory >= 199 && OpSys == "LINUX" && (Arch != "DUMMY")
notification          = never
Arguments             = \$(cluster) \$(process)  $CMSSW_BASE $CMSSW_BASE/src/CMS1/TableMaker/test/results/$1/ $CMSSW_BASE/src/CMS1/TableMaker/test/results/$1/$1.cfg CMS1/DataSets/CMSSW_1_2_2_DiLepton_$1_fnal.cff $2
Queue $jobs

EOF

condor_submit $CMSSW_BASE/src/CMS1/TableMaker/test/results/$1/$1.condor
