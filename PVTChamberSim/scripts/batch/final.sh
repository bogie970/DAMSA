#!/bin/bash

# Define input file pattern and output file name
input_pattern="dms-fullsim-out*.root"
output_file="None_None_Pln_gamma_100MeV_100cm.root"

# Combine files using hadd
hadd $output_file $input_pattern

# Check if output file exists
if [ -f $output_file ]; then
    echo "Output file $output_file created successfully."
    
    # Delete the input files
    rm $input_pattern
    echo "Input files deleted."
else
    echo "Error: Output file $output_file not created."
fi

