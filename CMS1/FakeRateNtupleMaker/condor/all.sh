#!/bin/bash

./createProject --project_name $1_01 --parameter_set makeFakeRateNtuple.cfg --dataset_cff CMSSW_1_5_2_qcd_pt_0_15_fnal.cff --events 25000 --jobs 20
./createProject --project_name $1_02 --parameter_set makeFakeRateNtuple.cfg --dataset_cff CMSSW_1_5_2_qcd_pt_15_20_fnal.cff --events 25000 --jobs 51
./createProject --project_name $1_03 --parameter_set makeFakeRateNtuple.cfg --dataset_cff CMSSW_1_5_2_qcd_pt_20_30_fnal.cff --events 25000 --jobs 87
./createProject --project_name $1_04 --parameter_set makeFakeRateNtuple.cfg --dataset_cff CMSSW_1_5_2_qcd_pt_30_50_fnal.cff --events 25000 --jobs 43
./createProject --project_name $1_05 --parameter_set makeFakeRateNtuple.cfg --dataset_cff CMSSW_1_5_2_qcd_pt_50_80_fnal.cff --events 25000 --jobs 37
./createProject --project_name $1_06 --parameter_set makeFakeRateNtuple.cfg --dataset_cff CMSSW_1_5_2_qcd_pt_80_120_fnal.cff --events 25000 --jobs 50
./createProject --project_name $1_07 --parameter_set makeFakeRateNtuple.cfg --dataset_cff CMSSW_1_5_2_qcd_pt_120_170_fnal.cff --events 25000 --jobs 50
./createProject --project_name $1_08 --parameter_set makeFakeRateNtuple.cfg --dataset_cff CMSSW_1_5_2_qcd_pt_170_230_fnal.cff --events 25000 --jobs 37
./createProject --project_name $1_09 --parameter_set makeFakeRateNtuple.cfg --dataset_cff CMSSW_1_5_2_qcd_pt_230_300_fnal.cff --events 25000 --jobs 32
./createProject --project_name $1_10 --parameter_set makeFakeRateNtuple.cfg --dataset_cff CMSSW_1_5_2_qcd_pt_300_380_fnal.cff --events 25000 --jobs 51
#./createProject --project_name $1_11 --parameter_set makeFakeRateNtuple.cfg --dataset_cff CMSSW_1_5_2_qcd_pt_380_470_fnal.cff --events 25000 --jobs 
./createProject --project_name $1_12 --parameter_set makeFakeRateNtuple.cfg --dataset_cff CMSSW_1_5_2_qcd_pt_470_600_fnal.cff --events 25000 --jobs 53
./createProject --project_name $1_13 --parameter_set makeFakeRateNtuple.cfg --dataset_cff CMSSW_1_5_2_qcd_pt_600_800_fnal.cff --events 25000 --jobs 10
./createProject --project_name $1_14 --parameter_set makeFakeRateNtuple.cfg --dataset_cff CMSSW_1_5_2_qcd_pt_800_1000_fnal.cff --events 25000 --jobs 29
#./createProject --project_name $1_15 --parameter_set makeFakeRateNtuple.cfg --dataset_cff CMSSW_1_5_2_qcd_pt_1000_1400_fnal.cff --events 25000 --jobs 
./createProject --project_name $1_16 --parameter_set makeFakeRateNtuple.cfg --dataset_cff CMSSW_1_5_2_qcd_pt_1400_1800_fnal.cff --events 25000 --jobs 12
./createProject --project_name $1_17 --parameter_set makeFakeRateNtuple.cfg --dataset_cff CMSSW_1_5_2_qcd_pt_1800_2200_fnal.cff --events 25000 --jobs 13
./createProject --project_name $1_18 --parameter_set makeFakeRateNtuple.cfg --dataset_cff CMSSW_1_5_2_qcd_pt_2200_2600_fnal.cff --events 25000 --jobs 31
./createProject --project_name $1_19 --parameter_set makeFakeRateNtuple.cfg --dataset_cff CMSSW_1_5_2_qcd_pt_2600_3000_fnal.cff --events 25000 --jobs 30
./createProject --project_name $1_20 --parameter_set makeFakeRateNtuple.cfg --dataset_cff CMSSW_1_5_2_qcd_pt_3000_3500_fnal.cff --events 25000 --jobs 10
#./createProject --project_name $1_21 --parameter_set makeFakeRateNtuple.cfg --dataset_cff CMSSW_1_5_2_qcd_pt_3500_inf_fnal.cff --events 25000 --jobs 

