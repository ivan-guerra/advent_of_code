#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct Coordinate2D {
  int x = 0;
  int y = 0;
  Coordinate2D(int x_, int y_) : x(x_), y(y_) {}
};

std::string DecipherKeyCode(const std::vector<std::string>& instructions) {
  using KeyMatrix = std::vector<std::vector<char>>;
  static const KeyMatrix kKeys = {
      {'1', '2', '3'},
      {'4', '5', '6'},
      {'7', '8', '9'},
  };

  std::string keycode;
  Coordinate2D curr_pos(1, 1);
  for (const std::string& inst : instructions) {
    int x = curr_pos.x;
    int y = curr_pos.y;
    for (const char& dir : inst) {
      switch (dir) {
        case 'U':
          x--;
          break;
        case 'D':
          x++;
          break;
        case 'L':
          y--;
          break;
        case 'R':
          y++;
          break;
      }
      if ((x >= 0) && (x < kKeys.size()) && (y >= 0) && (y < kKeys.size())) {
        curr_pos.x = x;
        curr_pos.y = y;
      } else {
        x = curr_pos.x;
        y = curr_pos.y;
      }
    }
    keycode += kKeys[curr_pos.x][curr_pos.y];
  }
  return keycode;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream input(argv[1]);
  std::string instruction;
  std::vector<std::string> instructions;
  while (std::getline(input, instruction)) {
    instructions.push_back(instruction);
  }

  std::cout << "Answer: " << DecipherKeyCode(instructions) << std::endl;

  exit(EXIT_SUCCESS);
}
