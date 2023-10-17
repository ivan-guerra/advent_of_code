#include <sys/types.h>

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

struct AdapterNode {
  int id = 0;
  int jolts = 0;
};

using AdapterGraph =
    std::unordered_map<int, std::pair<AdapterNode, std::vector<AdapterNode>>>;

void LoadGraph(const std::vector<int>& adapters, AdapterGraph& graph) {
  int id = 0;

  AdapterNode charging_outlet = {.id = id, .jolts = 0};
  graph[id++] = {charging_outlet, {}};

  for (const int& jolt : adapters) {
    AdapterNode node = {.id = id, .jolts = jolt};
    graph[id++] = {node, {}};
  }

  int max_jolt = *std::max_element(std::begin(adapters), std::end(adapters));
  AdapterNode dev_adapter = {.id = id, .jolts = max_jolt + 3};
  graph[id++] = {dev_adapter, {}};

  for (int i = 0; i < id; ++i) {
    for (int j = 0; j < id; ++j) {
      if (i != j) {
        AdapterNode& node1 = graph[i].first;
        AdapterNode& node2 = graph[j].first;
        int jolt_delta = node1.jolts - node2.jolts;
        if ((jolt_delta >= 1) && (jolt_delta <= 3)) {
          graph[node2.id].second.push_back(node1);
        }
      }
    }
  }
}

uint64_t CountPaths(const AdapterGraph& graph, int src, int dst,
                    std::unordered_map<int, uint64_t>& memo) {
  if (src == dst) {
    return 1;
  }

  if (memo.count(src)) {
    return memo[src];
  }

  for (const auto& neighbor : graph.at(src).second) {
    memo[src] += CountPaths(graph, neighbor.id, dst, memo);
  }
  return memo[src];
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream fhandle(argv[1]);
  std::string line;
  std::vector<int> adapters;
  while (std::getline(fhandle, line)) {
    adapters.push_back(std::stoi(line));
  }

  AdapterGraph graph;
  LoadGraph(adapters, graph);

  int src = 0;
  int dst = graph.size() - 1;
  std::unordered_map<int, uint64_t> memo;
  std::cout << "Answer: " << CountPaths(graph, src, dst, memo) << std::endl;

  exit(EXIT_SUCCESS);
}
