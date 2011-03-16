set terminal png size 800,600 enhanced font "verdana" 16 truecolor
set size 1,1

set output "grafico-speed-up.png"

set title "Speed-up per numero di processori"
set key Right spacing 1.5
set key top

set grid linewidth 0.01

set xlabel "Processori"
set ylabel "Speed-up"

set xrange [2:8]
set yrange [0:3]
set xtics 2, 1, 10
set ytics 0, 0.25, 3

plot "speed-up.dat" using 1:2 notitle with linespoints linewidth 1 linetype 2 pointsize 1 pointtype 5
