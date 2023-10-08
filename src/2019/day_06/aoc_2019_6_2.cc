#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>

using OrbitGraph = std::unordered_map<std::string, std::string>;

void ParseOrbit(const std::string& input, OrbitGraph& orbits) {
  std::size_t seperator = input.find(')');
  std::string src = input.substr(seperator + 1);
  std::string dst = input.substr(0, seperator);

  orbits[src] = dst;
}

std::vector<std::string> GetOrbitPath(const std::string& src,
                                      const OrbitGraph& orbits) {
  std::vector<std::string> path;
  std::string curr = src;
  while (orbits.at(curr) != "COM") {
    curr = orbits.at(curr);
    path.push_back(curr);
  }
  return path;
}

int FindMinTransfers(const std::string& you, const std::string& santa,
                     const OrbitGraph& orbits) {
  auto you_path = GetOrbitPath(you, orbits);
  auto santa_path = GetOrbitPath(santa, orbits);

  std::string intersect;
  for (int i = 0; i < you_path.size(); ++i) {
    for (int j = 0; j < santa_path.size(); ++j) {
      if (you_path[i] == santa_path[j]) {
        return i + j;
      }
    }
  }
  return -1; /* unreachable */
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream input(argv[1]);
  std::string line;
  OrbitGraph orbits;
  while (std::getline(input, line)) {
    ParseOrbit(line, orbits);
  }

  std::cout << "Answer: " << FindMinTransfers("YOU", "SAN", orbits)
            << std::endl;

  exit(EXIT_SUCCESS);
}
