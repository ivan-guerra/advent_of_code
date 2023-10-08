#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>

std::vector<int> GetDivisors(int n) {
  std::vector<int> small_divisors;
  for (int i = 1; i <= std::sqrt(n) + 1; ++i) {
    if (0 == (n % i)) {
      small_divisors.push_back(i);
    }
  }

  std::vector<int> large_divisors;
  for (const int& i : small_divisors) {
    if (n != (i * i)) {
      large_divisors.push_back(n / i);
    }
  }

  small_divisors.insert(small_divisors.end(), large_divisors.begin(),
                        large_divisors.end());
  return small_divisors;
}

int GetNumPresents(int house, std::unordered_map<int, int>& delivery_tracker) {
  std::vector<int> divisors = GetDivisors(house);
  int num_presents = 0;
  for (const int& i : divisors) {
    ++delivery_tracker[i];
    if (delivery_tracker[i] < 50) {
      num_presents += i;
    }
  }
  return (num_presents * 11);
}

int GetLowestHouseWithTargetPresents(int target) {
  const int kHouseUpperBound = target / 10;
  std::unordered_map<int, int> delivery_tracker;
  for (int house = 1; house < kHouseUpperBound; ++house) {
    if (GetNumPresents(house, delivery_tracker) >= target) {
      return house;
    }
  }
  return -1; /* unreachable */
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream input(argv[1]);
  std::string target_presents;
  std::getline(input, target_presents);

  int target_num = std::stoi(target_presents);
  std::cout << "Answer: " << GetLowestHouseWithTargetPresents(target_num)
            << std::endl;

  exit(EXIT_SUCCESS);
}
