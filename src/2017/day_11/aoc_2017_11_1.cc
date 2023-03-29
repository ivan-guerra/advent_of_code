#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

enum Direction {
    kNorth,
    kSouth,
    kNorthEast,
    kNorthWest,
    kSouthEast,
    kSouthWest,
};

struct CubeCoordinate {
    int q = -1;
    int r = 0;
    int s = 1;
};

std::vector<Direction> ParsePath(const std::string& text) {
    std::istringstream iss(text);
    std::vector<std::string> tokens;
    std::copy(std::istream_iterator<std::string>(iss),
              std::istream_iterator<std::string>(), std::back_inserter(tokens));

    std::vector<Direction> path;
    for (const std::string& token : tokens) {
        if (token == "ne") {
            path.push_back(Direction::kNorthEast);
        } else if (token == "nw") {
            path.push_back(Direction::kNorthWest);
        } else if (token == "n") {
            path.push_back(Direction::kNorth);
        } else if (token == "se") {
            path.push_back(Direction::kSouthEast);
        } else if (token == "sw") {
            path.push_back(Direction::kSouthWest);
        } else if (token == "s") {
            path.push_back(Direction::kSouth);
        }
    }
    return path;
}

CubeCoordinate GetChildLocation(const std::vector<Direction>& path) {
    CubeCoordinate origin;
    for (const Direction& dir : path) {
        switch (dir) {
            case kNorth:
                origin.r--;
                origin.q++;
                break;
            case kSouth:
                origin.r++;
                origin.q--;
                break;
            case kNorthEast:
                origin.q++;
                origin.s--;
                break;
            case kNorthWest:
                origin.s++;
                origin.r--;
                break;
            case kSouthEast:
                origin.s--;
                origin.r++;
                break;
            case kSouthWest:
                origin.q--;
                origin.s++;
                break;
        }
    }
    return origin;
}

int ComputeMinDistToChild(const CubeCoordinate& child_loc) {
    CubeCoordinate origin;
    return ((std::abs(origin.q - child_loc.q) +
             std::abs(origin.r - child_loc.r) +
             std::abs(origin.s - child_loc.s)) /
            2);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "error: missing puzzle input file" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream input(argv[1]);
    std::string text;
    std::getline(input, text);

    std::vector<Direction> path = ParsePath(text);
    CubeCoordinate child_location = GetChildLocation(path);

    std::cout << "Answer: " << ComputeMinDistToChild(child_location)
              << std::endl;

    exit(EXIT_SUCCESS);
}
