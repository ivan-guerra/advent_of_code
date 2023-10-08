#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int ComputeSpinlockValue(int step, int num_iter) {
  std::vector<int> buffer;
  buffer.push_back(0);

  int curr_pos = 0;
  for (int i = 1; i <= num_iter; ++i) {
    for (int j = 0; j < step; ++j) {
      curr_pos = (curr_pos + 1) % buffer.size();
    }

    curr_pos++;
    if (buffer.size() == curr_pos) {
      buffer.push_back(i);
    } else {
      buffer.insert(buffer.begin() + curr_pos, i);
    }
  }
  return buffer[(curr_pos + 1) % buffer.size()];
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
  const int kNumIter = 2017;
  std::cout << "Answer: " << ComputeSpinlockValue(step, kNumIter) << std::endl;

  exit(EXIT_SUCCESS);
}
