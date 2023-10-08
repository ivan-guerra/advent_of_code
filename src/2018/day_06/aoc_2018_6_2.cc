#include <algorithm>
#include <climits>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

struct Point {
  int x = -1;
  int y = -1;

  friend bool operator==(const Point& a, const Point& b) {
    return ((a.x == b.x) && (a.y == b.y));
  }
};

struct PointHash {
  std::size_t operator()(const Point& p) const {
    std::string hash_str = std::to_string(p.x) + "," + std::to_string(p.y);
    return std::hash<std::string>{}(hash_str);
  }
};

Point ParsePoint(const std::string& text) {
  std::istringstream iss(text);
  std::vector<std::string> words;
  std::copy(std::istream_iterator<std::string>(iss),
            std::istream_iterator<std::string>(), std::back_inserter(words));

  Point point;
  point.x = std::stoi(words[0].substr(0, words[0].size() - 1));
  point.y = std::stoi(words[1]);

  return point;
}

int Distance(const Point& a, const Point& b) {
  return (std::abs(a.x - b.x) + std::abs(a.y - b.y));
}

bool IsWithinRegion(const Point& candidate, const std::vector<Point>& points,
                    int dist_limit) {
  int dist_sum = 0;
  for (const Point& p : points) {
    dist_sum += Distance(candidate, p);
    if (dist_sum >= dist_limit) {
      return false;
    }
  }
  return true;
}

std::vector<Point> GetNeighbors(const Point& curr,
                                const std::vector<Point>& points,
                                int dist_limit) {
  using Direction = std::pair<int, int>;
  static const std::vector<Direction> directions = {
      {0, 1}, {1, 0}, {-1, 0}, {0, -1}, {1, 1}, {-1, -1}, {-1, 1}, {1, -1},
  };

  std::vector<Point> neighbors;
  for (const Direction& dir : directions) {
    Point candidate = {
        .x = curr.x + dir.first,
        .y = curr.y + dir.second,
    };
    if (IsWithinRegion(candidate, points, dist_limit)) {
      neighbors.push_back(candidate);
    }
  }
  return neighbors;
}

void SearchAllDirections(const Point& src, const std::vector<Point>& points,
                         std::unordered_set<Point, PointHash>& region,
                         int dist_limit) {
  std::unordered_set<Point, PointHash> visited;
  std::queue<Point> nodes;
  nodes.push(src);

  while (!nodes.empty()) {
    Point curr = nodes.front();
    nodes.pop();
    if (visited.count(curr)) {
      continue;
    }

    if (IsWithinRegion(curr, points, dist_limit)) {
      region.insert(curr);
    }

    visited.insert(curr);
    std::vector<Point> neighbors = GetNeighbors(curr, points, dist_limit);
    for (const Point& neighbor : neighbors) {
      if (!visited.count(neighbor)) {
        nodes.push(neighbor);
      }
    }
  }
}

int ComputeRegionSize(const std::vector<Point>& points, int dist_limit) {
  std::unordered_set<Point, PointHash> region;
  for (const Point& p : points) {
    SearchAllDirections(p, points, region, dist_limit);
  }
  return region.size();
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream input(argv[1]);
  std::string line;
  std::vector<Point> points;
  while (std::getline(input, line)) {
    points.push_back(ParsePoint(line));
  }

  const int kDistLimit = 10000;
  std::cout << "Answer: " << ComputeRegionSize(points, kDistLimit) << std::endl;

  exit(EXIT_SUCCESS);
}
