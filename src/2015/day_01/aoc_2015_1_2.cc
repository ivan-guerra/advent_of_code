#include <cstdlib>
#include <fstream>
#include <iostream>

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream input(argv[1]);
  std::string instructions;
  std::getline(input, instructions);

  int search_pos = 0;
  int floor = 0;
  for (int i = 0; i < instructions.size(); ++i) {
    floor = (instructions[i] == '(') ? (floor + 1) : (floor - 1);
    if (floor < 0) {
      search_pos = i + 1; /* +1 because positions are 1-indexed. */
      break;
    }
  }
  std::cout << "Answer: " << search_pos << std::endl;

  exit(EXIT_SUCCESS);
}
