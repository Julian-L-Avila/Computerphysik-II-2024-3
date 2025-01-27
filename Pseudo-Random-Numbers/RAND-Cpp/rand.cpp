#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

int main() {
	std::srand(std::time(nullptr));  // Initialize the random seed

	unsigned int seed = std::rand();
	std::ofstream data_file("sequence.tsv");

	if (!data_file.is_open()) return 1;

	data_file << "#Previous\tGenerated" << std::endl;

	unsigned int current = seed;
	for (int i = 0; i < 10000; ++i) {
		unsigned int next = std::rand();
		data_file << current << "\t" << next << std::endl;
		current = next;
	}

	return 0;
}

