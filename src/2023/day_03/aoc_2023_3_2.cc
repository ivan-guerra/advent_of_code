#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct Number;
struct Pos2DHash;

using Schematic = std::vector<std::string>;
using Pos2D = std::pair<int, int>;
using Pos2DVec = std::vector<Pos2D>;
using NumberVec = std::vector<Number>;
using NumberMap = std::unordered_map<Pos2D, int, Pos2DHash>;

struct Number {
  Pos2DVec digits;
};

struct Pos2DHash {
  std::size_t operator()(const Pos2D& p) const {
    std::string pos_str =
        std::to_string(p.first) + "," + std::to_string(p.second);
    return std::hash<std::string>{}(pos_str);
  }
};

[[nodiscard]] Schematic LoadSchematic(const std::string& filename) {
  std::ifstream fhandle(filename);
  std::string line;
  Schematic engine;
  while (std::getline(fhandle, line)) {
    engine.push_back(line);
  }
  return engine;
}

[[nodiscard]] NumberVec ParseNums(const Schematic& engine) {
  NumberVec nums;
  Number curr_num;
  for (int i = 0; i < engine.size(); ++i) {
    for (int j = 0; j < engine[0].size(); ++j) {
      if (std::isdigit(engine[i][j])) {
        curr_num.digits.emplace_back(i, j);
      } else if (!curr_num.digits.empty()) {
        nums.push_back(curr_num);
        curr_num.digits.clear();
      }
    }

    /* special case for nums on the edge of the schematic */
    if (!curr_num.digits.empty()) {
      nums.push_back(curr_num);
      curr_num.digits.clear();
    }
  }
  return nums;
}

[[nodiscard]] int ToInt(const Schematic& engine, const Number& num) noexcept {
  int value = 0;
  for (const auto& digit : num.digits) {
    value = (value * 10) + (engine[digit.first][digit.second] - '0');
  }
  return value;
};

[[nodiscard]] std::unordered_set<int> GetAdjNums(const Schematic& engine,
                                                 const NumberMap& num_lookup,
                                                 int i, int j) {
  static const std::vector<std::pair<int, int>> kDirections = {
      {0, 1}, {1, 0}, {1, 1}, {0, -1}, {-1, 0}, {-1, -1}, {1, -1}, {-1, 1},
  };

  std::unordered_set<int> adj_nums;
  int rr = 0;
  int cc = 0;
  for (const auto& direction : kDirections) {
    rr = i + direction.first;
    cc = j + direction.second;
    if ((rr >= 0) && (rr < engine.size()) && (cc >= 0) &&
        (cc < engine[0].size()) && std::isdigit(engine[rr][cc])) {
      adj_nums.insert(num_lookup.at({rr, cc}));
    }
  }
  return adj_nums;
}

[[nodiscard]] int GetGearRatioSum(const Schematic& engine,
                                  const NumberVec& nums) {
  NumberMap num_lookup;
  for (const Number& num : nums) {
    int value = ToInt(engine, num);
    for (const Pos2D& digit : num.digits) {
      num_lookup[digit] = value;
    }
  }

  int gear_ratio_sum = 0;
  for (int i = 0; i < engine.size(); ++i) {
    for (int j = 0; j < engine[0].size(); ++j) {
      if (engine[i][j] == '*') {
        std::unordered_set<int> adj_nums = GetAdjNums(engine, num_lookup, i, j);
        if (2 == adj_nums.size()) {
          gear_ratio_sum += std::accumulate(
              adj_nums.cbegin(), adj_nums.cend(), 1,
              [](const auto& a, const auto& b) { return (a * b); });
        }
      }
    }
  }
  return gear_ratio_sum;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  Schematic engine = LoadSchematic(argv[1]);
  NumberVec nums = ParseNums(engine);

  std::cout << "Answer: " << GetGearRatioSum(engine, nums) << std::endl;

  exit(EXIT_SUCCESS);
}
