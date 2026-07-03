set terminal pngcairo size 800,600 enhanced font 'Arial,14'
set output 'u_vs_N.png'
set title 'u vs N for x=0.25 and x=0.5'
set xlabel 'N (number of terms)'
set ylabel 'u'
set grid
set key left top
plot 'u_vs_N.dat' using 1:2 with lines lw 2 lc rgb '#d62728' title 'x=0.25', \
     'u_vs_N.dat' using 1:3 with lines lw 2 lc rgb '#1f77b4' title 'x=0.5'
