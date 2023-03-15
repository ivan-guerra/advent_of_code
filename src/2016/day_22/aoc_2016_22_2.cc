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

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "error: missing puzzle input file" << std::endl;
        exit(EXIT_FAILURE);
    }

    /* Solved through visualization. */
    (void)argv;

    exit(EXIT_SUCCESS);
}
