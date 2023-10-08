#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

using Range = std::pair<int, int>;

Range ParseRange(const std::string& input) {
  Range range;
  std::size_t seperator_idx = input.find('-');
  range.first = std::stoi(input.substr(0, seperator_idx));
  range.second = std::stoi(input.substr(seperator_idx + 1));

  return range;
}

bool HasTwoEqualAdjDigits(const std::string& num) {
  for (int i = 1; i < num.size(); ++i) {
    if (num[i] == num[i - 1]) {
      return true;
    }
  }
  return false;
}

bool IsNeverDecreasing(const std::string& num) {
  for (int i = 1; i < num.size(); ++i) {
    if (num[i] < num[i - 1]) {
      return false;
    }
  }
  return true;
}

int CountCandidatePasswords(const Range& passwords) {
  int candidates = 0;
  for (int i = passwords.first; i <= passwords.second; ++i) {
    std::string pass = std::to_string(i);
    if (HasTwoEqualAdjDigits(pass) && IsNeverDecreasing(pass)) {
      candidates++;
    }
  }
  return candidates;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream input(argv[1]);
  std::string line;
  std::getline(input, line);
  Range passwords = ParseRange(line);

  std::cout << "Answer: " << CountCandidatePasswords(passwords) << std::endl;

  exit(EXIT_SUCCESS);
}
