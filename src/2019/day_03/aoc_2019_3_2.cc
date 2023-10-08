
#include <climits>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

struct Point2D {
  int x = 0;
  int y = 0;

  friend bool operator==(const Point2D a, const Point2D b) {
    return ((a.x == b.x) && (a.y == b.y));
  }
};

struct Point2DHash {
  std::size_t operator()(const Point2D& p) const {
    std::string point_str = std::to_string(p.x) + "," + std::to_string(p.y);
    return std::hash<std::string>{}(point_str);
  }
};

enum Direction {
  kUp = 'U',
  kDown = 'D',
  kLeft = 'L',
  kRight = 'R',
};

struct Instruction {
  Direction direction;
  unsigned int magnitude;

  Instruction(const Direction& dir, unsigned int mag)
      : direction(dir), magnitude(mag) {}
};

using PointSet = std::unordered_set<Point2D, Point2DHash>;

std::vector<Instruction> ParseInstructions(const std::string& wire_str) {
  std::vector<Instruction> instructions;
  std::stringstream ss(wire_str);
  std::string token;
  while (std::getline(ss, token, ',')) {
    std::string dir = token.substr(0, 1);
    unsigned int mag = std::stoi(token.substr(1));
    switch (dir[0]) {
      case Direction::kUp:
        instructions.emplace_back(Direction::kUp, mag);
        break;
      case Direction::kDown:
        instructions.emplace_back(Direction::kDown, mag);
        break;
      case Direction::kLeft:
        instructions.emplace_back(Direction::kLeft, mag);
        break;
      case Direction::kRight:
        instructions.emplace_back(Direction::kRight, mag);
        break;
    }
  }
  return instructions;
}

PointSet ParsePoints(const std::vector<Instruction>& instructions) {
  PointSet points;
  Point2D curr_loc = {0, 0};
  for (const Instruction& instr : instructions) {
    for (int i = 0; i < instr.magnitude; ++i) {
      switch (instr.direction) {
        case Direction::kUp:
          curr_loc.y -= 1;
          points.insert(curr_loc);
          break;
        case Direction::kDown:
          curr_loc.y += 1;
          points.insert(curr_loc);
          break;
        case Direction::kLeft:
          curr_loc.x -= 1;
          points.insert(curr_loc);
          break;
        case Direction::kRight:
          curr_loc.x += 1;
          points.insert(curr_loc);
          break;
      }
    }
  }
  return points;
}

PointSet Intersection(const PointSet& wire1, const PointSet& wire2) {
  PointSet intersection;
  for (const Point2D p : wire1) {
    if (wire2.count(p)) {
      intersection.insert(p);
    }
  }
  return intersection;
}

int CountSteps(const std::vector<Instruction>& instructions,
               const Point2D& target) {
  Point2D curr_loc = {0, 0};
  int steps = 0;
  for (const Instruction& instr : instructions) {
    for (int i = 0; i < instr.magnitude; ++i) {
      switch (instr.direction) {
        case Direction::kUp:
          curr_loc.y -= 1;
          break;
        case Direction::kDown:
          curr_loc.y += 1;
          break;
        case Direction::kLeft:
          curr_loc.x -= 1;
          break;
        case Direction::kRight:
          curr_loc.x += 1;
          break;
      }

      steps++;
      if (curr_loc == target) {
        return steps;
      }
    }
  }
  return -1; /* unreachable */
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream input(argv[1]);
  std::string wire1;
  std::string wire2;
  std::getline(input, wire1);
  std::getline(input, wire2);

  auto wire1_instructions = ParseInstructions(wire1);
  auto wire2_instructions = ParseInstructions(wire2);

  PointSet wire1_points = ParsePoints(wire1_instructions);
  PointSet wire2_points = ParsePoints(wire2_instructions);
  PointSet wire_intersects = Intersection(wire1_points, wire2_points);

  int min_delay = INT_MAX;
  for (const Point2D p : wire_intersects) {
    int wire_delay1 = CountSteps(wire1_instructions, p);
    int wire_delay2 = CountSteps(wire2_instructions, p);
    min_delay = std::min(min_delay, wire_delay1 + wire_delay2);
  }

  std::cout << "Answer: " << min_delay << std::endl;

  exit(EXIT_SUCCESS);
}
