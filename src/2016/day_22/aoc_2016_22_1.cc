#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

struct Node {
  int size = 0;
  int used = 0;
  int available = 0;
  int percent_used = 0;
  int x = 0;
  int y = 0;

  friend bool operator==(const Node& a, const Node& b) {
    return ((a.x == b.x) && (a.y == b.y));
  }
  friend bool operator!=(const Node& a, const Node& b) { return !(a == b); }
};

Node ParseNode(const std::string& text) {
  std::istringstream iss(text);
  std::vector<std::string> tokens;
  std::copy(std::istream_iterator<std::string>(iss),
            std::istream_iterator<std::string>(), std::back_inserter(tokens));

  auto RemoveLastChar = [](const std::string& s) {
    return s.substr(0, s.size() - 1);
  };

  std::string x;
  for (int i = tokens[0].find('x') + 1; tokens[0][i] != '-'; ++i) {
    x += tokens[0][i];
  }
  std::string y;
  for (int i = tokens[0].find('y') + 1; i < tokens[0].size(); ++i) {
    y += tokens[0][i];
  }
  Node node = {
      .size = std::stoi(RemoveLastChar(tokens[1])),
      .used = std::stoi(RemoveLastChar(tokens[2])),
      .available = std::stoi(RemoveLastChar(tokens[3])),
      .percent_used = std::stoi(RemoveLastChar(tokens[4])),
      .x = std::stoi(x),
      .y = std::stoi(y),
  };
  return node;
}

bool IsViablePair(const Node& a, const Node& b) {
  return ((0 != a.used) && (a != b) && (a.used <= b.available));
}

int GetNumViablePairs(const std::vector<Node>& nodes) {
  int num_viable_pair = 0;
  for (const Node& n1 : nodes) {
    for (const Node& n2 : nodes) {
      if (IsViablePair(n1, n2)) {
        num_viable_pair++;
      }
    }
  }
  return num_viable_pair;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream input(argv[1]);
  std::string node;
  std::vector<Node> nodes;
  while (std::getline(input, node)) {
    nodes.push_back(ParseNode(node));
  }

  std::cout << "Answer: " << GetNumViablePairs(nodes) << std::endl;

  exit(EXIT_SUCCESS);
}
