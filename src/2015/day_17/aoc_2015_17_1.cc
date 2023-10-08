#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

std::vector<std::vector<int>> NChooseK(const std::vector<int>& values, int k) {
  std::vector<int> aux(values.size(), 1);
  std::fill(aux.begin(), aux.begin() + k, 0);

  std::vector<std::vector<int>> combinations;
  do {
    std::vector<int> combo;
    for (int i = 0; i < aux.size(); ++i) {
      if (aux[i]) {
        combo.push_back(values[i]);
      }
    }
    combinations.push_back(combo);
  } while (std::next_permutation(aux.begin(), aux.end()));
  return combinations;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream input(argv[1]);
  std::string container;
  std::vector<int> containers;
  while (std::getline(input, container)) {
    containers.push_back(std::stoi(container));
  }

  const int kTargetVolume = 150;
  int result = 0;
  for (int i = 0; i < containers.size(); ++i) {
    auto combinations = NChooseK(containers, i);
    for (const std::vector<int>& combo : combinations) {
      int total_volume = std::accumulate(combo.begin(), combo.end(), 0);
      if (total_volume == kTargetVolume) {
        result++;
      }
    }
  }

  std::cout << "Answer: " << result << std::endl;

  exit(EXIT_SUCCESS);
}
