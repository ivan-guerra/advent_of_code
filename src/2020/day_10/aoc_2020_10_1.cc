#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

struct AdapterNode {
  int id = 0;
  int jolts = 0;
};

using AdapterGraph =
    std::unordered_map<int, std::pair<AdapterNode, std::vector<AdapterNode>>>;
using InDegreeSet = std::unordered_map<int, int>;

void LoadGraph(const std::vector<int>& adapters, AdapterGraph& graph,
               InDegreeSet& in_degrees) {
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
          in_degrees[node1.id]++;
        }
      }
    }
  }
}

int ComputeDeltaProduct(const std::vector<AdapterNode>& sorted_nodes) {
  std::unordered_map<int, int> delta_counts;
  for (int i = 1; i < sorted_nodes.size(); ++i) {
    delta_counts[sorted_nodes[i].jolts - sorted_nodes[i - 1].jolts]++;
  }
  return (delta_counts[1] * delta_counts[3]);
}

std::vector<AdapterNode> TopologicalSort(const AdapterGraph& graph,
                                         InDegreeSet& in_degrees) {
  std::queue<AdapterNode> nodes;
  nodes.push(graph.at(0).first);

  std::vector<AdapterNode> sorted_nodes;
  while (!nodes.empty()) {
    AdapterNode curr = nodes.front();
    nodes.pop();

    sorted_nodes.push_back(curr);
    for (const AdapterNode& node : graph.at(curr.id).second) {
      in_degrees[node.id]--;
      if (in_degrees[node.id] == 0) {
        nodes.push(node);
      }
    }
  }
  return sorted_nodes;
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
  InDegreeSet in_degrees;
  LoadGraph(adapters, graph, in_degrees);

  std::vector<AdapterNode> sorted_nodes = TopologicalSort(graph, in_degrees);
  std::cout << "Answer: " << ComputeDeltaProduct(sorted_nodes) << std::endl;

  exit(EXIT_SUCCESS);
}
