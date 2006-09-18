#!/bin/bash

#
# variables from arguments string in jdl
#
# format:
#
# 1:  condor cluster number
# 2:  condor process number
# 3:  CMSSW_DIR
# 4:  RUN_DIR
# 5:  PARAMETER_SET (full path, has to contain all needed files in PoolSource and filled following variables with keywords: FrameworkJobReport xml to CONDOR_FRAMEWORKJOBREPORT, maxEvents = CONDOR_MAXEVENTS, skipEvents = CONDOR_SKIPEVENTS, output fileName = CONDOR_OUTPUTFILENAME)
# 6:  RANDOM_NUMBER (incremented by 1000*process number)
# 7:  PSET_HACK
# 8:  FIRSTRUN (incremented by 10*process number)
# 9:  MAXEVENTS
#

CONDOR_CLUSTER=$1
CONDOR_PROCESS=$2
CMSSW_DIR=$3
RUN_DIR=$4
PARAMETER_SET=$5
RANDOM_NUMBER=$6
PSET_HACK=$7
FIRSTRUN=$8
MAXEVENTS=$9


#
# header 
#

echo ""
echo "CMSSW on Condor"
echo ""

START_TIME=`/bin/date`
echo "started at $START_TIME"

echo ""
echo "parameter set:"
echo "CONDOR_CLUSTER: $CONDOR_CLUSTER"
echo "CONDOR_PROCESS: $CONDOR_PROCESS"
echo "CMSSW_DIR: $CMSSW_DIR"
echo "RUN_DIR: $RUN_DIR"
echo "PRAMETER_SET: $PARAMETER_SET"
echo "RANDOM_NUMBER: $RANDOM_NUMBER"
echo "PSET_HACK: $PSET_HACK"
echo "FIRSTRUN: $FIRSTRUN"
echo "MAXEVENTS: $MAXEVENTS"


#
# setup software environment at FNAL for the given CMSSW release
#
source /afs/fnal.gov/files/code/cms/setup/shrc uaf
export SCRAM_ARCH=slc3_ia32_gcc323
cd $CMSSW_DIR
eval `scramv1 runtime -sh`

#
# change to output directory
#
cd $RUN_DIR

#
# modify parameter-set
#

FINAL_PARAMETER_SET_NAME=`echo simulate_${CONDOR_CLUSTER}_${CONDOR_PROCESS}`
FINAL_PARAMETER_SET=`echo $FINAL_PARAMETER_SET_NAME.cfg`
FINAL_LOG=`echo $FINAL_PARAMETER_SET_NAME.log`
FINAL_OUTPUTFILENAME=`echo $FINAL_PARAMETER_SET_NAME.root`
FINAL_FRAMEWORKJOBREPORT=`echo $FINAL_PARAMETER_SET_NAME`
let "FINAL_RANDOMNUMBER = $CONDOR_PROCESS*100 + $RANDOM_NUMBER"
FINAL_PSETHACK=$PSET_HACK
let "FINAL_FIRSTRUN = $CONDOR_PROCESS*100 + $FIRSTRUN"
FINAL_MAXEVENTS=$MAXEVENTS

echo ""
echo "Writing final parameter-set: $FINAL_PARAMETER_SET to RUN_DIR: $RUN_DIR"
echo ""

cat $PARAMETER_SET | sed -e s/CONDOR_FRAMEWORKJOBREPORT/$FINAL_FRAMEWORKJOBREPORT/ \
| sed -e s/CONDOR_RANDOMNUMBER/$FINAL_RANDOMNUMBER/ \
| sed -e s/CONDOR_PSETHACK/"$FINAL_PSETHACK"/ \
| sed -e s/CONDOR_FIRSTRUN/$FINAL_FIRSTRUN/ \
| sed -e s/CONDOR_MAXEVENTS/$FINAL_MAXEVENTS/ \
| sed -e s/CONDOR_OUTPUTFILENAME/$FINAL_OUTPUTFILENAME/  > $FINAL_PARAMETER_SET

#
# run cmssw
#

echo "run: time cmsRun $FINAL_PARAMETER_SET > $FINAL_LOG 2>&1"
cmsRun $FINAL_PARAMETER_SET >> $FINAL_LOG 2>&1
exitcode=$?

#
# end run
#

echo ""
START_TIME=`/bin/date`
echo "finished at $START_TIME"
exit $exitcode
