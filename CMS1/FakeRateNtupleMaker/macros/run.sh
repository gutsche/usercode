#!/bin/bash
root -b -q -l process_QCD_Pt_15_20.C >& process_QCD_Pt_15_20.log &
root -b -q -l process_QCD_Pt_20_30.C >& process_QCD_Pt_20_30.log &
root -b -q -l process_QCD_Pt_120_170.C >& process_QCD_Pt_120_170.log &
root -b -q -l process_QCD_Pt_300_380.C >& process_QCD_Pt_300_380.log &
root -b -q -l process_QCD_Pt_3000_3500.C >& process_QCD_Pt_3000_3500.log &
