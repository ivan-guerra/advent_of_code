#include <algorithm>
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
using ProgWeights = std::unordered_map<std::string, int>;

ProgGraph ParseProgGraph(const std::string& data) {
    ProgGraph programs;

    std::ifstream input(data);
    std::string line;
    while (std::getline(input, line)) {
        std::istringstream iss(line);
        std::vector<std::string> tokens;
        std::copy(std::istream_iterator<std::string>(iss),
                  std::istream_iterator<std::string>(),
                  std::back_inserter(tokens));

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

ProgWeights ParseProgWeights(const std::string& data) {
    ProgWeights weights;

    std::ifstream input(data);
    std::string line;
    while (std::getline(input, line)) {
        std::istringstream iss(line);
        std::vector<std::string> tokens;
        std::copy(std::istream_iterator<std::string>(iss),
                  std::istream_iterator<std::string>(),
                  std::back_inserter(tokens));

        weights[tokens.front()] =
            std::stoi(tokens[1].substr(1, tokens[1].size() - 1));
    }
    return weights;
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

std::string GetRootProgram(const ProgGraph& programs) {
    for (const auto& [program, neighbors] : programs) {
        if (NumReachablePrograms(programs, program) == programs.size()) {
            return program;
        }
    }
    return ""; /* unreachable */
}

void GetNetWeights(const ProgGraph& programs, const ProgWeights& weights,
                   const std::string& root, ProgWeights& net_weights) {
    if (programs.at(root).empty()) {
        net_weights[root] = weights.at(root);
    }

    net_weights[root] = weights.at(root);
    for (const std::string& neighbor : programs.at(root)) {
        GetNetWeights(programs, weights, neighbor, net_weights);
        net_weights[root] += net_weights[neighbor];
    }
}

std::pair<std::string, int> GetOddManOut(
    std::vector<std::pair<std::string, int>>& nodes) {
    std::sort(nodes.begin(), nodes.end(),
              [](const std::pair<std::string, int>& n1,
                 const std::pair<std::string, int>& n2) {
                  return (n1.second < n2.second);
              });
    if (nodes[0].second != nodes[1].second) {
        return nodes[0];
    } else if (nodes[nodes.size() - 1].second !=
               nodes[nodes.size() - 2].second) {
        return nodes.back();
    }
    return std::make_pair("", 0);
}

int GetAdjustedWeight(const ProgGraph& programs, const ProgWeights& weights,
                      const ProgWeights& net_weights, const std::string& root) {
    std::queue<std::string> nodes;
    nodes.push(root);
    while (!nodes.empty()) {
        int num_nodes = nodes.size();
        std::vector<std::pair<std::string, int>> level_nodes;
        for (int i = 0; i < num_nodes; ++i) {
            std::string curr = nodes.front();
            nodes.pop();

            level_nodes.emplace_back(curr, net_weights.at(curr));
            for (const std::string& neighbor : programs.at(curr)) {
                nodes.push(neighbor);
            }
        }

        /* This is crappy but basically if a there's more than one node on a
         * level, one of those level nodes does not match the others, and going
         * deeper down into the mismatch nodes children does not yield a
         * mismatch, then we return the adjusted height. */
        if (level_nodes.size() > 1) {
            auto oddman = GetOddManOut(level_nodes);
            if (!oddman.first.empty()) {
                if (!programs.at(oddman.first).empty() &&
                    (GetAdjustedWeight(programs, weights, net_weights,
                                       oddman.first) > 0)) {
                    return GetAdjustedWeight(programs, weights, net_weights,
                                             oddman.first);
                }
                int expected_weight = level_nodes[1].second;
                int delta = (oddman.second > expected_weight)
                                ? (expected_weight - oddman.second)
                                : (oddman.second - expected_weight);

                return (weights.at(oddman.first) + delta);
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

    ProgGraph programs = ParseProgGraph(argv[1]);
    ProgWeights weights = ParseProgWeights(argv[1]);
    std::string root = GetRootProgram(programs);

    ProgWeights net_weights;
    GetNetWeights(programs, weights, root, net_weights);

    std::cout << "Answer: "
              << GetAdjustedWeight(programs, weights, net_weights, root)
              << std::endl;

    exit(EXIT_SUCCESS);
}
