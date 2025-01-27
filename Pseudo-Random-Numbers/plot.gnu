# Set output as a PDF file with LaTeX font and size in centimeters
set terminal cairolatex standalone size 25cm, 17.5cm

# Set the LaTeX preamble for standalone class
set output 'scatter_plot.tex'

# Set grid and labels for better visualization
set auto xy
set xlabel '\(X_1\)'
set ylabel '\(X_2\)'
set title 'Lehmer Congruential'
unset key

# Load the data from the TSV file
# Assumes the data has two columns: X and Y
plot './Lehmer-Method/sequence.tsv' using 1:2 with points pt 7 ps 0.5

# End of the script

