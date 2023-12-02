#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using CalVec = std::vector<std::string>;

[[nodiscard]] int GetScore(const std::string& calibration) {
  int digit_1 = 0;
  for (const char& c : calibration) {
    if (std::isdigit(c)) {
      digit_1 = c - '0';
      break;
    }
  }
  int digit_2 = 0;
  for (auto it = calibration.crbegin(); it != calibration.crend(); it++) {
    if (std::isdigit(*it)) {
      digit_2 = *it - '0';
      break;
    }
  }
  return ((digit_1 * 10) + digit_2);
}

[[nodiscard]] CalVec LoadCalibrations(const std::string& filename) {
  std::ifstream fhandle(filename);
  std::string line;
  CalVec cals;
  while (std::getline(fhandle, line)) {
    cals.push_back(line);
  }
  return cals;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  int answer = 0;
  CalVec cals = LoadCalibrations(argv[1]);
  for (const std::string& cal : cals) {
    answer += GetScore(cal);
  }
  std::cout << "Answer: " << answer << std::endl;

  exit(EXIT_SUCCESS);
}
