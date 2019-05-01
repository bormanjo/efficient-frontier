set terminal png
set output "../../output/frontier.png"
set title "Simulated Efficient Frontier"
set ylabel "Expected Return"
set xlabel "Variance"
set key left top
plot "../../output/frontier.txt" using 2:1 title "Portfolios" with points pointtype 5 pointsize 1, \
     "../../output/portfolios.txt" using 2:1:3 title "Optimized Portfolios" with labels offset 5.5,0 point pointtype 5 lc rgb "red"
