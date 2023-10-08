#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <numeric>
#include <queue>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct Node;
using NodePtr = std::shared_ptr<Node>;
using NodeAndPos = std::pair<NodePtr, int>;
using DependencyGraph =
    std::unordered_map<NodePtr, std::unordered_set<NodePtr>>;
using NodeMap = std::unordered_map<int, NodePtr>;

struct Node {
  int id = -1;
  int value = 0;
  std::vector<NodePtr> children;
  std::vector<int> metadata;
};

std::vector<int> ParseLicenseData(const std::string& text) {
  std::istringstream iss(text);
  std::vector<std::string> words;
  std::copy(std::istream_iterator<std::string>(iss),
            std::istream_iterator<std::string>(), std::back_inserter(words));

  std::vector<int> values;
  for (const std::string& word : words) {
    values.push_back(std::stoi(word));
  }
  return values;
}

NodeAndPos AddNode(const std::vector<int>& values, int pos) {
  if (pos >= values.size()) {
    return std::make_pair(nullptr, -1);
  }

  static int id = 0;
  NodePtr root = std::make_shared<Node>();
  root->id = id++;
  int num_children = values[pos];
  int num_meta = values[pos + 1];
  pos += 2;
  for (int i = 0; i < num_children; ++i) {
    NodeAndPos child = AddNode(values, pos);
    pos = child.second;
    root->children.push_back(child.first);
  }

  while (num_meta--) {
    root->metadata.push_back(values[pos++]);
  }

  return std::make_pair(root, pos);
}

void InitDependencyGraph(NodePtr& root, DependencyGraph& deps) {
  std::stack<NodePtr> nodes;
  nodes.push(root);

  while (!nodes.empty()) {
    NodePtr curr = nodes.top();
    nodes.pop();

    for (NodePtr& child : curr->children) {
      deps[curr].insert(child);
      deps[child];
      nodes.push(child);
    }
  }
}

void InitNodeMap(NodePtr& root, NodeMap& map) {
  std::stack<NodePtr> nodes;
  nodes.push(root);

  while (!nodes.empty()) {
    NodePtr curr = nodes.top();
    nodes.pop();

    map[curr->id] = curr;
    for (NodePtr& child : curr->children) {
      nodes.push(child);
    }
  }
}

std::vector<int> TopologicalSort(DependencyGraph& deps) {
  std::queue<NodePtr> no_dependencies;
  for (const auto& [node, dependencies] : deps) {
    if (dependencies.empty()) {
      no_dependencies.push(node);
    }
  }

  std::vector<int> result;
  while (!no_dependencies.empty()) {
    NodePtr curr = no_dependencies.front();
    no_dependencies.pop();

    result.push_back(curr->id);

    for (auto& [node, dependencies] : deps) {
      if (dependencies.count(curr)) {
        dependencies.erase(curr);

        if (dependencies.empty()) {
          no_dependencies.push(node);
        }
      }
    }
  }
  return result;
}

int EvaluateTree(const std::vector<int>& eval_order, NodeMap& nodes) {
  for (const int& id : eval_order) {
    if (nodes[id]->children.empty()) {
      nodes[id]->value = std::accumulate(nodes[id]->metadata.begin(),
                                         nodes[id]->metadata.end(), 0);
    } else {
      for (const int& index : nodes[id]->metadata) {
        int child_idx = index - 1;
        if ((child_idx >= 0) && (child_idx < nodes[id]->children.size())) {
          nodes[id]->value += nodes[id]->children[child_idx]->value;
        }
      }
    }
  }
  return nodes[0]->value;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream input(argv[1]);
  std::string line;
  std::getline(input, line);

  std::vector<int> values = ParseLicenseData(line);
  NodePtr root = AddNode(values, 0).first;

  DependencyGraph deps;
  InitDependencyGraph(root, deps);

  NodeMap nodes;
  InitNodeMap(root, nodes);

  std::vector<int> eval_order = TopologicalSort(deps);

  std::cout << "Answer: " << EvaluateTree(eval_order, nodes) << std::endl;

  exit(EXIT_SUCCESS);
}
