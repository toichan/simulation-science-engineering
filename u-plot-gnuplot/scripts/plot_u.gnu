set terminal pngcairo
set output 'u_plot.png'
set title 'Plot of u(x, t, kappa, N)'
set xlabel 'N'
set ylabel 'u(x, t, kappa, N)'
set grid

plot 'data/u_values.dat' using 1:2 with linespoints title 'u(x, t, kappa, N)'