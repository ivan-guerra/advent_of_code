#include <cmath>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

struct Point2D {
  int x;
  int y;

  Point2D(int x_, int y_) : x(x_), y(y_) {}
};

bool operator==(const Point2D& a, const Point2D& b) {
  return ((a.x == b.x) && (a.y == b.y));
}

struct Point2DHash {
  std::size_t operator()(const Point2D& p) const { return (p.x + p.y); }
};

void MoveTail(const Point2D& head, Point2D& tail) {
  if ((head.x != tail.x) && (head.y != tail.y)) {
    // Move diagonally
    (head.x > tail.x) ? tail.x++ : tail.x--;
    (head.y > tail.y) ? tail.y++ : tail.y--;
  } else if (head.x != tail.x) {
    // Move up/down
    (head.x > tail.x) ? tail.x++ : tail.x--;
  } else if (head.y != tail.y) {
    // Move left/right
    (head.y > tail.y) ? tail.y++ : tail.y--;
  }
}

int GetDistance(const Point2D& a, const Point2D& b) {
  int dx_2 = (a.x - b.x) * (a.x - b.x);
  int dy_2 = (a.y - b.y) * (a.y - b.y);
  return std::floor(std::sqrt(dx_2 + dy_2));
}

std::vector<std::string> SplitOnSpace(const std::string& s) {
  std::istringstream buffer(s);
  return std::vector<std::string>((std::istream_iterator<std::string>(buffer)),
                                  std::istream_iterator<std::string>());
}

int CountUniqueTailPositions(const std::string& input, int rope_len) {
  std::ifstream move_seq(input);

  std::string line;
  std::vector<Point2D> rope(rope_len, Point2D(0, 0));
  std::unordered_set<Point2D, Point2DHash> visited_positions;
  visited_positions.insert(rope.back());
  while (std::getline(move_seq, line)) {
    std::vector<std::string> tokens = SplitOnSpace(line);
    char direction = tokens[0][0];
    int num_moves = std::stoi(tokens[1]);
    while (num_moves--) {
      switch (direction) {
        case 'U':
          rope.front().y++;
          break;
        case 'D':
          rope.front().y--;
          break;
        case 'R':
          rope.front().x++;
          break;
        case 'L':
          rope.front().x--;
          break;
      }

      // Cascade updates to head in pairs of two down to the tail.
      for (int i = 0; i < rope.size() - 1; ++i) {
        if (GetDistance(rope[i], rope[i + 1]) > 1) {
          MoveTail(rope[i], rope[i + 1]);
        }
      }
      visited_positions.insert(rope.back());
    }
  }
  return visited_positions.size();
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "usage: aoc INPUT_FILE" << std::endl;
    exit(EXIT_FAILURE);
  }

  const int kRopeLen = 10;
  std::cout << "Number of Visited Points: "
            << CountUniqueTailPositions(argv[1], kRopeLen) << std::endl;

  exit(EXIT_SUCCESS);
}
