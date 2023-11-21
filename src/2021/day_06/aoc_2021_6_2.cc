#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

using AgeVec = std::vector<int>;

[[nodiscard]] AgeVec ParseInitialAges(const std::string& filename) {
  std::ifstream fhandle(filename);
  std::string line;
  std::getline(fhandle, line);

  std::stringstream ss(line);
  std::string age;
  AgeVec ages;
  while (std::getline(ss, age, ',')) {
    ages.push_back(std::stoi(age));
  }
  return ages;
}

[[nodiscard]] uint64_t SimulateLanternfish(const AgeVec& ages, int days) {
  const int kSeniorAge = 7;
  const int kNewBornAge = 9;

  std::vector<uint64_t> tracker;
  for (int i = 0; i < kNewBornAge; ++i) {
    tracker.push_back(std::count(ages.cbegin(), ages.cend(), i));
  }

  for (int i = 0; i < days; ++i) {
    tracker[(i + kSeniorAge) % kNewBornAge] += tracker[i % kNewBornAge];
  }
  return std::accumulate(tracker.cbegin(), tracker.cend(), 0ULL);
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  AgeVec ages = ParseInitialAges(argv[1]);
  std::cout << "Answer: " << SimulateLanternfish(ages, 256) << std::endl;

  exit(EXIT_SUCCESS);
}
