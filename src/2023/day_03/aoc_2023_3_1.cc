#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct Number;

using Schematic = std::vector<std::string>;
using Pos2D = std::pair<int, int>;
using Pos2DVec = std::vector<Pos2D>;
using NumberVec = std::vector<Number>;

struct Number {
  Pos2DVec digits;
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

[[nodiscard]] bool HasAdjSymbol(const Schematic& engine, int i,
                                int j) noexcept {
  static const std::vector<std::pair<int, int>> kDirections = {
      {0, 1}, {1, 0}, {1, 1}, {0, -1}, {-1, 0}, {-1, -1}, {1, -1}, {-1, 1},
  };

  int rr = 0;
  int cc = 0;
  for (const auto& direction : kDirections) {
    rr = i + direction.first;
    cc = j + direction.second;
    if ((rr >= 0) && (rr < engine.size()) && (cc >= 0) &&
        (cc < engine[0].size()) &&
        (!std::isdigit(engine[rr][cc]) && ('.' != engine[rr][cc]))) {
      return true;
    }
  }
  return false;
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

[[nodiscard]] int SumValidPartNums(const Schematic& engine,
                                   const NumberVec& nums) {
  auto ToInt = [&engine](const Number& num) {
    int value = 0;
    for (const auto& digit : num.digits) {
      value = (value * 10) + (engine[digit.first][digit.second] - '0');
    }
    return value;
  };

  int partnum_sum = 0;
  bool has_adj_sym = false;
  for (const Number& num : nums) {
    has_adj_sym = std::any_of(
        num.digits.cbegin(), num.digits.cend(), [&engine](const auto& pos) {
          return HasAdjSymbol(engine, pos.first, pos.second);
        });
    if (has_adj_sym) {
      partnum_sum += ToInt(num);
    }
  }
  return partnum_sum;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  Schematic engine = LoadSchematic(argv[1]);
  NumberVec nums = ParseNums(engine);

  std::cout << "Answer: " << SumValidPartNums(engine, nums) << std::endl;

  exit(EXIT_SUCCESS);
}
