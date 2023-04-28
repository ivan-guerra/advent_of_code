#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream input(argv[1]);
  std::string line;
  int freq = 0;
  while (std::getline(input, line)) {
    freq += std::stoi(line);
  }

  std::cout << "Answer: " << freq << std::endl;

  exit(EXIT_SUCCESS);
}
