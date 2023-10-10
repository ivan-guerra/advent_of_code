#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using BagGraph =
    std::unordered_map<std::string, std::vector<std::pair<std::string, int>>>;

std::vector<std::string> SplitOnWhitespace(const std::string& s) {
  std::istringstream iss(s);
  std::vector<std::string> tokens;
  std::copy(std::istream_iterator<std::string>(iss),
            std::istream_iterator<std::string>(), std::back_inserter(tokens));
  return tokens;
}

void AddEdge(const std::string& rule, BagGraph& rules) {
  std::vector<std::string> tokens = SplitOnWhitespace(rule);

  /* parse the container bag */
  auto contain_it = std::find(std::begin(tokens), std::end(tokens), "contain");
  std::string container;
  for (auto it = tokens.begin(); it != contain_it - 1; it++) {
    container += *it + " ";
  }
  container.pop_back();

  if (std::find(std::begin(tokens), std::end(tokens), "no") != tokens.end()) {
    rules[container];
    return; /* this bag is empty */
  }

  int i = 0;
  while (!std::isdigit(rule[i])) {
    i++;
  }

  while (i < rule.size()) {
    if (std::isdigit(rule[i])) {
      int count = rule[i] - '0';
      i += 2; /* skip over digit and ' ' */
      std::string bag;
      while (rule[i] != '.' && rule[i] != ',') {
        bag += rule[i];
        i++;
      }

      /* erase the text trailing the bag color */
      int j = i;
      while (rule[j] != ' ') {
        bag.pop_back();
        j--;
      }
      i += 2; /* skip , or . and follow on ' ' */

      rules[container].push_back({bag, count});
    } else {
      i++;
    }
  }
}

int CountRequiredBags(const BagGraph& rules, const std::string& src) {
  int required = 1;
  for (const auto& neighbor : rules.at(src)) {
    required += neighbor.second * CountRequiredBags(rules, neighbor.first);
  }
  return required;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream fhandle(argv[1]);
  std::string line;
  BagGraph rules;
  while (std::getline(fhandle, line)) {
    AddEdge(line, rules);
  }

  const std::string kTarget = "shiny gold";
  std::cout << "Answer: " << CountRequiredBags(rules, kTarget) - 1 << std::endl;

  exit(EXIT_SUCCESS);
}
