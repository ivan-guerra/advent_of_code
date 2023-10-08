#include <cstdlib>
#include <fstream>
#include <functional>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_set>

struct Coordinate2D {
  int x = 0;
  int y = 0;

  Coordinate2D(int x_, int y_) : x(x_), y(y_) {}

  friend bool operator==(const Coordinate2D& a, const Coordinate2D& b) {
    return (a.x == b.x) && (a.y == b.y);
  }
};

struct Coordinate2DHash {
  std::size_t operator()(const Coordinate2D& c) const {
    std::string hash_str = std::to_string(c.x) + "," + std::to_string(c.y);
    return std::hash<std::string>{}(hash_str);
  }
};

bool IsWall(const Coordinate2D& pos, int designer_num) {
  int value = (pos.x * pos.x) + (3 * pos.x) + (2 * pos.x * pos.y) + pos.y +
              (pos.y * pos.y);
  value += designer_num;

  int num_ones = 0;
  while (value) {
    num_ones++;
    value &= (value - 1);
  }
  return (num_ones & 1);
}

bool IsValidPosition(const Coordinate2D& pos) {
  return (pos.x >= 0) && (pos.y >= 0);
}

std::vector<Coordinate2D> GetNeighbors(const Coordinate2D& pos) {
  Coordinate2D p1(pos.x, pos.y + 1);
  Coordinate2D p2(pos.x + 1, pos.y);
  Coordinate2D p3(pos.x, pos.y - 1);
  Coordinate2D p4(pos.x - 1, pos.y);
  return {p1, p2, p3, p4};
}

int GetMinPathLen(const Coordinate2D& target, int designer_num) {
  Coordinate2D initial_pos(1, 1);
  std::queue<Coordinate2D> nodes;
  nodes.push(initial_pos);

  int steps = 0;
  std::unordered_set<Coordinate2D, Coordinate2DHash> visited;
  while (!nodes.empty()) {
    int num_nodes = nodes.size();
    for (int i = 0; i < num_nodes; ++i) {
      Coordinate2D curr = nodes.front();
      nodes.pop();

      if (curr == target) {
        return steps;
      }

      visited.insert(curr);
      auto neighbors = GetNeighbors(curr);
      for (const Coordinate2D& neighbor : neighbors) {
        if (IsValidPosition(neighbor) && !visited.count(neighbor) &&
            !IsWall(neighbor, designer_num)) {
          nodes.push(neighbor);
        }
      }
    }
    steps++;
  }
  return -1;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream input(argv[1]);
  std::string designer_num;
  std::getline(input, designer_num);

  Coordinate2D target(31, 39);
  std::cout << "Answer: " << GetMinPathLen(target, std::stoi(designer_num))
            << std::endl;

  exit(EXIT_SUCCESS);
}
