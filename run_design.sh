#!/bin/bash

# chmod +x run_design.sh

# Create the output directory if it doesn't exist
mkdir -p output

# Compile all design*.cpp files, outputting them to the output directory with .out extension
for f in design*.cpp; do 
    clang++ -O2 -std=c++20 "$f" -o "output/${f%.cpp}.out"; 
done

# Run all executables in the output directory
for f in output/*.out; do 
    if [[ -x "$f" ]]; then 
        "$f"; 
    fi 
done