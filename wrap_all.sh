#!/bin/bash

# Directory containing the raw STL files
INPUT_DIR="tree_ref/raw_tree"

# Scaling factor
SCALING_FACTOR=100

# Output directory for the wrapped STL files
OUTPUT_DIR="tree_ref/"

# Loop through all STL files in the input directory
for input_file in $INPUT_DIR/*.stl; do
	# Extract the filename without the extension

	# Define the output filename

	# Run the wrap program
	./build/wrap "$input_file" "$SCALING_FACTOR"

done
