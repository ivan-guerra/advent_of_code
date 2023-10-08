#include <algorithm>
#include <climits>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

using Spreadsheet = std::vector<std::vector<int>>;

int GetEvenlyDivisibleValue(const std::vector<int>& row) {
  for (int i = 0; i < row.size(); ++i) {
    for (int j = 0; j < row.size(); ++j) {
      if ((row[i] > row[j]) && (0 == (row[i] % row[j]))) {
        return (row[i] / row[j]);
      }
    }
  }
  return -1;
}

int ComputeChecksum(const Spreadsheet& spreadsheet) {
  int checksum = 0;
  for (const std::vector<int>& row : spreadsheet) {
    checksum += GetEvenlyDivisibleValue(row);
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
