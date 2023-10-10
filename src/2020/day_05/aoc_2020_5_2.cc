#include <algorithm>
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

std::vector<int> GetSeatIds(const std::vector<std::string>& passes) {
  const int kNumRows = 8;
  std::vector<int> seat_ids;
  for (const std::string& pass : passes) {
    int row = Reduce(pass.substr(0, kNumRows - 1), 'F', 'B');
    int col = Reduce(pass.substr(kNumRows - 1), 'L', 'R');
    seat_ids.push_back(row * kNumRows + col);
  }
  return seat_ids;
}

int FindMySeatId(std::vector<int>& seat_ids) {
  std::sort(std::begin(seat_ids), std::end(seat_ids));
  for (int i = 1; i < seat_ids.size(); ++i) {
    if ((seat_ids[i] - seat_ids[i - 1]) != 1) {
      return (seat_ids[i] - 1);
    }
  }
  return -1;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::vector<int> seat_ids = GetSeatIds(LoadBoardingPasses(argv[1]));
  std::cout << "Answer: " << FindMySeatId(seat_ids) << std::endl;

  exit(EXIT_SUCCESS);
}
