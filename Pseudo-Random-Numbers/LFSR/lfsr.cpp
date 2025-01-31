#include <iostream>
#include <fstream>
#include <bitset>

using namespace std;

// Function to generate the next number using LFSR (16-bit)
uint16_t lfsr(uint16_t lfsr_reg) {
	// LFSR polynomial: x^16 + x^14 + x^13 + x$^11 + 1
	uint16_t bit = ((lfsr_reg >> 0) ^ (lfsr_reg >> 2) ^ (lfsr_reg >> 3) ^ (lfsr_reg >> 5)) & 1;
	return (lfsr_reg >> 1) | (bit << 15);
}

int main() {
	uint16_t seed = 0xACE1u; // Initial seed value
	uint16_t current_num = seed;

	ofstream out_file("lfsr_output.csv");
	out_file << "#x\ty\n"; // Column headers: current number, next number

	for (int i = 0; i < 1000000; ++i) {
		uint16_t next_num = lfsr(current_num);
		out_file << current_num << "\t" << next_num << "\n";
		current_num = next_num;
	}

	out_file.close();
	cout << "Generated 10,000 numbers and saved to 'lfsr_output.csv'." << endl;

	return 0;
}

