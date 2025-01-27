#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>

long long LehmerCongruential(long long seed, long long multiplier, long long modulus) {
	return (multiplier * seed) % modulus;
}

std::pair<int, std::unordered_map<long long, int>> GenerateSequence(
		long long seed, long long multiplier, long long modulus, std::ofstream& sequence_file) {

	std::unordered_map<long long, int> seen;
	int iterations = 0;

	while (true) {
		long long next = LehmerCongruential(seed, multiplier, modulus);
		sequence_file << seed << "\t" << next << "\n";

		if (seen[next]++) break;  // Break if the value is repeated
		seed = next;
		++iterations;
	}

	return {iterations, seen};
}

std::pair<long long, int> FindMostFrequent(const std::unordered_map<long long, int>& occurrences) {
	long long most_frequent = 0;
	int max_frequency = 0;

	for (const auto& [num, freq] : occurrences) {
		if (freq > max_frequency) {
			most_frequent = num;
			max_frequency = freq;
		}
	}

	return {most_frequent, max_frequency};
}

void AnalyzeSeeds(long long start, long long end, long long multiplier, long long modulus,
		const std::string& summary_filename, const std::string& sequence_filename) {

	std::ofstream summary_file(summary_filename);
	std::ofstream sequence_file(sequence_filename);

	if (!summary_file.is_open() || !sequence_file.is_open()) {
		std::cerr << "Failed to open output files." << std::endl;
		return;
	}

	summary_file << "Seed\tIterations\tMost_Frequent\tFrequency\n";

	for (long long seed = start; seed <= end; ++seed) {
		auto [iterations, occurrences] = GenerateSequence(seed, multiplier, modulus, sequence_file);
		auto [most_frequent, max_frequency] = FindMostFrequent(occurrences);

		summary_file << seed << "\t" << iterations
			<< "\t" << most_frequent << "\t" << max_frequency << "\n";
	}
}

int main() {
	const long long start = 1, end = 100;  // Example range of seeds
	const long long multiplier = 48271;   // Common Lehmer multiplier (e.g., 48271)
	const long long modulus = 2147483647; // Common modulus (e.g., 2^31 - 1)
	const std::string summary_filename = "summary.tsv", sequence_filename = "sequence.tsv";

	AnalyzeSeeds(start, end, multiplier, modulus, summary_filename, sequence_filename);

	std::cout << "Results saved to " << summary_filename << " and " << sequence_filename << std::endl;
	return 0;
}

