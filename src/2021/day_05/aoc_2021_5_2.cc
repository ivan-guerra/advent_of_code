#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

struct Point2D {
  int x = 0;
  int y = 0;

  friend bool operator==(const Point2D& a, const Point2D& b) {
    return (a.x == b.x) && (a.y == b.y);
  }
  friend bool operator!=(const Point2D& a, const Point2D& b) {
    return !(a == b);
  }
};

struct Point2DHash {
  std::size_t operator()(const Point2D& point) const {
    std::string point_str =
        std::to_string(point.x) + "," + std::to_string(point.y);
    return std::hash<std::string>{}(point_str);
  }
};

using Point2DVec = std::vector<Point2D>;

struct LineSegment {
  Point2DVec points;
};

using LineSegmentVec = std::vector<LineSegment>;

[[nodiscard]] Point2DVec GeneratePoints(const Point2D& a, const Point2D& b) {
  int upper = -1;
  int lower = -1;
  Point2DVec points;
  if (a.x == b.x) { /* vertical line */
    upper = std::max(a.y, b.y);
    lower = std::min(a.y, b.y);
    for (int i = lower; i <= upper; ++i) {
      points.push_back({.x = a.x, .y = i});
    }
  } else if (a.y == b.y) { /* horizontal line */
    upper = std::max(a.x, b.x);
    lower = std::min(a.x, b.x);
    for (int i = lower; i <= upper; ++i) {
      points.push_back({.x = i, .y = a.y});
    }
  } else { /* diagonal line */
    int x_dt = (a.x - b.x < 0) ? 1 : -1;
    int y_dt = (a.y - b.y < 0) ? 1 : -1;
    Point2D point = a;
    while (point != b) {
      points.push_back(point);
      point.x += x_dt;
      point.y += y_dt;
    }
    points.push_back(b);
  }
  return points;
}

[[nodiscard]] LineSegmentVec ParseLineSegments(const std::string& filename) {
  std::ifstream fhandle(filename);
  std::string line;
  LineSegmentVec segments;
  while (std::getline(fhandle, line)) {
    Point2D start;
    Point2D end;
    std::sscanf(line.c_str(), "%d,%d -> %d,%d", &start.x, &start.y, &end.x,
                &end.y);

    Point2DVec points = GeneratePoints(start, end);
    if (!points.empty()) {
      segments.push_back({.points = points});
    }
  }
  return segments;
}

[[nodiscard]] int CountNumberOfIntersectsOverLimit(
    const LineSegmentVec& segments, int limit) noexcept {
  std::unordered_map<Point2D, int, Point2DHash> point_ctr;
  for (const LineSegment& segment : segments) {
    for (const Point2D& point : segment.points) {
      point_ctr[point]++;
    }
  }

  int ret = 0;
  for (const auto& kv : point_ctr) {
    if (kv.second >= limit) {
      ret++;
    }
  }
  return ret;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  LineSegmentVec segments = ParseLineSegments(argv[1]);
  std::cout << "Answer: " << CountNumberOfIntersectsOverLimit(segments, 2)
            << std::endl;

  exit(EXIT_SUCCESS);
}
