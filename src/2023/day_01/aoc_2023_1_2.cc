#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using CalVec = std::vector<std::string>;

[[nodiscard]] int GetScore(const std::string& calibration) {
  static const std::unordered_map<std::string, int> kWordToDigit = {
      {"one", 1}, {"two", 2},   {"three", 3}, {"four", 4}, {"five", 5},
      {"six", 6}, {"seven", 7}, {"eight", 8}, {"nine", 9}};
  static const std::vector<std::string> kNumStrs = {
      "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

  bool num_found = false;
  int score = 0;
  std::string num_str;
  for (const char& c : calibration) {
    if (std::isdigit(c)) {
      num_found = true;
      score = c - '0';
    } else {
      num_str += c;
      for (const std::string& num : kNumStrs) {
        if (std::string::npos != num_str.find(num)) {
          num_found = true;
          score = kWordToDigit.at(num);
        }
      }
    }

    if (num_found) {
      break;
    }
  }

  num_str.clear();
  num_found = false;

  for (auto it = calibration.crbegin(); it != calibration.crend(); it++) {
    if (std::isdigit(*it)) {
      num_found = true;
      score = (score * 10) + (*it - '0');
    } else {
      num_str.insert(num_str.begin(), *it);
      for (const std::string& num : kNumStrs) {
        if (std::string::npos != num_str.find(num)) {
          num_found = true;
          score = (score * 10) + kWordToDigit.at(num);
        }
      }
    }

    if (num_found) {
      break;
    }
  }

  return score;
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
