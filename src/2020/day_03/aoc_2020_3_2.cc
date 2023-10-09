#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

using TreeMap = std::vector<std::string>;

struct Slope {
  int x;
  int y;
};

TreeMap ParseTreeMap(const std::string& infile) {
  std::ifstream infile_handle(infile);
  std::string line;
  TreeMap map;
  while (std::getline(infile_handle, line)) {
    map.push_back(line);
  }
  return map;
}

int CountTrees(TreeMap& map, const Slope& slope) {
  int num_trees = 0;
  int lower_bound = map.size();
  std::pair<int, int> curr_pos = {0, 0};
  while (curr_pos.first < lower_bound) {
    if (map[curr_pos.first][curr_pos.second] == '#') {
      num_trees++;
    }

    curr_pos.first += slope.y;
    if (curr_pos.first >= lower_bound) {
      break;
    }
    curr_pos.second += slope.x;
    if (curr_pos.second >= map[curr_pos.first].size()) {
      for (int i = curr_pos.first; i < lower_bound; ++i) {
        map[i] += map[i];
      }
    }
  }
  return num_trees;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  TreeMap map = ParseTreeMap(argv[1]);
  std::vector<Slope> slopes = {{.x = 1, .y = 1},
                               {.x = 3, .y = 1},
                               {.x = 5, .y = 1},
                               {.x = 7, .y = 1},
                               {.x = 1, .y = 2}};
  uint64_t product = 1;
  for (const Slope& slope : slopes) {
    product *= CountTrees(map, slope);
  }
  std::cout << "Answer: " << product << std::endl;

  exit(EXIT_SUCCESS);
}
