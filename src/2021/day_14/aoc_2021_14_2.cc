#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <unordered_map>
#include <vector>

using Polymer = std::string;
struct PolymerPair {
  char first = 0;
  char second = 0;

  friend bool operator==(const PolymerPair& a, const PolymerPair& b) {
    return (a.first == b.first) && (a.second == b.second);
  }
};

struct PolymerPairHash {
  std::size_t operator()(const PolymerPair& p) const {
    std::string pair_str =
        std::string(1, p.first) + "," + std::string(1, p.second);
    return std::hash<std::string>{}(pair_str);
  }
};

using RuleTable = std::unordered_map<std::string, char>;
using PairCounter = std::unordered_map<PolymerPair, uint64_t, PolymerPairHash>;
using CharCounter = std::unordered_map<char, uint64_t>;

[[nodiscard]] Polymer ParsePolymer(const std::string& filename) {
  std::ifstream fhandle(filename);
  std::string polymer;
  std::getline(fhandle, polymer);

  return polymer;
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

[[nodiscard]] CharCounter CountChars(const Polymer& polymer) {
  CharCounter counter;
  for (const char& c : polymer) {
    counter[c]++;
  }
  return counter;
}

[[nodiscard]] PairCounter CountPairs(const Polymer& polymer) {
  PairCounter counter;
  for (int i = 1; i < polymer.size(); ++i) {
    counter[{polymer[i - 1], polymer[i]}]++;
  }
  return counter;
}

[[nodiscard]] uint64_t FindMinMaxDiff(int steps, const RuleTable& rules,
                                      CharCounter& char_counts,
                                      PairCounter& pair_counts) {
  auto CharPairToStr = [](const PolymerPair& p) -> std::string {
    return std::string(1, p.first) + std::string(1, p.second);
  };

  while (steps--) {
    PairCounter pair_cp = pair_counts;
    for (const auto& kv : pair_cp) {
      char x = rules.at(CharPairToStr(kv.first));
      pair_counts[kv.first] -= kv.second;
      pair_counts[{kv.first.first, x}] += kv.second;
      pair_counts[{x, kv.first.second}] += kv.second;
      char_counts[x] += kv.second;
    }
  }

  uint64_t min_count = std::numeric_limits<uint64_t>::max();
  uint64_t max_count = 0;
  for (const auto& kv : char_counts) {
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
  CharCounter char_counts = CountChars(polymer);
  PairCounter pair_counts = CountPairs(polymer);

  std::cout << "Answer: " << FindMinMaxDiff(40, rules, char_counts, pair_counts)
            << std::endl;

  std::exit(EXIT_SUCCESS);
}
