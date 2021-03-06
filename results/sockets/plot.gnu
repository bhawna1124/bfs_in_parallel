

set terminal png nocrop enhanced font "/usr/share/fonts/truetype/msttcorefonts/arial.ttf" 11 size 620,420 
set output 'sockets.png'

set datafile missing '-'
set style data linespoints
set title "CPU Optimized by Inner-sockects with Changing Thread Number"

set xrange [4:12]
set yrange [0:1200]

set xlabel "Thread Number"
set ylabel "Elapsed Time (ms)"

set xtics auto 1 nomirror
set ytics auto 200 nomirror

set mxtics 1
set mytics 2


plot 'sockets.dat' using 1:2 t "RGL1", '' using 1:3 t "IRGL1", '' using 1:4 t "BAY", '' using 1:5 t "SW1" 


