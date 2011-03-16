set terminal png size 800,600 enhanced font "verdana" 16 truecolor

set size 1,1

set output "grafico-tempi.png"

set title "Tempo di esecuzione per numero di processori"

set key Right spacing 1.5
set key top

set grid linewidth 0.01

set xlabel "Processori"
set ylabel "Tempo (in centesimi di secondo)"

set xrange [1:8]
set yrange [0:200]
set xtics 0, 1, 10
set ytics 0, 20, 200

plot "tempi.dat" using 1:2 notitle with linespoints linewidth 1 linetype 1 pointsize 1 pointtype 5
