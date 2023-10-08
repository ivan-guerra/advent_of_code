#include <algorithm>
#include <climits>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

int FindMaxCalories(const std::string& input) {
  std::ifstream calories(input);
  if (!calories.is_open()) {
    std::cerr << "invalid input file: " << input << std::endl;
    return -1;
  }

  std::string line;
  int curr_calories = 0;
  int max_calories = INT_MIN;
  while (std::getline(calories, line)) {
    if (line.empty())
      curr_calories = 0;
    else
      curr_calories += std::stoi(line);
    max_calories = std::max(max_calories, curr_calories);
  }

  return max_calories;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "usage: aoc_1 ELF_CALORIES" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::cout << "Max Calories Carried by Any Elf: " << FindMaxCalories(argv[1])
            << std::endl;

  exit(EXIT_SUCCESS);
}
