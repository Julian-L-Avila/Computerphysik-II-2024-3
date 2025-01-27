#include <iostream>
#include <fstream>
#include <unordered_set>
#include <cstdint> // For uint64_t

using namespace std;

// Function to generate pseudo-random numbers using Linear Congruential Generator
unsigned int linear_congruential_generator(unsigned int seed, unsigned int a, unsigned int c, uint64_t m) {
    return (a * seed + c) % m;
}

// Function to check when the sequence starts repeating
int find_degeneration_point(unsigned int seed, unsigned int a, unsigned int c, uint64_t m) {
    unordered_set<unsigned int> seen_numbers;
    unsigned int current = seed;
    int iterations = 0;

    while (seen_numbers.find(current) == seen_numbers.end()) {
        seen_numbers.insert(current);
        current = linear_congruential_generator(current, a, c, m);
        iterations++;
    }
    return iterations;
}

int main() {
    // Parameters for LCG
    unsigned int seed = 12345;  // Initial seed value
    unsigned int a = 1664525;   // Multiplier
    unsigned int c = 1013904223; // Increment
    uint64_t m = 4294967296; // Modulus (2^32)

    // Output files
    ofstream data_file("sequence.tsv");
    ofstream summary_file("iterations_summary.txt");

    if (!data_file.is_open() || !summary_file.is_open()) {
        cerr << "Error opening output files." << endl;
        return 1;
    }

    // Writing header for the data file
    data_file << "#Previous\tGenerated" << endl;

    // Generate the pseudo-random numbers and write to file
    unsigned int current = seed;
    int iterations = 0;
    while (iterations < 10000) {  // Set a reasonable number of iterations (e.g., 10,000)
        unsigned int next = linear_congruential_generator(current, a, c, m);
        data_file << current << "\t" << next << endl;
        current = next;
        iterations++;
    }

    // Find the degeneration point
    int degeneration_iterations = find_degeneration_point(seed, a, c, m);

    // Writing summary to file
    summary_file << "Iterations before degeneration: " << degeneration_iterations << endl;

    // Close the files
    data_file.close();
    summary_file.close();

    cout << "Files generated: random_data.csv and iterations_summary.txt" << endl;

    return 0;
}

