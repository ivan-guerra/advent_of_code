#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::vector<int> GetDigits(int n) {
  if (!n) {
    return {0};
  }

  std::vector<int> digits;
  while (n) {
    digits.push_back(n % 10);
    n /= 10;
  }
  std::reverse(digits.begin(), digits.end());

  return digits;
}

int FindSubVector(const std::vector<int>& data,
                  const std::vector<int>& values) {
  for (int i = 0; (data.size() - i) >= values.size(); ++i) {
    std::vector<int> slice(data.begin() + i, data.begin() + i + values.size());
    if (slice == values) {
      return i;
    }
  }
  return -1;
}

int RunSimulation(int target_scores) {
  const std::vector<int> kTargetRecipes = GetDigits(target_scores);

  int elf1 = 0;
  int elf2 = 1;
  std::vector<int> recipes = {3, 7};
  int match_idx = 0;
  while ((match_idx = FindSubVector(recipes, kTargetRecipes)) < 0) {
    int ctr = 100000;
    while (ctr--) {
      int sum = recipes[elf1] + recipes[elf2];
      std::vector<int> sum_digits = GetDigits(sum);
      recipes.insert(recipes.end(), sum_digits.begin(), sum_digits.end());

      elf1 = (elf1 + 1 + recipes[elf1]) % recipes.size();
      elf2 = (elf2 + 1 + recipes[elf2]) % recipes.size();
    }
  }
  return match_idx;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream input(argv[1]);
  std::string line;
  std::getline(input, line);
  int target_scores = std::stoi(line);

  std::cout << "Answer: " << RunSimulation(target_scores) << std::endl;

  exit(EXIT_SUCCESS);
}
