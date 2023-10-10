#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> LoadBoardingPasses(const std::string& infile) {
  std::ifstream passes_file(infile);
  std::string pass;
  std::vector<std::string> passes;
  while (std::getline(passes_file, pass)) {
    passes.push_back(pass);
  }
  return passes;
}

int Reduce(const std::string& space, char left, char right) {
  int size = std::pow(2, space.size());
  std::pair<int, int> low_rng = {0, size / 2 - 1};
  std::pair<int, int> high_rng = {size / 2, size - 1};
  std::pair<int, int> curr_pos;
  for (const char& c : space) {
    if (c == left) {
      curr_pos = low_rng;
    } else if (c == right) {
      curr_pos = high_rng;
    }
    size = curr_pos.second - curr_pos.first + 1;
    low_rng = {curr_pos.first, curr_pos.first + (size / 2 - 1)};
    high_rng = {curr_pos.first + (size / 2), curr_pos.second};
  }
  return curr_pos.first;
}

int FindMaxSeatId(const std::vector<std::string>& passes) {
  const int kNumRows = 8;
  int max_id = 0;
  for (const std::string& pass : passes) {
    int row = Reduce(pass.substr(0, kNumRows - 1), 'F', 'B');
    int col = Reduce(pass.substr(kNumRows - 1), 'L', 'R');
    max_id = std::max(max_id, row * kNumRows + col);
  }
  return max_id;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::cout << "Answer: " << FindMaxSeatId(LoadBoardingPasses(argv[1]))
            << std::endl;

  exit(EXIT_SUCCESS);
}
