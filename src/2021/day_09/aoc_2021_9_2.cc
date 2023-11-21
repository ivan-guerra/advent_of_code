#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_set>
#include <vector>

struct Point2D {
  int row = 0;
  int col = 0;

  friend bool operator==(const Point2D a, const Point2D& b) {
    return (a.row == b.row) && (a.col == b.col);
  }
};

struct Point2DHash {
  std::size_t operator()(const Point2D& p) const {
    std::string point_str = std::to_string(p.row) + "," + std::to_string(p.col);
    return std::hash<std::string>{}(point_str);
  }
};

using SmokeMap = std::vector<std::vector<int>>;
using PointVec = std::vector<Point2D>;

[[nodiscard]] SmokeMap ParseSmokeMap(const std::string& filename) {
  std::ifstream fhandle(filename);
  std::string line;
  SmokeMap smoke_map;
  while (std::getline(fhandle, line)) {
    std::vector<int> row;
    for (const char& c : line) {
      row.push_back(static_cast<int>(c - '0'));
    }
    smoke_map.push_back(row);
  }
  return smoke_map;
}

[[nodiscard]] PointVec FindLowPoints(const SmokeMap smoke_map) noexcept {
  static const std::vector<std::vector<int>> kDirections = {
      {0, 1},
      {1, 0},
      {-1, 0},
      {0, -1},
  };

  PointVec low_points;
  for (int i = 0; i < smoke_map.size(); ++i) {
    for (int j = 0; j < smoke_map[i].size(); ++j) {
      bool is_low_point = true;
      for (const auto& direction : kDirections) {
        int rr = i + direction[0];
        int cc = j + direction[1];
        if ((rr >= 0) && (rr < smoke_map.size()) && (cc >= 0) &&
            (cc < smoke_map[rr].size())) {
          if (smoke_map[i][j] >= smoke_map[rr][cc]) {
            is_low_point = false;
            break;
          }
        }
      }
      if (is_low_point) {
        low_points.push_back({.row = i, .col = j});
      }
    }
  }
  return low_points;
}

[[nodiscard]] PointVec GetNeighbors(const SmokeMap& smoke_map,
                                    const Point2D pos) {
  static const std::vector<std::vector<int>> kDirections = {
      {0, 1},
      {1, 0},
      {-1, 0},
      {0, -1},
  };

  PointVec neighbors;
  for (const auto& direction : kDirections) {
    int rr = pos.row + direction[0];
    int cc = pos.col + direction[1];
    if ((rr >= 0) && (rr < smoke_map.size()) && (cc >= 0) &&
        (cc < smoke_map[rr].size())) {
      neighbors.push_back({.row = rr, .col = cc});
    }
  }
  return neighbors;
}

[[nodiscard]] int GetBasinSize(const SmokeMap& smoke_map,
                               const Point2D& low_point) noexcept {
  std::queue<Point2D> buffer;
  buffer.push({low_point.row, low_point.col});

  int basin_size = 0;
  std::unordered_set<Point2D, Point2DHash> visited;
  while (!buffer.empty()) {
    Point2D curr_pos = buffer.front();
    buffer.pop();

    if (!visited.count(curr_pos)) {
      basin_size++;
      visited.insert(curr_pos);
    } else {
      continue;
    }

    auto neighbors = GetNeighbors(smoke_map, curr_pos);
    for (const auto& neighbor : neighbors) {
      int neighbor_pos_val = smoke_map[neighbor.row][neighbor.col];
      if (9 != neighbor_pos_val && !visited.count(neighbor)) {
        buffer.push(neighbor);
      }
    }
  }
  return basin_size;
}

[[nodiscard]] int GetLargestBasinProd(const SmokeMap& smoke_map,
                                      const PointVec& low_points) {
  std::vector<int> basin_sizes;
  for (const Point2D& lp : low_points) {
    basin_sizes.push_back(GetBasinSize(smoke_map, lp));
  }

  std::sort(basin_sizes.begin(), basin_sizes.end());

  int n = basin_sizes.size();
  return (basin_sizes[n - 1] * basin_sizes[n - 2] * basin_sizes[n - 3]);
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  SmokeMap smoke_map = ParseSmokeMap(argv[1]);
  PointVec low_points = FindLowPoints(smoke_map);

  std::cout << "Answer: " << GetLargestBasinProd(smoke_map, low_points)
            << std::endl;

  exit(EXIT_SUCCESS);
}
