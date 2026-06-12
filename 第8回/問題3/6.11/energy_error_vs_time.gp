set terminal pngcairo size 1400,800 enhanced font ',16'
set xlabel 't'
set ylabel 'energy error'
set grid
set key outside

set output 'problem1_energy_error.png'
set title 'Problem 1: energy error vs time'
plot \
    'problem1_fixed.dat' using 1:8 with lines lw 2 lc rgb '#1f77b4' title 'Fixed', \
    'problem1_adaptive.dat' using 1:8 with lines lw 2 lc rgb '#d62728' title 'Adaptive'

set output 'problem2_energy_error.png'
set title 'Problem 2: energy error vs time'
plot \
    'problem2_fixed.dat' using 1:8 with lines lw 2 lc rgb '#1f77b4' title 'Fixed', \
    'problem2_adaptive.dat' using 1:8 with lines lw 2 lc rgb '#d62728' title 'Adaptive'

unset output
