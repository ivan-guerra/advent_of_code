#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

bool ContainsNIdenticalLetters(const std::string &s, int n) {
  std::unordered_map<int, int> counts;
  for (const char &c : s) {
    counts[c]++;
  }

  for (const auto &[letter, count] : counts) {
    if (count == n) {
      return true;
    }
  }
  return false;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream input(argv[1]);
  std::string line;
  int num_twos = 0;
  int num_threes = 0;
  while (std::getline(input, line)) {
    if (ContainsNIdenticalLetters(line, 2)) {
      num_twos++;
    }
    if (ContainsNIdenticalLetters(line, 3)) {
      num_threes++;
    }
  }

  std::cout << "Answer: " << (num_twos * num_threes) << std::endl;

  exit(EXIT_SUCCESS);
}
