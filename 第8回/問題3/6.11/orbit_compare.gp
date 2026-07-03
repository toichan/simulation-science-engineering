set terminal pngcairo size 1200,1200 enhanced font ',16'
set size ratio -1
set xrange [-5:5]
set yrange [-5:5]
set xlabel 'x'
set ylabel 'y'
set grid
set key outside

set xrange [-7:7]
set yrange [-7:7]
set output 'problem1_orbit_compare.png'
set title 'Problem 1: Fixed vs Adaptive Orbit'
plot \
    'problem1_fixed.dat' using 2:3 with lines lw 2 lc rgb '#1f77b4' title 'Fixed e1', \
    'problem1_fixed.dat' using 4:5 with lines lw 2 lc rgb '#ff7f0e' title 'Fixed e2', \
    'problem1_adaptive.dat' using 2:3 with lines lw 2 dt 2 lc rgb '#2ca02c' title 'Adaptive e1', \
    'problem1_adaptive.dat' using 4:5 with lines lw 2 dt 2 lc rgb '#d62728' title 'Adaptive e2'

set xrange [-3:3]
set yrange [-3:3]
set output 'problem2_orbit_compare.png'
set title 'Problem 2: Fixed vs Adaptive Orbit'
plot \
    'problem2_fixed.dat' using 2:3 with lines lw 2 lc rgb '#1f77b4' title 'Fixed e1', \
    'problem2_fixed.dat' using 4:5 with lines lw 2 lc rgb '#ff7f0e' title 'Fixed e2', \
    'problem2_adaptive.dat' using 2:3 with lines lw 2 dt 2 lc rgb '#2ca02c' title 'Adaptive e1', \
    'problem2_adaptive.dat' using 4:5 with lines lw 2 dt 2 lc rgb '#d62728' title 'Adaptive e2'

unset output
