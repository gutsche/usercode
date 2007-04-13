#!/bin/bash

processes="DrellYan HWW TTbar WW WZ Zee Zmumu Ztautau ZZ"

for process in $processes
do
$CMSSW_BASE/src/CMS1/TableMaker/test/splitme.sh $process 2000
done
