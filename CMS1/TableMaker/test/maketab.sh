#!/bin/bash

for i in  DrellYan HWW TTbar WW WZ Zee Zmumu Ztautau ZZ ; do

echo $i

cd $CMSSW_BASE/src/CMS1/TableMaker/test/results/$i

rm -f ${i}_Results.root

hadd ${i}_Results.root *.root >& /dev/null

cat *.log | awk -f $CMSSW_BASE/src/CMS1/TableMaker/test/comblog.awk
echo " "
echo " "
echo " "
cd -

done
