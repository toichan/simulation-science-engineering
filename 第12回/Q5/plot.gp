# gnuplot script to plot u(x) from u_values.dat
set terminal pngcairo size 800,600 enhanced font 'Arial,14'
set output 'u_plot.png'
set title 'u(x) at t=0'
set xlabel 'x'
set ylabel 'u(x)'
set grid
set key off

# skip comment lines starting with #
plot 'u_values.dat' using 1:2 with lines lw 2 lc rgb '#1f77b4'

# EOF
