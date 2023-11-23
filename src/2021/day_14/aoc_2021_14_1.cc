#include <cstdlib>
#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <unordered_map>
#include <vector>

using Polymer = std::list<char>;
using RuleTable = std::unordered_map<std::string, char>;

[[nodiscard]] Polymer ParsePolymer(const std::string& filename) {
  std::ifstream fhandle(filename);
  std::string line;
  std::getline(fhandle, line);

  return Polymer(line.begin(), line.end());
}

[[nodiscard]] RuleTable ParseRules(const std::string& filename) {
  std::ifstream fhandle(filename);
  std::string line;
  while (std::getline(fhandle, line) && !line.empty()) {
    /* skip the polymer and blank line */
  }

  RuleTable rules;
  char lhs[3] = {0};
  char rhs = 0;
  while (std::getline(fhandle, line)) {
    std::sscanf(line.c_str(), "%s -> %c", lhs, &rhs);
    rules[lhs] = rhs;
  }
  return rules;
}

void ExpandPolymer(int steps, const RuleTable& rules, Polymer& polymer) {
  while (steps--) {
    auto first = polymer.begin();
    auto second = std::next(first, 1);
    std::string pair;
    while (second != polymer.end()) {
      pair += *first;
      pair += *second;

      char insert_char = rules.at(pair);
      pair.clear();

      auto insert_it = polymer.insert(second, insert_char);
      first = std::next(insert_it, 1);
      second = std::next(insert_it, 2);
    }
  }
}

[[nodiscard]] int FindMinMaxDiff(const Polymer& polymer) noexcept {
  std::unordered_map<char, int> counter;
  for (const char& c : polymer) {
    counter[c]++;
  }

  int min_count = polymer.size() + 1;
  int max_count = -1;
  for (const auto& kv : counter) {
    min_count = std::min(min_count, kv.second);
    max_count = std::max(max_count, kv.second);
  }
  return (max_count - min_count);
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  Polymer polymer = ParsePolymer(argv[1]);
  RuleTable rules = ParseRules(argv[1]);
  ExpandPolymer(10, rules, polymer);

  std::cout << "Answer: " << FindMinMaxDiff(polymer) << std::endl;

  std::exit(EXIT_SUCCESS);
}
