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

int ComputeDistance(const CubeCoordinate& src, const CubeCoordinate& dst) {
  return ((std::abs(src.q - dst.q) + std::abs(src.r - dst.r) +
           std::abs(src.s - dst.s)) /
          2);
}

int ComputeMaxDistAlongPath(const std::vector<Direction>& path) {
  CubeCoordinate origin;
  CubeCoordinate child;
  int max_dist = 0;
  for (const Direction& dir : path) {
    switch (dir) {
      case kNorth:
        child.r--;
        child.q++;
        break;
      case kSouth:
        child.r++;
        child.q--;
        break;
      case kNorthEast:
        child.q++;
        child.s--;
        break;
      case kNorthWest:
        child.s++;
        child.r--;
        break;
      case kSouthEast:
        child.s--;
        child.r++;
        break;
      case kSouthWest:
        child.q--;
        child.s++;
        break;
    }
    max_dist = std::max(max_dist, ComputeDistance(origin, child));
  }
  return max_dist;
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
  std::cout << "Answer: " << ComputeMaxDistAlongPath(path) << std::endl;

  exit(EXIT_SUCCESS);
}
