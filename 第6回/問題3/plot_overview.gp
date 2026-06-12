set terminal pngcairo size 1600,1200
set xrange [-5:5]
set yrange [-5:5]
set size ratio -1
unset key
set grid

$stars << EOD
-1 0
1 0
EOD

if (!exists("dir")) dir = "horizontal"

set output sprintf("overview_%s.png", dir)
set multiplot layout 3,4 rowsfirst title sprintf("%s overview", dir)

do for [i=0:11] {
    v = 0.25 * (i + 1)
    set title sprintf("v=%.2f", v)
    plot sprintf("orbit_%s_%d.dat", dir, i) using 1:2 with lines lw 1, \
         $stars using 1:2 with points pt 7 ps 1.5 lc rgb "red"
}

unset multiplot
