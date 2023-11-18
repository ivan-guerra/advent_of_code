#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

[[nodiscard]] int GetNumberOfDepthIncreases(
    const std::vector<int>& depths) noexcept {
  int num_increases = 0;
  for (int i = 1; i < depths.size(); ++i) {
    if (depths[i] > depths[i - 1]) {
      num_increases++;
    }
  }
  return num_increases;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::vector<int> depths;
  std::string line;
  std::ifstream fhandle(argv[1]);
  while (std::getline(fhandle, line)) {
    depths.push_back(std::stoi(line));
  }

  std::cout << "Answer: " << GetNumberOfDepthIncreases(depths) << std::endl;

  exit(EXIT_SUCCESS);
}
