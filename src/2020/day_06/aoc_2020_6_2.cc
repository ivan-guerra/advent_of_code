#include <bitset>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using Group = std::vector<std::string>;
using Groups = std::vector<Group>;

Groups ParseGroups(const std::string& infile) {
  std::ifstream fhandle(infile);
  std::string line;
  Groups groups;
  Group group;
  while (std::getline(fhandle, line)) {
    if (line.empty()) {
      groups.push_back(group);
      group.clear();
    } else {
      group.push_back(line);
    }
  }
  groups.push_back(group);

  return groups;
}

int CountNumYes(const Group& group) {
  std::vector<std::bitset<26>> answers(group.size());
  for (int i = 0; i < group.size(); ++i) {
    for (int j = 0; j < group[i].size(); ++j) {
      answers[i][group[i][j] - 'a'] = 1;
    }
  }

  std::bitset<26> result;
  result.set();
  for (const auto& answer : answers) {
    result &= answer;
  }
  return result.count();
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  int total_yes = 0;
  Groups groups = ParseGroups(argv[1]);
  for (const Group& group : groups) {
    total_yes += CountNumYes(group);
  }
  std::cout << "Answer: " << total_yes << std::endl;

  exit(EXIT_SUCCESS);
}
