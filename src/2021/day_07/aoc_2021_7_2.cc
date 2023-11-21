#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using CrabPosVec = std::vector<int>;

[[nodiscard]] CrabPosVec ParsePositions(const std::string& filename) {
  std::ifstream fhandle(filename);
  std::string line;
  std::getline(fhandle, line);

  std::stringstream ss(line);
  std::string pos;
  CrabPosVec positions;
  while (std::getline(ss, pos, ',')) {
    positions.push_back(std::stoi(pos));
  }
  return positions;
}

[[nodiscard]] int MoveCost(const CrabPosVec& positions, int target_pos) {
  int total_cost = 0;
  for (const int& pos : positions) {
    int steps = std::abs(pos - target_pos);
    total_cost += (steps * (steps + 1)) / 2;
  }
  return total_cost;
}

[[nodiscard]] int FindMinCostPosition(const CrabPosVec& positions) {
  int min_pos = *std::min_element(positions.cbegin(), positions.cend());
  int max_pos = *std::max_element(positions.cbegin(), positions.cend());
  int min_cost = MoveCost(positions, positions[0]);
  for (int i = min_pos; i <= max_pos; ++i) {
    min_cost = std::min(min_cost, MoveCost(positions, i));
  }
  return min_cost;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  CrabPosVec positions = ParsePositions(argv[1]);
  std::cout << "Answer: " << FindMinCostPosition(positions) << std::endl;

  exit(EXIT_SUCCESS);
}
