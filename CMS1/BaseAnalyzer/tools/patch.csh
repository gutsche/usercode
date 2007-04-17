#!/bin/csh -f
if (! $?CMSSW_BASE) then
  echo "CMSSW release evironment is not set. Cannot patch the code."
  exit 1
endif
cd $CMSSW_BASE/src

if (! -e $CMSSW_BASE/src/FWCore/ParameterSet) then
  echo "FWCore/ParameterSet is not checked out. Cannot patch the code."
  exit 1
endif

echo "replacing boost::int64_t with long long int in ParameterSet"
find FWCore/ParameterSet -type f | xargs -i perl -pi -e 's/boost::int64_t/long long int/' {}

echo "uncommenting reflex disctionary generation"
perl -pi -e 's/\/\///' CMS1/BaseAnalyzer/src/classes.h
perl -pi -e 's/\/\///' CMS1/SampleAnalyzer/src/classes.h
perl -pi -e 's/\/\///' CMS1/TableMaker/src/classes.h
perl -pi -e 's/(\<\!--|--\>)//' CMS1/BaseAnalyzer/src/classes_def.xml
perl -pi -e 's/(\<\!--|--\>)//' CMS1/SampleAnalyzer/src/classes_def.xml
perl -pi -e 's/(\<\!--|--\>)//' CMS1/TableMaker/src/classes_def.xml
echo "done"
