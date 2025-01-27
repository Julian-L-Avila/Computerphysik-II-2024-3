#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>

std::string MiddleProduct(std::string num1, std::string num2, int width) {
  auto product = std::stoull(num1) * std::stoull(num2);
  auto product_str = std::to_string(product);
  int len = product_str.length();
  int start = (len - width) / 2;
  return product_str.substr(start, width);
}

std::pair<int, std::unordered_map<std::string, int>> GenerateSequence(
    std::string seed1, std::string seed2, int width,
    std::ofstream& sequence_file) {

  std::unordered_map<std::string, int> seen;
  int iterations = 0;

  while (true) {
    auto next = MiddleProduct(seed1, seed2, width);
    sequence_file << seed1 << "\t" << seed2 << "\t" << next << "\n";

    if (seen[next]++) break;
    if (next == std::string(width, '0')) break;

    std::swap(seed1, seed2);
    seed2 = next;
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

  summary_file << "Seed1\tSeed2\tIterations\tMost_Frequent\tFrequency\n";

  for (int seed1 = start; seed1 <= end; seed1 += 100) {
    for (int seed2 = start; seed2 <= end; seed2 += 100) {
      std::string s1 = std::to_string(seed1), s2 = std::to_string(seed2);

      s1.insert(s1.begin(), width - s1.length(), '0');
      s2.insert(s2.begin(), width - s2.length(), '0');

      auto [iterations, occurrences] = GenerateSequence(s1, s2, width, sequence_file);
      auto [most_frequent, max_frequency] = FindMostFrequent(occurrences);

      summary_file << seed1 << "\t" << seed2 << "\t" << iterations
        << "\t" << most_frequent << "\t" << max_frequency << "\n";
    }
  }
}

int main() {
  const int start = 1111, end = 5555, width = 4;
  const std::string summary_filename = "summary.tsv", sequence_filename = "sequence.tsv";

  AnalyzeSeeds(start, end, width, summary_filename, sequence_filename);

  std::cout << "Results saved to " << summary_filename << " and " << sequence_filename << std::endl;
  return 0;
}

