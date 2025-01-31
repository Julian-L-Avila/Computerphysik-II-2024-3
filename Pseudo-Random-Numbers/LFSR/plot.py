import pandas as pd
import matplotlib.pyplot as plt

# Load the data
df = pd.read_csv('./lfsr_output.csv', header=None, sep='\t')  # Adjust delimiter if needed

# Extract the second column
second_column = df[1]  # Index 1 for the second column

# Plot the histogram
plt.hist(second_column, bins=50, edgecolor='black')  # Adjust the number of bins as needed
plt.xlabel('n')
plt.ylabel('Frequency')
plt.title('LFSR')

# Save the plot as a PDF
plt.savefig('histogram_plot.pdf', format='pdf')

