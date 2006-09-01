#!/bin/bash
#
# replace CONDOR_* variables in parameter-set to run it locally
#
# parameters:
#
# 1: parameter-set
# 2: CONDOR_FRAMEWORKJOBREPORT
# 3: CONDOR_MAXEVENTS
# 4: CONDOR_SKIPEVENTS
# 5: CONDOR_OUTPUTFILENAME
#

cat $1 | sed -e s/CONDOR_FRAMEWORKJOBREPORT/$2/ | sed -e s/CONDOR_MAXEVENTS/$3/ | sed -e s/CONDOR_SKIPEVENTS/$4/ | sed -e s/CONDOR_OUTPUTFILENAME/$5/

