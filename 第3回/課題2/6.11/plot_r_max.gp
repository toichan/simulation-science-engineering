set terminal pngcairo size 900,600
set output "r_max.png"

set xlabel "dt"
set ylabel "r_max"
set logscale x
set grid
set key top right

set xrange [0.001:0.1]
set samples 2

plot "r_max_data.txt" using 1:2 with lines lw 2 title "r_max", \
     1.0 with lines lw 2 dt 2 title "r_max = 1"
