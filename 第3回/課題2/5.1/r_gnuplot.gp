set xlabel "t"
set ylabel "r"
plot "r_data.txt" using 1:2 with lines title "r", \
	"r_data.txt" using 1:3 with lines title "r max"