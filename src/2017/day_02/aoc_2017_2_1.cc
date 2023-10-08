#include <climits>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

using Spreadsheet = std::vector<std::vector<int>>;

int ComputeChecksum(const Spreadsheet& spreadsheet) {
  int checksum = 0;
  for (const std::vector<int>& row : spreadsheet) {
    int min = INT_MAX;
    int max = INT_MIN;
    for (const int& val : row) {
      min = std::min(min, val);
      max = std::max(max, val);
    }
    checksum += (max - min);
  }
  return checksum;
}

std::vector<int> ParseRow(const std::string& text) {
  std::istringstream iss(text);
  std::vector<std::string> tokens;
  std::copy(std::istream_iterator<std::string>(iss),
            std::istream_iterator<std::string>(), std::back_inserter(tokens));

  std::vector<int> values;
  for (const std::string& token : tokens) {
    values.push_back(std::stoi(token));
  }
  return values;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream input(argv[1]);
  std::string row;
  Spreadsheet spreadsheet;
  while (std::getline(input, row)) {
    spreadsheet.push_back(ParseRow(row));
  }

  std::cout << "Answer: " << ComputeChecksum(spreadsheet) << std::endl;

  exit(EXIT_SUCCESS);
}
