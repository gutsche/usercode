#!/bin/bash
#
# replace CONDOR_* variables in parameter-set to run it locally
#
# parameters:
#
# 1: input parameter-set
# 2: OutputFileName
#

cat $1 | sed -e s/CONDOR_OUTPUTFILENAME/$2/

