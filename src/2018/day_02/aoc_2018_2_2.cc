#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::string FindSimilarBoxes(const std::vector<std::string> &ids) {
  for (int i = 0; i < ids.size(); ++i) {
    for (int j = i; j < ids.size(); ++j) {
      if (ids[i].size() != ids[j].size()) {
        continue;
      }

      int num_differences = 0;
      int diff_idx = 0;
      for (int k = 0; k < ids[i].size(); ++k) {
        if (ids[i][k] != ids[j][k]) {
          num_differences++;
          diff_idx = k;
        }
      }

      if (num_differences == 1) {
        std::string common = ids[i];
        common.erase(common.begin() + diff_idx);
        return common;
      }
    }
  }
  return ""; /* unreachable */
}

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream input(argv[1]);
  std::string line;
  std::vector<std::string> ids;
  while (std::getline(input, line)) {
    ids.push_back(line);
  }

  std::cout << "Answer: " << FindSimilarBoxes(ids) << std::endl;

  exit(EXIT_SUCCESS);
}
