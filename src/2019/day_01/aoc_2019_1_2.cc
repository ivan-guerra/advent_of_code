#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

int CalculateFuelFromMass(int mass) {
  static const int kDivisor = 3;
  static const int kSubtrahend = 2;
  return (mass / kDivisor) - kSubtrahend;
}

int DeduceFuel(int mass) {
  int fuel_sum = 0;

  int curr_fuel = CalculateFuelFromMass(mass);
  while (curr_fuel > 0) {
    fuel_sum += curr_fuel;
    curr_fuel = CalculateFuelFromMass(curr_fuel);
  }
  return fuel_sum;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream input(argv[1]);
  std::string line;
  int fuel_sum = 0;
  while (std::getline(input, line)) {
    fuel_sum += DeduceFuel(std::stoi(line));
  }

  std::cout << "Answer: " << fuel_sum << std::endl;

  exit(EXIT_SUCCESS);
}
