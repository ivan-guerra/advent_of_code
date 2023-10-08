#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

int FindFirstDupeFreq(const std::vector<int> &deltas) {
  std::unordered_set<int> seen_freqs;

  int freq = 0;
  while (true) {
    for (const int &delta : deltas) {
      freq += delta;
      if (seen_freqs.count(freq)) {
        return freq;
      }
      seen_freqs.insert(freq);
    }
  }
  return -1; /* unreachable */
}

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream input(argv[1]);
  std::string line;
  std::vector<int> deltas;
  while (std::getline(input, line)) {
    deltas.push_back(std::stoi(line));
  }

  std::cout << "Answer: " << FindFirstDupeFreq(deltas) << std::endl;

  exit(EXIT_SUCCESS);
}
