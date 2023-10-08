#include <climits>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <list>
#include <string>

bool HasReaction(char a, char b) {
  return ((std::tolower(a) == std::tolower(b)) && (a != b));
}

void ReducePolymer(std::list<char>& polymer) {
  bool modified = true;
  while ((polymer.size() > 1) && modified) {
    modified = false;
    auto p1 = polymer.begin();
    auto p2 = polymer.begin();
    p2++;
    while (p2 != polymer.end()) {
      if (HasReaction(*p1, *p2)) {
        polymer.erase(p1);
        polymer.erase(p2);
        modified = true;
        break;
      }
      p1++;
      p2++;
    }
  }
}

int FindOptimalPolymerLen(const std::list<char>& polymer) {
  int min_polymer_len = INT_MAX;
  const std::string kUnits = "abcdefghijklmnopqrstuvwxyz";
  for (const char& unit : kUnits) {
    std::list<char> candidate = polymer;
    candidate.remove(unit);
    candidate.remove(std::toupper(unit));

    ReducePolymer(candidate);
    min_polymer_len =
        std::min(min_polymer_len, static_cast<int>(candidate.size()));
  }
  return min_polymer_len;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream input(argv[1]);
  std::string line;
  std::getline(input, line);

  std::list<char> polymer(line.begin(), line.end());

  std::cout << "Answer: " << FindOptimalPolymerLen(polymer) << std::endl;

  exit(EXIT_SUCCESS);
}
