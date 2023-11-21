#include <cstdlib>
#include <fstream>
#include <iostream>
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

[[nodiscard]] int SimulateLanternfish(int days, AgeVec& ages) {
  const int kSeniorAge = 6;
  const int kNewBornAge = 8;

  int eights = 0;
  while (days--) {
    for (int& age : ages) {
      if (0 == age) {
        age = kSeniorAge;
        eights++;
      } else {
        age--;
      }
    }

    while (eights) {
      ages.push_back(kNewBornAge);
      eights--;
    }
  }
  return ages.size();
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  AgeVec ages = ParseInitialAges(argv[1]);

  std::cout << "Answer: " << SimulateLanternfish(80, ages) << std::endl;

  exit(EXIT_SUCCESS);
}
