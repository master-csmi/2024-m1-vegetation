#!/bin/bash

# Directory containing the raw STL files
INPUT_DIR="tree_ref"

# Get the alpha value from the command line or set a default value
if [ -z "$1" ]; then
	ALPHA=0.1
else
	ALPHA=$1
fi

# Loop through all STL files in the input directory
for input_file in $INPUT_DIR/*.stl; do
	./build/simplify "$input_file" "$ALPHA"
done
