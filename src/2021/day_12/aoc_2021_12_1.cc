#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using Cave = std::string;
using CaveSet = std::unordered_set<std::string>;
using CaveGraph = std::unordered_map<Cave, std::vector<Cave>>;

[[nodiscard]] CaveGraph ParseCaveGraph(const std::string& filename) {
  std::ifstream fhandle(filename);
  std::string line;
  constexpr int kBuffSize = 256;
  char start_node[kBuffSize] = {0};
  char end_node[kBuffSize] = {0};
  CaveGraph caves;

  while (std::getline(fhandle, line)) {
    std::sscanf(line.c_str(), "%s - %s", start_node, end_node);
    caves[start_node].push_back(end_node);
    caves[end_node].push_back(start_node);
  }

  return caves;
}

/* taken straightfrom
 * https://www.reddit.com/r/adventofcode/comments/rehj2r/2021_day_12_solutions/
 * see 4HbQ's solution */
[[nodiscard]] int CountPaths(const CaveGraph& neighbors, CaveSet visited,
                             Cave cave) {
  if (cave == "end") {
    return 1;
  }
  if (visited.count(cave)) {
    if (cave == "start") {
      return 0;
    }
    if (std::all_of(cave.cbegin(), cave.cend(),
                    [](const char& c) { return std::islower(c); })) {
      return 0;
    }
  }

  int paths = 0;
  for (const Cave& neighbor : neighbors.at(cave)) {
    visited.insert(cave);
    paths += CountPaths(neighbors, visited, neighbor);
  }
  return paths;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  CaveGraph neighbors = ParseCaveGraph(argv[1]);
  CaveSet visited;
  std::cout << "Answer: " << CountPaths(neighbors, visited, "start")
            << std::endl;

  exit(EXIT_SUCCESS);
}
