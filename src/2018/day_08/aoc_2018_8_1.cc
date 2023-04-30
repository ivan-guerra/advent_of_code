#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <numeric>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

struct Node;
using NodePtr = std::shared_ptr<Node>;
using NodeAndPos = std::pair<NodePtr, int>;

struct Node {
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

    NodePtr root = std::make_shared<Node>();
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

int SumMetaData(NodePtr& root) {
    std::stack<NodePtr> nodes;
    nodes.push(root);

    int sum = 0;
    while (!nodes.empty()) {
        NodePtr curr = nodes.top();
        nodes.pop();

        sum += std::accumulate(curr->metadata.begin(), curr->metadata.end(), 0);
        for (NodePtr& child : curr->children) {
            nodes.push(child);
        }
    }
    return sum;
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

    std::cout << "Answer: " << SumMetaData(root) << std::endl;

    exit(EXIT_SUCCESS);
}
