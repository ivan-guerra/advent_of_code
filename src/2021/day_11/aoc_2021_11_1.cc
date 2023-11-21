#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

struct Point2D {
  int x = 0;
  int y = 0;

  friend bool operator==(const Point2D& a, const Point2D& b) {
    return (a.x == b.x) && (a.y == b.y);
  }
};

struct Point2DHash {
  std::size_t operator()(const Point2D& p) const {
    std::string point_str = std::to_string(p.x) + "," + std::to_string(p.y);
    return std::hash<std::string>{}(point_str);
  }
};

using OctopusGrid = std::vector<std::vector<int>>;
using FlashPointSet = std::unordered_set<Point2D, Point2DHash>;

[[nodiscard]] OctopusGrid ParseOctoGrid(const std::string& filename) {
  std::ifstream fhandle(filename);
  std::string line;
  OctopusGrid grid;
  while (std::getline(fhandle, line)) {
    std::vector<int> row;
    std::transform(line.begin(), line.end(), std::back_inserter(row),
                   [](const char& c) { return (c - '0'); });
    grid.push_back(row);
  }
  return grid;
}

[[nodiscard]] bool HasFlashPoint(const OctopusGrid& grid,
                                 const FlashPointSet& flash_points,
                                 Point2D& flash_point_out) noexcept {
  for (int i = 0; i < grid.size(); ++i) {
    for (int j = 0; j < grid[i].size(); ++j) {
      flash_point_out.x = i;
      flash_point_out.y = j;
      if (grid[i][j] >= 9 && !flash_points.count(flash_point_out)) {
        return true;
      }
    }
  }
  return false;
}

[[nodiscard]] int UpdateOctopuses(OctopusGrid& grid) {
  static const std::vector<Point2D> kDirections = {
      {.x = 0, .y = 1},  {.x = 1, .y = 0},  {.x = 0, .y = -1},
      {.x = -1, .y = 0}, {.x = 1, .y = 1},  {.x = -1, .y = 1},
      {.x = 1, .y = -1}, {.x = -1, .y = -1}};

  /* Find all flash points and flash them. Keep track of the points in the grid
   * that have flashed so as to be sure not flash them twice in a single step
   * and so that we can avoid incrementing later in the final increment loop. */
  FlashPointSet flash_points;
  Point2D flash_point;
  while (HasFlashPoint(grid, flash_points, flash_point)) {
    /* flash and reset this octopus */
    grid[flash_point.x][flash_point.y] = 0;
    flash_points.insert(flash_point);

    /* update the neighbor values */
    for (const Point2D& direction : kDirections) {
      int rr = flash_point.x + direction.x;
      int cc = flash_point.y + direction.y;
      if ((rr >= 0) && (rr < grid.size()) && (cc >= 0) &&
          (cc < grid[0].size()) && !flash_points.count({.x = rr, .y = cc})) {
        /* only increment a neighbor if rr/cc are in bounds and the location
         * they point to (i.e., (rr, cc)) has not already been flashed during
         * this round */
        grid[rr][cc]++;
      }
    }
  }

  /* increase all octo values by one at the end of the round */
  for (int i = 0; i < grid.size(); ++i) {
    for (int j = 0; j < grid[0].size(); ++j) {
      if (!flash_points.count({.x = i, .y = j})) {
        grid[i][j]++;
      }
    }
  }

  return flash_points.size();
}

[[nodiscard]] int GetTotalFlashes(int steps, OctopusGrid& grid) {
  int flashes = 0;
  while (steps--) {
    flashes += UpdateOctopuses(grid);
  }
  return flashes;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  OctopusGrid grid = ParseOctoGrid(argv[1]);
  std::cout << "Answer: " << GetTotalFlashes(100, grid) << std::endl;

  exit(EXIT_SUCCESS);
}
