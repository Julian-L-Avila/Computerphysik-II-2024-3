set term qt
set grid
set style line 1 lw 4 ps 2
set title 'Lagrange Cardinals'

l_0(x) = (1.0 / -1442.39) * (x - 2.3) * (x - 3.15) * (x - 4.85) * (x - 6.25) * (x - 7.95)
l_1(x) = (1.0 / 104.002) * (x - 0.15) * (x - 3.15) * (x - 4.85) * (x - 6.25) * (x - 7.95)
l_2(x) = (1.0 / -64.5048) * (x - 0.15) * (x - 2.3) * (x - 4.85) * (x - 6.25) * (x - 7.95)
l_3(x) = (1.0 / 88.4253) * (x - 0.15) * (x - 2.3) * (x - 3.15) * (x - 6.25) * (x - 7.95)
l_4(x) = (1.0 / -177.773) * (x - 0.15) * (x - 2.3) * (x - 3.15) * (x - 4.85) * (x - 7.95)
l_5(x) = (1.0 / 1114.79) * (x - 0.15) * (x - 2.3) * (x - 3.15) * (x - 4.85) * (x - 6.25)

p './data.dat' tit 'data', l_0(x), l_1(x), l_2(x), l_3(x), l_4(x), l_5(x)
pause -1
L(x) = 0.0 + 4.79867 * l_0(x) + 4.49013 * l_1(x) + 4.2243 * l_2(x) + 3.47313 * l_3(x) + 2.66674 * l_4(x) + 1.51909 * l_5(x) 
set auto xy
set title 'Interpolation Polynomial'

p './data.dat' tit 'data', L(x) tit 'polynomial', './interpolated_values.dat' tit 'interpolated'
pause -1
