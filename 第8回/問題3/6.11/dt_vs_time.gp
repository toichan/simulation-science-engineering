set terminal pngcairo size 1400,800 enhanced font ',16'
set xlabel 't'
set ylabel 'dt'
set grid
set key off

set output 'dt_vs_time_problem1.png'
set title 'Problem 1: dt vs time'
plot 'problem1_adaptive.dat' using 1:6 with lines lw 2 lc rgb '#1f77b4'

set output 'dt_vs_time_problem2.png'
set title 'Problem 2: dt vs time'
plot 'problem2_adaptive.dat' using 1:6 with lines lw 2 lc rgb '#d62728'

unset output
