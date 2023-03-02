#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using Coordinate = std::pair<int, int>;

Coordinate GetNextCoordinate(const Coordinate& prev) {
    Coordinate next;
    if ((prev.first - 1) == 0) {
        next.first = prev.second + 1;
        next.second = 1;
    } else {
        next.first = prev.first - 1;
        next.second = prev.second + 1;
    }
    return next;
}

Coordinate ParseTargetCoordinate(const std::string& instruction) {
    std::istringstream iss(instruction);
    std::vector<std::string> tokens;
    std::copy(std::istream_iterator<std::string>(iss),
              std::istream_iterator<std::string>(), std::back_inserter(tokens));

    Coordinate target;
    target.first = std::stoi(tokens[15].substr(0, tokens[15].size() - 1));
    target.second = std::stoi(tokens[17].substr(0, tokens[17].size() - 1));
    return target;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "error: missing puzzle input file" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream input(argv[1]);
    std::string instruction;
    std::getline(input, instruction);
    Coordinate target = ParseTargetCoordinate(instruction);

    const uint64_t kMultiplier = 252533;
    const uint64_t kModulo = 33554393;
    Coordinate curr(1, 1);
    uint64_t code = 20151125;
    do {
        curr = GetNextCoordinate(curr);
        code = ((code % kModulo) * (kMultiplier % kModulo)) % kModulo;
    } while (curr != target);

    std::cout << "Answer: " << code << std::endl;

    exit(EXIT_SUCCESS);
}
