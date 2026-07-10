#!/bin/bash

# Compile the C program
gcc -o u_program src/Q5.c -lm

# Run the program and redirect output to data/u_values.dat
./u_program > data/u_values.dat

# Notify the user
echo "u_values.dat has been generated in the data directory."