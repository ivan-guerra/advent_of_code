#include <algorithm>
#include <climits>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using HappinessGraph =
    std::unordered_map<std::string, std::unordered_map<std::string, int>>;

void AddEdge(const std::string& rule, HappinessGraph& happy_graph) {
    std::istringstream iss(rule);
    std::vector<std::string> tokens;
    std::copy(std::istream_iterator<std::string>(iss),
              std::istream_iterator<std::string>(), std::back_inserter(tokens));

    std::string src = tokens[0];
    std::string dst = tokens.back().substr(0, tokens.back().size() - 1);
    int weight = std::stoi(tokens[3]);
    if (tokens[2] == "lose") {
        weight = -weight;
    }

    happy_graph[src][dst] = weight;
}

int GetHappinessScore(const std::vector<std::string>& guest_names,
                      const HappinessGraph& happy_graph) {
    int score = 0;
    for (int i = 0; i < guest_names.size(); ++i) {
        std::string guest_to_left =
            guest_names[(i + guest_names.size() - 1) % guest_names.size()];
        std::string guest_to_right = guest_names[(i + 1) % guest_names.size()];

        score += happy_graph.at(guest_names[i]).at(guest_to_left) +
                 happy_graph.at(guest_names[i]).at(guest_to_right);
    }
    return score;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "error: missing puzzle input file" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream input(argv[1]);
    std::string rule;
    HappinessGraph happy_graph;
    while (std::getline(input, rule)) {
        AddEdge(rule, happy_graph);
    }

    std::vector<std::string> guest_names;
    for (const auto& kv : happy_graph) {
        guest_names.push_back(kv.first);
    }

    std::sort(guest_names.begin(), guest_names.end());
    int max_happiness_score = INT_MIN;
    do {
        max_happiness_score = std::max(
            max_happiness_score, GetHappinessScore(guest_names, happy_graph));
    } while (std::next_permutation(guest_names.begin(), guest_names.end()));

    std::cout << "Answer: " << max_happiness_score << std::endl;

    exit(EXIT_SUCCESS);
}
