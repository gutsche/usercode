#!/bin/bash

root -l -q -b process_QCD_Pt_470_600.C
root -l -q -b process_QCD_Pt_600_800.C
root -l -q -b process_QCD_Pt_800_1000.C
root -l -q -b process_QCD_Pt_1000_1400.C
root -l -q -b process_QCD_Pt_1400_1800.C
root -l -q -b process_QCD_Pt_1800_2200.C
root -l -q -b process_QCD_Pt_2200_2600.C
root -l -q -b process_QCD_Pt_2600_3000.C
# root -l -q -b process_QCD_Pt_3000_3500.C
root -l -q -b process_QCD_Pt_3500_inf.C
