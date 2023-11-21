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

[[nodiscard]] char IsCorrupted(const std::string& instruction) {
  auto IsOpeningBrace = [](const char& c) {
    return (c == '{') || (c == '[') || (c == '(') || (c == '<');
  };

  static const std::unordered_map<char, char> kBraceLookup = {
      {'{', '}'}, {'(', ')'}, {'[', ']'}, {'<', '>'}};

  std::stack<char> buffer;
  for (const char& c : instruction) {
    if (IsOpeningBrace(c)) {
      buffer.push(c);
    } else {
      if (buffer.empty()) {
        return '\0'; /* should be unreachable by rules of this problem */
      } else {
        char buddy = kBraceLookup.at(buffer.top());
        buffer.pop();
        if (c != buddy) {
          return c;
        }
      }
    }
  }
  return '\0';
}

[[nodiscard]] int GetSyntaxErrorScore(const NavTable& instructions) {
  static const std::unordered_map<char, int> kScores = {
      {')', 3}, {']', 57}, {'}', 1197}, {'>', 25137}, {'\0', 0}};

  int score = 0;
  for (const std::string& instruction : instructions) {
    score += kScores.at(IsCorrupted(instruction));
  }
  return score;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  NavTable instructions = ParseNavTable(argv[1]);

  std::cout << "Answer: " << GetSyntaxErrorScore(instructions) << std::endl;

  exit(EXIT_SUCCESS);
}
