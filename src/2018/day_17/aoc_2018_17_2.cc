#include <algorithm>
#include <climits>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

struct Point2D;
struct Point2DHash;
struct Vein;

using VeinMap = std::unordered_map<Point2D, Vein, Point2DHash>;

enum class Direction {
  kLeft,
  kRight,
};

enum class VeinType {
  kClay,
  kRunningWater,
  kStandingWater,
};

struct Point2D {
  int x;
  int y;

  friend bool operator==(const Point2D& lhs, const Point2D& rhs) {
    return (lhs.x == rhs.x) && (lhs.y == rhs.y);
  }
};

struct Point2DHash {
  std::size_t operator()(const Point2D& p) const {
    std::string hash_str = std::to_string(p.x) + "," + std::to_string(p.y);
    return std::hash<std::string>{}(hash_str);
  }
};

struct Vein {
  Point2D pos;
  VeinType type;
};

struct GridBounds {
  int horiz_min = INT_MAX;
  int horiz_max = INT_MIN;
  int vert_min = INT_MAX;
  int vert_max = INT_MIN;
};

void ParseVeinHelper(const std::string& word1, const std::string& word2,
                     bool x_first, VeinMap& veins) {
  int a = std::stoi(word1.substr(2, word1.size() - 1));

  std::size_t seperator_idx = word2.find("..");
  int b_start = std::stoi(word2.substr(2, word2.size() - seperator_idx));
  int b_end = std::stoi(word2.substr(seperator_idx + 2));

  for (int i = b_start; i <= b_end; ++i) {
    Point2D pos = {
        .x = (x_first) ? a : i,
        .y = (!x_first) ? a : i,
    };
    Vein cell = {
        .pos = pos,
        .type = VeinType::kClay,
    };
    veins[pos] = cell;
  }
}

void ParseVeins(const std::string& text, VeinMap& veins) {
  std::istringstream iss(text);
  std::vector<std::string> words;
  std::copy(std::istream_iterator<std::string>(iss),
            std::istream_iterator<std::string>(), std::back_inserter(words));

  if (words[0][0] == 'x') {
    ParseVeinHelper(words[0], words[1], true, veins);
  } else {
    ParseVeinHelper(words[0], words[1], false, veins);
  }
}

int FindWallBound(const VeinMap& veins, const Vein& src,
                  const GridBounds& bounds, Direction dir) {
  auto CheckRight = [](int i, const GridBounds& bounds) {
    return (i <= bounds.horiz_max);
  };
  auto CheckLeft = [](int i, const GridBounds& bounds) {
    return (i >= bounds.horiz_min);
  };

  auto CheckDir = (dir == Direction::kLeft) ? CheckLeft : CheckRight;
  int delta = (dir == Direction::kLeft) ? -1 : 1;
  for (int i = src.pos.x; CheckDir(i, bounds); i += delta) {
    Point2D pos = {
        .x = i,
        .y = src.pos.y,
    };
    if (!veins.count(pos)) {
      break;
    }
    if (veins.at(pos).type == VeinType::kClay) {
      return i;
    }
  }
  return -1;
}

void MarkWaterLocations(VeinMap& veins, Vein& curr, const GridBounds& bounds) {
  /* Current cell is either clay or has been previously been visited and
   * marked as water. */
  if (veins.count(curr.pos)) {
    return;
  }

  /* curr is a valid free vein we can mark with water. */
  curr.type = VeinType::kRunningWater;
  veins[curr.pos] = curr;

  Vein down = curr;
  down.pos.y += 1;

  /* Only continue down if the step doesn't take us out of bounds. */
  if (down.pos.y <= bounds.vert_max) {
    MarkWaterLocations(veins, down, bounds);
  }

  /* To go left or right, we must check if the vein immediately below us
   * is clay or water. clay/water. */
  if (veins.count(down.pos) &&
      (veins[down.pos].type == VeinType::kClay ||
       veins[down.pos].type == VeinType::kStandingWater)) {
    Vein left = curr;
    left.pos.x -= 1;
    MarkWaterLocations(veins, left, bounds);

    Vein right = curr;
    right.pos.x += 1;
    MarkWaterLocations(veins, right, bounds);
  }

  /* This is gross but it works lol. Find whether we are bounded on the
   * left and right by a clay vein. If so, mark all veins within the
   * left/right bound as standing water. */
  int lb = FindWallBound(veins, curr, bounds, Direction::kLeft);
  int rb = FindWallBound(veins, curr, bounds, Direction::kRight);
  if ((lb >= 0) && (rb >= 0)) {
    for (int i = lb + 1; i < rb; ++i) {
      Point2D pos = {.x = i, .y = curr.pos.y};
      if (veins.count(pos) && veins[pos].type == VeinType::kRunningWater) {
        veins[pos].type = VeinType::kStandingWater;
      }
    }
  }
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream input(argv[1]);
  std::string line;
  VeinMap veins;
  while (std::getline(input, line)) {
    ParseVeins(line, veins);
  }

  /* Find the max/min x and y coordinates. */
  GridBounds grid_bounds;
  for (const auto& [loc, cell] : veins) {
    grid_bounds.horiz_min = std::min(grid_bounds.horiz_min, cell.pos.x);
    grid_bounds.horiz_max = std::max(grid_bounds.horiz_max, cell.pos.x);
    grid_bounds.vert_min = std::min(grid_bounds.vert_min, cell.pos.y);
    grid_bounds.vert_max = std::max(grid_bounds.vert_max, cell.pos.y);
  }

  const Point2D kWellPos = {.x = 500, .y = 0};
  Vein src = {
      .pos = kWellPos,
      .type = VeinType::kRunningWater,
  };
  MarkWaterLocations(veins, src, grid_bounds);

  auto num_reachable_tiles =
      std::count_if(veins.cbegin(), veins.cend(), [](const auto& c) {
        return (c.second.type == VeinType::kStandingWater);
      });

  std::cout << "Answer: " << num_reachable_tiles << std::endl;

  exit(EXIT_SUCCESS);
}
