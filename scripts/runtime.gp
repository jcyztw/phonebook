reset
set ylabel 'time(sec)'
set style fill solid
set title 'performance comparison'
set term png enhanced font 'Verdana,10'
set output 'runtime.png'

plot [:][:0.150]'output.txt' using 2:xtic(1) with histogram title 'original', \
'' using ($0-0.06):($2+0.001):2 with labels title ' ', \
'' using 3:xtic(1) with histogram title 'optimized 1'  , \
'' using ($0+0.15):($3+0.0015):3 with labels title ' ', \
'' using 4:xtic(1) with histogram title 'opt.2: hash', \
'' using ($0+0.4):($4+0.0013):4 with labels title ' '
