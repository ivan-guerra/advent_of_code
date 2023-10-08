#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

int GetTotalSquareFootage(const std::string& dimensions) {
  std::istringstream iss(dimensions);
  std::string token;
  std::vector<int> dims;
  while (std::getline(iss, token, 'x')) {
    dims.push_back(std::stoi(token));
  }

  int length = dims[0];
  int width = dims[1];
  int height = dims[2];
  int total_sq_footage =
      2 * ((length * width) + (width * height) + (height * length));
  int present_sq_footage =
      std::min(width * length, std::min(length * height, width * height));

  return (total_sq_footage + present_sq_footage);
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream input(argv[1]);
  std::string gift;

  int total_sq_footage = 0;
  while (std::getline(input, gift)) {
    total_sq_footage += GetTotalSquareFootage(gift);
  }
  std::cout << "Answer: " << total_sq_footage << std::endl;

  exit(EXIT_SUCCESS);
}
