#!/bin/bash

# Directory containing the raw STL files
INPUT_DIR="tree_ref/raw_tree"

# The alpha value to use for wrapping
ALPHA=100

# Loop through all STL files in the input directory
for input_file in $INPUT_DIR/*.stl; do
	./build/wrap "$input_file" "$ALPHA"
done
