set terminal png;
set output "../../output/frontier.png";
set title "Simulated Efficient Frontier";
set ylabel "Expected Return (%)";
set xlabel "Variance (%)";
set key left top;
plot "../../output/frontier.txt" using ($2*100):($1*100) \
        title "Portfolios" with points pointtype 5 pointsize 1, \
     "../../output/portfolios.txt" using ($2*100):($1*100):3 \
        title "Optimized Portfolios" with labels offset 0,1 \
        point pointtype 5 lc rgb "red";
set xrange [GPVAL_DATA_X_MIN:GPVAL_DATA_X_MAX+0.01];
replot;