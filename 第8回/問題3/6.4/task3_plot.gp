set terminal pngcairo size 1200,800
set datafile commentschars "#"

set grid
set key outside

set output 'task3_problem1_orbit.png'
set title 'Problem 1 Orbit'
set xlabel 'x'
set ylabel 'y'
plot \
    'task3_fixed_data_problem1.txt' using 3:4 with lines title 'fixed e1', \
    'task3_fixed_data_problem1.txt' using 5:6 with lines title 'fixed e2', \
    'task3_adaptive_data_problem1.txt' using 3:4 with lines title 'adaptive e1', \
    'task3_adaptive_data_problem1.txt' using 5:6 with lines title 'adaptive e2'

set output 'task3_problem1_energy.png'
set title 'Problem 1 Energy'
set xlabel 't'
set ylabel 'E'
plot \
    'task3_fixed_data_problem1.txt' using 1:7 with lines title 'fixed', \
    'task3_adaptive_data_problem1.txt' using 1:7 with lines title 'adaptive'

set output 'task3_problem1_dt.png'
set title 'Problem 1 Adaptive dt'
set xlabel 't'
set ylabel 'dt'
plot 'task3_adaptive_data_problem1.txt' using 1:2 with lines title 'adaptive dt'

set output 'task3_problem2_orbit.png'
set title 'Problem 2 Orbit'
set xlabel 'x'
set ylabel 'y'
plot \
    'task3_fixed_data_problem2.txt' using 3:4 with lines title 'fixed e1', \
    'task3_fixed_data_problem2.txt' using 5:6 with lines title 'fixed e2', \
    'task3_adaptive_data_problem2.txt' using 3:4 with lines title 'adaptive e1', \
    'task3_adaptive_data_problem2.txt' using 5:6 with lines title 'adaptive e2'

set output 'task3_problem2_energy.png'
set title 'Problem 2 Energy'
set xlabel 't'
set ylabel 'E'
plot \
    'task3_fixed_data_problem2.txt' using 1:7 with lines title 'fixed', \
    'task3_adaptive_data_problem2.txt' using 1:7 with lines title 'adaptive'

set output 'task3_problem2_dt.png'
set title 'Problem 2 Adaptive dt'
set xlabel 't'
set ylabel 'dt'
plot 'task3_adaptive_data_problem2.txt' using 1:2 with lines title 'adaptive dt'