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

using BuildGraph = std::unordered_map<char, std::unordered_set<char>>;

void AddDependency(BuildGraph& graph, const std::string& text) {
    std::istringstream iss(text);
    std::vector<std::string> words;
    std::copy(std::istream_iterator<std::string>(iss),
              std::istream_iterator<std::string>(), std::back_inserter(words));

    graph[words[7][0]].insert(words[1][0]);
    graph[words[1][0]];
}

std::vector<char> TopologicalSort(BuildGraph& graph) {
    std::priority_queue<char, std::vector<char>, std::greater<char>> choices;
    for (const auto& [node, dependencies] : graph) {
        if (dependencies.empty()) {
            choices.push(node);
        }
    }

    std::vector<char> result;
    std::queue<char> no_dependencies;
    no_dependencies.push(choices.top());
    choices.pop();
    while (!no_dependencies.empty()) {
        char curr = no_dependencies.front();
        no_dependencies.pop();

        result.push_back(curr);

        for (auto& [node, dependencies] : graph) {
            if (dependencies.count(curr)) {
                dependencies.erase(curr);

                if (dependencies.empty()) {
                    choices.push(node);
                }
            }
        }

        if (!choices.empty()) {
            no_dependencies.push(choices.top());
            choices.pop();
        }
    }
    return result;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "error: missing puzzle input file" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream input(argv[1]);
    std::string line;
    BuildGraph graph;
    while (std::getline(input, line)) {
        AddDependency(graph, line);
    }

    std::vector<char> build_order = TopologicalSort(graph);
    for (const char& c : build_order) {
        std::cout << c;
    }
    std::cout << std::endl;

    exit(EXIT_SUCCESS);
}
