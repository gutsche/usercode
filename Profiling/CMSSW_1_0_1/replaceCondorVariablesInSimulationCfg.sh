#!/bin/bash
#
# replace CONDOR_* variables in parameter-set to run it locally
#
# parameters:
#
# 1: parameter-set
# 2: CONDOR_FRAMEWORKJOBREPORT
# 3: CONDOR_RANDOMNUMBER
# 4: CONDOR_PSETHACK
# 4: CONDOR_FIRSTRUN
# 5: CONDOR_MAXEVENTS
# 6: CONDOR_OUTPUTFILENAME
#

cat $1 | sed -e s/CONDOR_FRAMEWORKJOBREPORT/$2/ \
| sed -e s/CONDOR_RANDOMNUMBER/$3/ \
| sed -e s/CONDOR_PSETHACK/$4/ \
| sed -e s/CONDOR_FIRSTRUN/$5/ \
| sed -e s/CONDOR_MAXEVENTS/$6/ \
| sed -e s/CONDOR_OUTPUTFILENAME/$7/ 
