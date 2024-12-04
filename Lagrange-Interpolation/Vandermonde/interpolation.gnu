set term qt
set grid
set style line 1 lw 4 ps 2
set title 'Lagrange Cardinals'

l_0(x) = (1.0 / -8.38771e-07) * (x - 1.895) * (x - 1.914) * (x - 1.947) * (x - 1.96) * (x - 1.97)
l_1(x) = (1.0 / 1.25229e-07) * (x - 1.869) * (x - 1.914) * (x - 1.947) * (x - 1.96) * (x - 1.97)
l_2(x) = (1.0 / -7.26818e-08) * (x - 1.869) * (x - 1.895) * (x - 1.947) * (x - 1.96) * (x - 1.97)
l_3(x) = (1.0 / 4.00206e-08) * (x - 1.869) * (x - 1.895) * (x - 1.914) * (x - 1.96) * (x - 1.97)
l_4(x) = (1.0 / -3.53717e-08) * (x - 1.869) * (x - 1.895) * (x - 1.914) * (x - 1.947) * (x - 1.97)
l_5(x) = (1.0 / 9.7566e-08) * (x - 1.869) * (x - 1.895) * (x - 1.914) * (x - 1.947) * (x - 1.96)

p './data.dat' tit 'data', l_0(x), l_1(x), l_2(x), l_3(x), l_4(x), l_5(x)
pause -1
L(x) = 0.0 + 1877.49 * l_0(x) + 4044.91 * l_1(x) + 7903.66 * l_2(x) + 15893.8 * l_3(x) + 20013.8 * l_4(x) + 23364.4 * l_5(x) 
set auto xy
set title 'Interpolation Polynomial'

p './data.dat' tit 'data', L(x) tit 'polynomial', './interpolated_values.dat' tit 'interpolated'
pause -1
