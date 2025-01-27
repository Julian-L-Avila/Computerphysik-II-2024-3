#include <iostream>
#include <fstream>
#include <unordered_set>
#include <cstdint>

unsigned int linear_congruential_generator(unsigned int seed, unsigned int a, unsigned int c, uint64_t m) {
  return (a * seed + c) % m;
}

int find_degeneration_point(unsigned int seed, unsigned int a, unsigned int c, uint64_t m) {
  std::unordered_set<unsigned int> seen_numbers;
  unsigned int current = seed;

  while (seen_numbers.insert(current).second) {
    current = linear_congruential_generator(current, a, c, m);
  }
  return seen_numbers.size();
}

int main() {
  unsigned int seed = 12345;
  unsigned int a = 1664525;
  unsigned int c = 1013904223;
  uint64_t m = 4294967296;

  std::ofstream data_file("sequence.tsv");
  std::ofstream summary_file("iterations_summary.txt");

  if (!data_file.is_open() || !summary_file.is_open()) return 1;

  data_file << "#Previous\tGenerated" << std::endl;

  unsigned int current = seed;
  for (int i = 0; i < 10000; ++i) {
    unsigned int next = linear_congruential_generator(current, a, c, m);
    data_file << current << "\t" << next << std::endl;
    current = next;
  }

  int degeneration_iterations = find_degeneration_point(seed, a, c, m);
  summary_file << "Iterations before degeneration: " << degeneration_iterations << std::endl;

  return 0;
}

