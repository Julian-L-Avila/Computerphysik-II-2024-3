#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>

std::string MiddleSquare(std::string seed, int width) {
  auto square = std::stoull(seed) * std::stoull(seed);
  auto square_str = std::to_string(square);
  int len = square_str.length();

  // Ensure the string is padded with leading zeros if necessary
  square_str.insert(square_str.begin(), 2 * width - len, '0');

  int start = (square_str.length() - width) / 2;
  return square_str.substr(start, width);
}

std::pair<int, std::unordered_map<std::string, int>> GenerateSequence(
    std::string seed, int width, std::ofstream& sequence_file) {

  std::unordered_map<std::string, int> seen;
  int iterations = 0;

  while (true) {
    auto next = MiddleSquare(seed, width);
    sequence_file << seed << "\t" << next << "\n";

    if (seen[next]++) break;
    if (next == std::string(width, '0')) break;

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

void AnalyzeSeeds(int start, int end, int width,
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

    auto [iterations, occurrences] = GenerateSequence(s, width, sequence_file);
    auto [most_frequent, max_frequency] = FindMostFrequent(occurrences);

    summary_file << seed << "\t" << iterations
      << "\t" << most_frequent << "\t" << max_frequency << "\n";
  }
}

int main() {
  const int start = 1111, end = 9999, width = 4;
  const std::string summary_filename = "summary.tsv", sequence_filename = "sequence.tsv";

  AnalyzeSeeds(start, end, width, summary_filename, sequence_filename);

  std::cout << "Results saved to " << summary_filename << " and " << sequence_filename << std::endl;
  return 0;
}

