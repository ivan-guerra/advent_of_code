#include <fstream>
#include <functional>
#include <iostream>
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
  int x = 0;
  int y = 0;
};

struct Ship {
  Coordinate2D position;
  Direction direction;
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

void MoveShip(Ship& ship, const Direction& dir, int arg) {
  switch (dir) {
    case Direction::kNorth:
      ship.position.y += arg;
      break;
    case Direction::kSouth:
      ship.position.y -= arg;
      break;
    case Direction::kEast:
      ship.position.x += arg;
      break;
    case Direction::kWest:
      ship.position.x -= arg;
      break;
  }
}

void UpdateAngle(Ship& ship, int dtheta, bool left) {
  static const std::unordered_map<Direction, Direction> kLeftMoves = {
      {Direction::kEast, Direction::kNorth},
      {Direction::kNorth, Direction::kWest},
      {Direction::kWest, Direction::kSouth},
      {Direction::kSouth, Direction::kEast}};
  static const std::unordered_map<Direction, Direction> kRightMoves = {
      {Direction::kEast, Direction::kSouth},
      {Direction::kSouth, Direction::kWest},
      {Direction::kWest, Direction::kNorth},
      {Direction::kNorth, Direction::kEast}};

  for (int i = 0; i < dtheta; i += 90) {
    if (left) {
      ship.direction = kLeftMoves.at(ship.direction);
    } else {
      ship.direction = kRightMoves.at(ship.direction);
    }
  }
}

void Navigate(const std::vector<NavInstruction>& instructions, Ship& ship) {
  for (const NavInstruction& inst : instructions) {
    switch (inst.type) {
      case InstructionType::kNorth:
        MoveShip(ship, Direction::kNorth, inst.arg);
        break;
      case InstructionType::kSouth:
        MoveShip(ship, Direction::kSouth, inst.arg);
        break;
      case InstructionType::kEast:
        MoveShip(ship, Direction::kEast, inst.arg);
        break;
      case InstructionType::kWest:
        MoveShip(ship, Direction::kWest, inst.arg);
        break;
      case InstructionType::kForward:
        MoveShip(ship, ship.direction, inst.arg);
        break;
      case InstructionType::kRight:
        UpdateAngle(ship, inst.arg, false);
        break;
      case InstructionType::kLeft:
        UpdateAngle(ship, inst.arg, true);
        break;
    }
  }
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  Ship ship = {.position = {.x = 0, .y = 0}, .direction = Direction::kEast};
  std::vector<NavInstruction> instructions = LoadInstructions(argv[1]);
  Navigate(instructions, ship);

  int dist = std::abs(ship.position.x) + std::abs(ship.position.y);
  std::cout << "Answer: " << dist << std::endl;

  exit(EXIT_SUCCESS);
}
