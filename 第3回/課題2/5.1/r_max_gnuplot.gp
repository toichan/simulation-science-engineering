set xlabel "dt"
set ylabel "r_max"
set logscale x
plot "r_max_data.txt" using 1:2 with lines title "r max"