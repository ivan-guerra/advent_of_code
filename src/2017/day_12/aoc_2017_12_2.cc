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

using ProgramGraph = std::unordered_map<int, std::vector<int>>;

ProgramGraph ParseProgramGraph(const std::vector<std::string>& pipes) {
    ProgramGraph prog_graph;
    for (const std::string& pipe : pipes) {
        std::istringstream iss(pipe);
        std::vector<std::string> tokens;
        std::copy(std::istream_iterator<std::string>(iss),
                  std::istream_iterator<std::string>(),
                  std::back_inserter(tokens));

        int src = std::stoi(tokens.front());
        for (int i = 2; i < tokens.size(); ++i) {
            if (tokens[i].size() == 2) {
                prog_graph[src].push_back(
                    std::stoi(tokens[i].substr(0, tokens.size() - 1)));
            } else {
                prog_graph[src].push_back(std::stoi(tokens[i]));
            }
        }
    }
    return prog_graph;
}

std::size_t ComputeGroupHash(const ProgramGraph& prog_graph, int src) {
    std::queue<int> nodes;
    std::unordered_set<int> visited;
    std::vector<int> node_vals;

    nodes.push(src);
    while (!nodes.empty()) {
        int curr = nodes.front();
        nodes.pop();

        if (visited.count(curr)) {
            continue;
        }

        node_vals.push_back(curr);
        visited.insert(curr);
        for (const int& neighbor : prog_graph.at(curr)) {
            if (!visited.count(neighbor)) {
                nodes.push(neighbor);
            }
        }
    }

    std::sort(node_vals.begin(), node_vals.end());

    std::string key;
    for (const int& i : node_vals) {
        key += std::to_string(i) + ",";
    }
    return std::hash<std::string>{}(key);
}

int ComputeNumGroups(const ProgramGraph& prog_graph) {
    std::unordered_set<std::size_t> group_hashes;
    for (const auto& [pid, neighbors] : prog_graph) {
        group_hashes.insert(ComputeGroupHash(prog_graph, pid));
    }
    return group_hashes.size();
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "error: missing puzzle input file" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream input(argv[1]);
    std::string pipe_config;
    std::vector<std::string> pipes;
    while (std::getline(input, pipe_config)) {
        pipes.push_back(pipe_config);
    }

    ProgramGraph prog_graph = ParseProgramGraph(pipes);

    std::cout << "Answer: " << ComputeNumGroups(prog_graph) << std::endl;

    exit(EXIT_SUCCESS);
}
