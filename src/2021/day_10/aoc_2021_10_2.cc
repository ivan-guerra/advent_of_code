#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

using NavTable = std::vector<std::string>;

[[nodiscard]] NavTable ParseNavTable(const std::string& filename) {
  std::ifstream fhandle(filename);
  std::string line;
  NavTable table;
  while (std::getline(fhandle, line)) {
    table.push_back(line);
  }
  return table;
}

[[nodiscard]] uint64_t GetAutoCompleteScore(const std::string& instruction) {
  auto IsOpeningBrace = [](const char& c) {
    return (c == '{') || (c == '[') || (c == '(') || (c == '<');
  };

  static const std::unordered_map<char, char> kBraceLookup = {
      {'{', '}'}, {'(', ')'}, {'[', ']'}, {'<', '>'}};
  static const std::unordered_map<char, int> kScore = {
      {')', 1}, {']', 2}, {'}', 3}, {'>', 4}};

  std::stack<char> buffer;
  for (const char& c : instruction) {
    if (IsOpeningBrace(c)) {
      buffer.push(c);
    } else {
      if (buffer.empty()) {
        return 0; /* should be unreachable by the rules of this problem */
      } else {
        char buddy = kBraceLookup.at(buffer.top());
        buffer.pop();
        if (c != buddy) {
          return 0;
        }
      }
    }
  }

  if (buffer.empty()) { /* well formed instruction */
    return 0;
  }

  uint64_t score = 0;
  while (!buffer.empty()) {
    score *= 5;
    score += kScore.at(kBraceLookup.at(buffer.top()));
    buffer.pop();
  }
  return score;
}

[[nodiscard]] uint64_t GetMiddleScore(const NavTable& instructions) {
  std::vector<uint64_t> scores;
  for (const std::string& instruction : instructions) {
    uint64_t score = GetAutoCompleteScore(instruction);
    if (score) {
      scores.push_back(score);
    }
  }

  std::sort(scores.begin(), scores.end());

  return scores[scores.size() / 2];
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  NavTable instructions = ParseNavTable(argv[1]);

  std::cout << "Answer: " << GetMiddleScore(instructions) << std::endl;

  exit(EXIT_SUCCESS);
}
