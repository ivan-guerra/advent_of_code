#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>

bool Rule1(const std::string s) {
  static const std::unordered_set<char> kVowels = {'a', 'e', 'i', 'o', 'u'};
  int num_vowels = 0;
  for (const char& c : s) {
    if (kVowels.count(c)) {
      num_vowels++;
      if (num_vowels == 3) {
        return true;
      }
    }
  }
  return false;
}

bool Rule2(const std::string& s) {
  for (int i = 1; i < s.size(); ++i) {
    if (s[i] == s[i - 1]) {
      return true;
    }
  }
  return false;
}

bool Rule3(const std::string& s) {
  return (s.find("ab") == std::string::npos) &&
         (s.find("cd") == std::string::npos) &&
         (s.find("pq") == std::string::npos) &&
         (s.find("xy") == std::string::npos);
}

bool IsNice(const std::string& s) { return (Rule3(s) && Rule1(s) && Rule2(s)); }

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream input(argv[1]);
  std::string candidate;

  int nice = 0;
  while (std::getline(input, candidate)) {
    if (IsNice(candidate)) {
      nice++;
    }
  }

  std::cout << "Answer: " << nice << std::endl;

  exit(EXIT_SUCCESS);
}
