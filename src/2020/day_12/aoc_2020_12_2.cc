#include <cmath>
#include <fstream>
#include <functional>
#include <iostream>
#include <numbers>
#include <unordered_map>
#include <vector>

enum class Direction {
  kNorth,
  kSouth,
  kEast,
  kWest,
};

enum class InstructionType {
  kNorth,
  kSouth,
  kEast,
  kWest,
  kForward,
  kRight,
  kLeft,
};

struct NavInstruction {
  InstructionType type;
  int arg = 0;
};

struct Coordinate2D {
  double x = 0.0;
  double y = 0.0;
};

struct Ship {
  Coordinate2D position;
  Direction direction;
  Coordinate2D waypoint;
};

std::vector<NavInstruction> LoadInstructions(const std::string& infile) {
  std::ifstream fhandle(infile);
  std::string line;
  std::vector<NavInstruction> instructions;
  while (std::getline(fhandle, line)) {
    char type = line[0];
    int arg = std::stoi(line.substr(1));
    switch (type) {
      case 'N':
        instructions.push_back({.type = InstructionType::kNorth, .arg = arg});
        break;
      case 'S':
        instructions.push_back({.type = InstructionType::kSouth, .arg = arg});
        break;
      case 'E':
        instructions.push_back({.type = InstructionType::kEast, .arg = arg});
        break;
      case 'W':
        instructions.push_back({.type = InstructionType::kWest, .arg = arg});
        break;
      case 'F':
        instructions.push_back({.type = InstructionType::kForward, .arg = arg});
        break;
      case 'L':
        instructions.push_back({.type = InstructionType::kLeft, .arg = arg});
        break;
      case 'R':
        instructions.push_back({.type = InstructionType::kRight, .arg = arg});
        break;
    }
  }
  return instructions;
}

void UpdateCoordinate(Coordinate2D& coord, const Direction& dir, int arg) {
  switch (dir) {
    case Direction::kNorth:
      coord.y += arg;
      break;
    case Direction::kSouth:
      coord.y -= arg;
      break;
    case Direction::kEast:
      coord.x += arg;
      break;
    case Direction::kWest:
      coord.x -= arg;
      break;
  }
}

double DegreesToRadians(double degrees) {
  return (degrees * (std::numbers::pi / 180.0));
}

void RotateWayPoint(Coordinate2D& waypoint, double angle_deg) {
  float s = std::sin(DegreesToRadians(angle_deg));
  float c = std::cos(DegreesToRadians(angle_deg));

  /* Pivot point is the origin. */
  Coordinate2D pivot = {.x = 0, .y = 0};

  waypoint.x -= pivot.x;
  waypoint.y -= pivot.y;

  float xnew = waypoint.x * c - waypoint.y * s;
  float ynew = waypoint.x * s + waypoint.y * c;

  waypoint.x = xnew + pivot.x;
  waypoint.y = ynew + pivot.y;
}

void Navigate(const std::vector<NavInstruction>& instructions, Ship& ship) {
  for (const NavInstruction& inst : instructions) {
    switch (inst.type) {
      case InstructionType::kNorth:
        UpdateCoordinate(ship.waypoint, Direction::kNorth, inst.arg);
        break;
      case InstructionType::kSouth:
        UpdateCoordinate(ship.waypoint, Direction::kSouth, inst.arg);
        break;
      case InstructionType::kEast:
        UpdateCoordinate(ship.waypoint, Direction::kEast, inst.arg);
        break;
      case InstructionType::kWest:
        UpdateCoordinate(ship.waypoint, Direction::kWest, inst.arg);
        break;
      case InstructionType::kForward:
        UpdateCoordinate(ship.position, Direction::kEast,
                         ship.waypoint.x * inst.arg);
        UpdateCoordinate(ship.position, Direction::kNorth,
                         ship.waypoint.y * inst.arg);
        break;
      case InstructionType::kRight:
        RotateWayPoint(ship.waypoint, -inst.arg);
        break;
      case InstructionType::kLeft:
        RotateWayPoint(ship.waypoint, inst.arg);
        break;
    }
  }
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  Ship ship = {.position = {.x = 0, .y = 0},
               .direction = Direction::kEast,
               .waypoint = {.x = 10, .y = 1}};
  std::vector<NavInstruction> instructions = LoadInstructions(argv[1]);
  Navigate(instructions, ship);

  int dist = std::abs(ship.position.x) + std::abs(ship.position.y);
  std::cout << "Answer: " << dist << std::endl;

  exit(EXIT_SUCCESS);
}
