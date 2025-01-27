#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>

std::string MiddleProductWithScaling(std::string seed, int scaling_factor, int width) {
	auto product = std::stoull(seed) * scaling_factor;
	auto product_str = std::to_string(product);
	int len = product_str.length();

	// Ensure the string is padded with leading zeros if necessary
	product_str.insert(product_str.begin(), 2 * width - len, '0');

	int start = (product_str.length() - width) / 2;
	return product_str.substr(start, width);
}

std::pair<int, std::unordered_map<std::string, int>> GenerateSequence(
		std::string seed, int scaling_factor, int width, std::ofstream& sequence_file) {

	std::unordered_map<std::string, int> seen;
	int iterations = 0;

	while (true) {
		auto next = MiddleProductWithScaling(seed, scaling_factor, width);
		sequence_file << seed << "\t" << next << "\n";

		if (seen[next]++) break;  // Break if the value is repeated
		if (next == std::string(width, '0')) break;  // Break if all zeros

		seed = next;
		++iterations;
	}

	return {iterations, seen};
}

std::pair<std::string, int> FindMostFrequent(const std::unordered_map<std::string, int>& occurrences) {
	std::string most_frequent;
	int max_frequency = 0;

	for (const auto& [num, freq] : occurrences) {
		if (freq > max_frequency) {
			most_frequent = num;
			max_frequency = freq;
		}
	}

	return {most_frequent, max_frequency};
}

void AnalyzeSeeds(int start, int end, int scaling_factor, int width,
		const std::string& summary_filename,
		const std::string& sequence_filename) {

	std::ofstream summary_file(summary_filename);
	std::ofstream sequence_file(sequence_filename);

	if (!summary_file.is_open() || !sequence_file.is_open()) {
		std::cerr << "Failed to open output files." << std::endl;
		return;
	}

	summary_file << "Seed\tIterations\tMost_Frequent\tFrequency\n";

	for (int seed = start; seed <= end; ++seed) {
		std::string s = std::to_string(seed);

		// Pad the seed to match the required width
		s.insert(s.begin(), width - s.length(), '0');

		auto [iterations, occurrences] = GenerateSequence(s, scaling_factor, width, sequence_file);
		auto [most_frequent, max_frequency] = FindMostFrequent(occurrences);

		summary_file << seed << "\t" << iterations
			<< "\t" << most_frequent << "\t" << max_frequency << "\n";
	}
}

int main() {
	const int start = 1000, end = 9999, width = 4;
	const int scaling_factor = 1234;  // Example scaling factor
	const std::string summary_filename = "summary.tsv", sequence_filename = "sequence.tsv";

	AnalyzeSeeds(start, end, scaling_factor, width, summary_filename, sequence_filename);

	std::cout << "Results saved to " << summary_filename << " and " << sequence_filename << std::endl;
	return 0;
}

