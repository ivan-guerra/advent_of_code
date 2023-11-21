#include <cstdlib>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

using SmokeMap = std::vector<std::vector<int>>;
using RiskVec = std::vector<int>;

[[nodiscard]] SmokeMap ParseSmokeMap(const std::string& filename) {
  std::ifstream fhandle(filename);
  std::string line;
  SmokeMap smoke_map;
  while (std::getline(fhandle, line)) {
    std::vector<int> row;
    for (const char& c : line) {
      row.push_back(static_cast<int>(c - '0'));
    }
    smoke_map.push_back(row);
  }
  return smoke_map;
}

[[nodiscard]] RiskVec FindRiskLevels(const SmokeMap smoke_map) noexcept {
  static const std::vector<std::vector<int>> kDirections = {
      {0, 1},
      {1, 0},
      {-1, 0},
      {0, -1},
  };

  RiskVec risk_levels;
  for (int i = 0; i < smoke_map.size(); ++i) {
    for (int j = 0; j < smoke_map[i].size(); ++j) {
      bool is_low_point = true;
      for (const auto& direction : kDirections) {
        int rr = i + direction[0];
        int cc = j + direction[1];
        if ((rr >= 0) && (rr < smoke_map.size()) && (cc >= 0) &&
            (cc < smoke_map[rr].size())) {
          if (smoke_map[i][j] >= smoke_map[rr][cc]) {
            is_low_point = false;
            break;
          }
        }
      }
      if (is_low_point) {
        risk_levels.push_back(smoke_map[i][j] + 1);
      }
    }
  }
  return risk_levels;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  SmokeMap smoke_map = ParseSmokeMap(argv[1]);
  RiskVec risk_levels = FindRiskLevels(smoke_map);
  int risk_sum = std::accumulate(risk_levels.cbegin(), risk_levels.cend(), 0);

  std::cout << "Answer: " << risk_sum << std::endl;

  exit(EXIT_SUCCESS);
}
