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

int CountUniqueTailPositions(const std::string& input) {
  std::ifstream move_seq(input);

  std::string line;
  Point2D head(0, 0);
  Point2D tail(0, 0);
  std::unordered_set<Point2D, Point2DHash> visited_positions;
  visited_positions.insert(tail);
  while (std::getline(move_seq, line)) {
    std::vector<std::string> tokens = SplitOnSpace(line);
    char direction = tokens[0][0];
    int num_moves = std::stoi(tokens[1]);
    while (num_moves--) {
      switch (direction) {
        case 'U':
          head.y++;
          break;
        case 'D':
          head.y--;
          break;
        case 'R':
          head.x++;
          break;
        case 'L':
          head.x--;
          break;
      }
      if (GetDistance(head, tail) > 1) {
        MoveTail(head, tail);
        visited_positions.insert(tail);
      }
    }
  }
  return visited_positions.size();
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "usage: aoc INPUT_FILE" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::cout << "Number of Visited Points: " << CountUniqueTailPositions(argv[1])
            << std::endl;

  exit(EXIT_SUCCESS);
}
