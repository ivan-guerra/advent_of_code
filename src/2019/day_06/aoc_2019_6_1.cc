#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stack>
#include <string>
#include <unordered_map>

using OrbitGraph = std::unordered_map<std::string, std::string>;

void ParseOrbit(const std::string& input, OrbitGraph& orbits) {
  std::size_t seperator = input.find(')');
  orbits[input.substr(seperator + 1)] = input.substr(0, seperator);
}

int CountChildren(const std::string& root, const OrbitGraph& orbits) {
  std::stack<std::string> nodes;
  nodes.push(root);
  int num_children = 0;
  while (!nodes.empty()) {
    std::string curr = nodes.top();
    nodes.pop();

    if (orbits.count(curr)) {
      num_children++;
      nodes.push(orbits.at(curr));
    }
  }
  return num_children;
}

int ComputeOrbitChecksum(const OrbitGraph& orbits) {
  int checksum = 0;
  for (const auto& kv : orbits) {
    checksum += CountChildren(kv.first, orbits);
  }
  return checksum;
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

  std::cout << "Answer: " << ComputeOrbitChecksum(orbits) << std::endl;

  exit(EXIT_SUCCESS);
}
