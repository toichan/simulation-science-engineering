set terminal pngcairo size 800,800
set xlabel "x"
set ylabel "y"
set xrange [-5:5]
set yrange [-5:5]
set size ratio -1
set grid
unset key

$stars << EOD
-1 0
1 0
EOD

if (!exists("dirs")) dirs = "horizontal vertical diagonal"

do for [j=1:words(dirs)] {
    dir = word(dirs, j)

    do for [i=0:11] {
        v = 0.25 * (i + 1)

        set output sprintf("orbit_%s_%d.png", dir, i)
        set title sprintf("%s v=%.2f", dir, v)

           plot sprintf("orbit_%s_%d.dat", dir, i) using 1:2 with lines lw 2, \
               $stars using 1:2 with points pt 7 ps 2.0 lc rgb "red"
    }
}
