#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

enum class Direction {
  kForward,
  kDown,
  kUp,
};

struct SubInstruction {
  Direction direction;
  int offset = 0;
};

struct Location2D {
  int x = 0;
  int y = 0;
};

[[nodiscard]] Location2D SimulateMoves(
    const std::vector<SubInstruction>& instructions) noexcept {
  Location2D position;
  for (const SubInstruction& inst : instructions) {
    switch (inst.direction) {
      case Direction::kForward:
        position.x += inst.offset;
        break;
      case Direction::kDown:
        position.y += inst.offset;
        break;
      case Direction::kUp:
        position.y -= inst.offset;
        break;
    }
  }
  return position;
}

[[nodiscard]] std::vector<SubInstruction> ParseInstructions(
    const std::string& filename) {
  std::ifstream fhandle(filename);
  std::string line;
  std::vector<SubInstruction> instructions;
  while (std::getline(fhandle, line)) {
    int blank = line.find(' ');
    std::string direction = line.substr(0, blank);
    int offset = std::stoi(line.substr(blank + 1));

    if (direction == "forward") {
      instructions.push_back(
          {.direction = Direction::kForward, .offset = offset});
    } else if (direction == "up") {
      instructions.push_back({.direction = Direction::kUp, .offset = offset});
    } else if (direction == "down") {
      instructions.push_back({.direction = Direction::kDown, .offset = offset});
    } else {
      throw std::invalid_argument("unknown directions");
    }
  }
  return instructions;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  try {
    std::vector<SubInstruction> instructions = ParseInstructions(argv[1]);
    Location2D final_position = SimulateMoves(instructions);

    std::cout << "Answer: " << (final_position.x * final_position.y)
              << std::endl;
  } catch (const std::invalid_argument& e) {
    std::cerr << "error: " << e.what() << std::endl;
  }

  exit(EXIT_SUCCESS);
}
