#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <list>
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

bool CanFormSum(const std::list<uint64_t>& window, int target_sum) {
  std::unordered_set<uint64_t> history;
  for (const uint64_t& i : window) {
    uint64_t buddy = target_sum - i;
    if (history.count(buddy)) {
      return true;
    }
    history.insert(i);
  }
  return false;
}

int FindOutOfStreamNum(const std::vector<uint64_t>& nums, int preamble) {
  std::list<uint64_t> window;
  int i = 0;
  while ((i < nums.size()) && (i < preamble)) {
    window.push_back(nums[i]);
    i++;
  }

  while (i < nums.size()) {
    if (!CanFormSum(window, nums[i])) {
      return nums[i];
    }

    window.pop_front();
    window.push_back(nums[i]);

    i++;
  }
  return -1;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  const int kPreamble = 25;
  std::vector<uint64_t> nums = LoadNums(argv[1]);
  std::cout << "Answer: " << FindOutOfStreamNum(nums, kPreamble) << std::endl;

  exit(EXIT_SUCCESS);
}
