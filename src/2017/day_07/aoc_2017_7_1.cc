#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using ProgGraph = std::unordered_map<std::string, std::vector<std::string>>;

ProgGraph ParseProgGraph(const std::string& data) {
  ProgGraph programs;

  std::ifstream input(data);
  std::string line;
  while (std::getline(input, line)) {
    std::istringstream iss(line);
    std::vector<std::string> tokens;
    std::copy(std::istream_iterator<std::string>(iss),
              std::istream_iterator<std::string>(), std::back_inserter(tokens));

    if (tokens.size() == 2) {
      if (!programs.count(tokens.front())) {
        programs[tokens.front()] = {};
      }
    } else {
      std::vector<std::string> neighbors;
      for (int i = 3; i < tokens.size() - 1; ++i) {
        neighbors.push_back(tokens[i].substr(0, tokens[i].size() - 1));
      }
      neighbors.push_back(tokens.back());

      programs[tokens.front()] = neighbors;
    }
  }
  return programs;
}

int NumReachablePrograms(const ProgGraph& programs, const std::string& src) {
  std::queue<std::string> nodes;
  nodes.push(src);
  std::unordered_set<std::string> visited;
  while (!nodes.empty()) {
    std::string curr = nodes.front();
    nodes.pop();

    visited.insert(curr);
    for (const std::string& neighbor : programs.at(curr)) {
      if (!visited.count(neighbor)) {
        nodes.push(neighbor);
      }
    }
  }
  return visited.size();
}

std::string GetRootProgram(const std::string& data) {
  ProgGraph programs = ParseProgGraph(data);
  for (const auto& [program, neighbors] : programs) {
    if (NumReachablePrograms(programs, program) == programs.size()) {
      return program;
    }
  }
  return ""; /* unreachable */
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::cout << "Answer: " << GetRootProgram(argv[1]) << std::endl;

  exit(EXIT_SUCCESS);
}
