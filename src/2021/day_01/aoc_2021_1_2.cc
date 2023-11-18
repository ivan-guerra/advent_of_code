#include <cstdlib>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

[[nodiscard]] int GetNumberOfDepthIncreases(const std::vector<int>& depths,
                                            int window_size) noexcept {
  if (window_size >= depths.size()) {
    return 0;
  }

  int num_increases = 0;
  int prev_win_sum =
      std::accumulate(depths.cbegin(), depths.cbegin() + window_size, 0);
  int curr_win_sum = 0;
  for (int i = 1; (i + window_size) <= depths.size(); ++i) {
    curr_win_sum = std::accumulate(depths.cbegin() + i,
                                   depths.cbegin() + i + window_size, 0);
    if (curr_win_sum > prev_win_sum) {
      num_increases++;
    }
    prev_win_sum = curr_win_sum;
  }
  return num_increases;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::vector<int> depths;
  std::string line;
  std::ifstream fhandle(argv[1]);
  while (std::getline(fhandle, line)) {
    depths.push_back(std::stoi(line));
  }

  std::cout << "Answer: " << GetNumberOfDepthIncreases(depths, 3) << std::endl;

  exit(EXIT_SUCCESS);
}
