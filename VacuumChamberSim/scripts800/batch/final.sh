#!/bin/bash

# Define input file pattern and output file name
input_pattern="RAONSCL2_VacuumChamberSim_Run*.root"
output_file="final_Vacuum_800.root"

# Combine files using hadd
hadd $output_file $input_pattern

