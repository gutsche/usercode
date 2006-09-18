#!/bin/bash

condor_submit profile-bjets.jdl
condor_submit profile-minbias.jdl
condor_submit profile-singlemu.jdl
condor_submit profile-ttbar.jdl
condor_submit profile-z700.jdl
condor_submit profile-zmumu.jdl
