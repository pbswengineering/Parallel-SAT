set terminal png size 800,600 enhanced font "verdana" 14 truecolor
set size 1,1

set output "grafico-efficienza.png"

set title "Efficienza per numero di processori"

set key Right spacing 1.5
set key top

set grid linewidth 0.01

set xlabel "Processori"
set ylabel "Efficienza"

set xrange [2:8]
set yrange [0:1]
set xtics 2, 1, 10
set ytics 0, 0.1, 1

plot "efficienza.dat" using 1:2 notitle with linespoints linewidth 1 linetype 3 pointsize 1 pointtype 5
