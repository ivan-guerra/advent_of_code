#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>

bool Rule1(const std::string s) {
  for (int i = 0; i < s.size() - 2; ++i) {
    std::string search_str = s.substr(i, 2);
    std::string search_space = s.substr(i + 2);
    if (search_space.find(search_str) != std::string::npos) {
      return true;
    }
  }
  return false;
}

bool Rule2(const std::string& s) {
  int window_size = 3;
  for (int i = 0; (s.size() - i) >= window_size; ++i) {
    if (s[i] == s[i + 2]) {
      return true;
    }
  }
  return false;
}

bool IsNice(const std::string& s) { return (Rule1(s) && Rule2(s)); }

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
