#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <list>
#include <numeric>
#include <string>
#include <unordered_set>
#include <vector>

std::vector<uint64_t> LoadNums(const std::string& infile) {
  std::vector<uint64_t> nums;
  std::ifstream fhandle(infile);
  std::string line;
  while (std::getline(fhandle, line)) {
    nums.push_back(std::stoull(line));
  }
  return nums;
}

int FindTargetSubset(const std::vector<uint64_t>& nums, uint64_t target_sum) {
  for (int window_size = 2; window_size <= nums.size(); ++window_size) {
    for (int i = 0; (nums.size() - i) >= window_size; ++i) {
      uint64_t sum =
          std::accumulate(nums.begin() + i, nums.begin() + i + window_size, 0);
      if (sum == target_sum) {
        auto minmax = std::minmax_element(nums.begin() + i,
                                          nums.begin() + i + window_size);
        return (*minmax.first + *minmax.second);
      }
    }
  }
  return -1;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  const uint64_t kTargetSum = 22406676;
  std::vector<uint64_t> nums = LoadNums(argv[1]);
  std::cout << FindTargetSubset(nums, kTargetSum) << std::endl;

  exit(EXIT_SUCCESS);
}
