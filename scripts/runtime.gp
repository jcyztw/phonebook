reset
set ylabel 'time(sec)'
set style fill solid
set title 'performance comparison'
set term png enhanced font 'Verdana,10'
set output 'runtime.png'

plot [:][:0.150]'output.txt' using 2:xtic(1) with histogram title 'original', \
'' using 3:xtic(1) with histogram title 'resized struc.'  , \
'' using 4:xtic(1) with histogram title 'hash', \
'' using 5:xtic(1) with histogram title 'mpool', \
'' using ($0+0.5):(0.065):2 with labels title ' ' textcolor lt 1, \
'' using ($0+0.5):(0.060):3 with labels title ' ' textcolor lt 2, \
'' using ($0+0.5):(0.055):4 with labels title ' ' textcolor lt 3, \
'' using ($0+0.5):(0.050):5 with labels title ' ' textcolor lt 4
