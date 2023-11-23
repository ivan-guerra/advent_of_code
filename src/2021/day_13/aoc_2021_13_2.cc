#include <cstdlib>
#include <fstream>
#include <functional>
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

enum class FoldType {
  kVertical,
  kHorizontal,
};

struct Fold {
  FoldType type = FoldType::kVertical;
  int value = 0;
};

using Point2DSet = std::unordered_set<Point2D, Point2DHash>;
using FoldVec = std::vector<Fold>;

[[nodiscard]] Point2DSet ParsePoints(const std::string& filename) {
  std::ifstream fhandle(filename);
  std::string line;
  Point2DSet points;
  while (std::getline(fhandle, line) && !line.empty()) {
    Point2D point;
    std::sscanf(line.c_str(), "%d,%d", &point.x, &point.y);
    points.insert(point);
  }
  return points;
}

[[nodiscard]] FoldVec ParseFolds(const std::string& filename) {
  std::ifstream fhandle(filename);
  std::string line;
  while (std::getline(fhandle, line) && !line.empty()) {
    /* skip over points and the trailing blank line */
  }

  FoldVec folds;
  char fold_type = 0;
  int value = 0;
  while (std::getline(fhandle, line)) {
    std::sscanf(line.c_str(), "fold along %c=%d", &fold_type, &value);
    if (fold_type == 'y') {
      folds.push_back({.type = FoldType::kHorizontal, .value = value});
    } else {
      folds.push_back({.type = FoldType::kVertical, .value = value});
    }
  }
  return folds;
}

void FoldPaper(const Fold& fold, Point2DSet& points) {
  /* find the current dimensions of the view plane (2D grid) */
  int width = -1;
  int height = -1;
  for (const Point2D& point : points) {
    width = std::max(width, point.x);
    height = std::max(height, point.y);
  }

  /* since we're folding either vertically or horizontally, one of the
   * dimensions will get halved */
  width = (fold.type == FoldType::kVertical) ? (width / 2) : width;
  height = (fold.type == FoldType::kHorizontal) ? (height / 2) : height;

  auto InBounds = [&width, &height](const Point2D& p) {
    return ((p.x >= 0) && (p.x <= width) && (p.y >= 0) && (p.y <= height));
  };

  std::vector<Point2D> new_points;
  if (fold.type == FoldType::kHorizontal) {
    for (auto it = points.begin(); it != points.end();) {
      if (it->y > fold.value) {
        Point2D new_point = {.x = it->x,
                             .y = fold.value - (it->y - fold.value)};
        it = points.erase(it);
        if (InBounds(new_point)) {
          new_points.push_back(new_point);
        }
      } else {
        it++;
      }
    }
  } else if (fold.type == FoldType::kVertical) {
    for (auto it = points.begin(); it != points.end();) {
      if (it->x > fold.value) {
        Point2D new_point = {.x = fold.value - (it->x - fold.value),
                             .y = it->y};
        it = points.erase(it);
        if (InBounds(new_point)) {
          new_points.push_back(new_point);
        }
      } else {
        it++;
      }
    }
  }

  /* push any new points into the point set, since we're using a set, duplicates
   * will be automatically removed */
  points.insert(new_points.begin(), new_points.end());
}

void FoldAndPrint(const FoldVec& folds, Point2DSet& points) {
  /* fold the paper as instructed in the input */
  for (const Fold& fold : folds) {
    FoldPaper(fold, points);
  }

  /* find the dimensions of the folded paper */
  int width = -1;
  int height = -1;
  for (const Point2D& point : points) {
    width = std::max(width, point.x);
    height = std::max(height, point.y);
  }

  /* initialize a 2D char matrix with '#' symbols corresponding to points on the
   * folded paper */
  std::vector<std::vector<char>> text(height + 1,
                                      std::vector<char>(width + 1, ' '));
  for (const Point2D& p : points) {
    text[p.y][p.x] = '#';
  }

  /* print the table to stdout */
  for (const auto& row : text) {
    for (const char& c : row) {
      std::cout << c;
    }
    std::cout << std::endl;
  }
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  Point2DSet points = ParsePoints(argv[1]);
  FoldVec folds = ParseFolds(argv[1]);
  FoldAndPrint(folds, points);

  std::exit(EXIT_SUCCESS);
}
