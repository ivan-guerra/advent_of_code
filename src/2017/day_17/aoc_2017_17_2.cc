#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int ComputeSpinlockValue(int step, int num_iter) {
  int curr_pos = 0;
  int value_after_zero = 0;
  int buffer_size = 1;
  for (int i = 1; i <= num_iter; ++i) {
    curr_pos = (curr_pos + step) % buffer_size;
    if (curr_pos == 0) {
      value_after_zero = i;
    }
    buffer_size++;
    curr_pos++;
  }
  return value_after_zero;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream input(argv[1]);
  std::string text;
  std::getline(input, text);

  int step = std::stoi(text);
  const int kNumIter = 50 * 1000000;
  std::cout << "Answer: " << ComputeSpinlockValue(step, kNumIter) << std::endl;

  exit(EXIT_SUCCESS);
}
