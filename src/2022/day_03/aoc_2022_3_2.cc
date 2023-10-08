#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

uint64_t GetRucksackBitMask(const std::string& rucksack) {
  uint64_t ret = 0;
  for (const char& c : rucksack) {
    int index = 0;
    if ((c >= 'a') && (c <= 'z')) {
      index = c - 'a';
    } else if ((c >= 'A') && (c <= 'Z')) {
      index = c - 'A' + 26;
    }
    ret |= (1ULL << index);
  }
  return ret;
}

int GetSetBit(uint64_t n) {
  for (int i = 0; i < 64; ++i) {
    if (n & (1ULL << i)) return i;
  }
  return -1;
}

char GetBadge(const std::vector<std::string>& rucksacks, int i) {
  uint64_t result = GetRucksackBitMask(rucksacks[i]) &
                    GetRucksackBitMask(rucksacks[i + 1]) &
                    GetRucksackBitMask(rucksacks[i + 2]);

  int set_bit = GetSetBit(result);
  return (set_bit < 26) ? ('a' + set_bit) : ('A' + (set_bit - 26));
}

int SumBadgePriorities(const std::vector<std::string>& rucksacks,
                       const std::unordered_map<char, int>& priorities) {
  int sum = 0;
  for (std::size_t i = 0; i < rucksacks.size(); i += 3) {
    sum += priorities.at(GetBadge(rucksacks, i));
  }
  return sum;
}

std::vector<std::string> LoadRucksacks(const std::string& input) {
  std::ifstream rucksack_if(input);

  std::string line;
  std::vector<std::string> rucksacks;
  while (std::getline(rucksack_if, line)) {
    rucksacks.push_back(line);
  }
  return rucksacks;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "usage: aoc INPUT_FILE" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::unordered_map<char, int> priorities;
  for (int i = 'a'; i <= 'z'; ++i)
    priorities[static_cast<char>(i)] = i - 'a' + 1;
  for (int i = 'A'; i <= 'Z'; ++i)
    priorities[static_cast<char>(i)] = i - 'A' + 27;

  std::vector<std::string> rucksacks = LoadRucksacks(argv[1]);
  std::cout << "Sum of Badge Priorities = "
            << SumBadgePriorities(rucksacks, priorities) << std::endl;

  exit(EXIT_SUCCESS);
}
