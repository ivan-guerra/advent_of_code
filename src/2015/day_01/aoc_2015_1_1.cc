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

  int floor = 0;
  for (const char& step : instructions) {
    floor = (step == '(') ? (floor + 1) : (floor - 1);
  }
  std::cout << "Answer: " << floor << std::endl;

  exit(EXIT_SUCCESS);
}
