#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

std::vector<int> ParseRopeLengths(const std::string& text) {
  std::istringstream iss(text);
  std::vector<std::string> tokens;
  std::copy(std::istream_iterator<std::string>(iss),
            std::istream_iterator<std::string>(), std::back_inserter(tokens));

  std::vector<int> lengths;
  for (const std::string& token : tokens) {
    lengths.push_back(std::stoi(token));
  }
  return lengths;
}

int ComputeHash(const std::vector<int>& lengths) {
  std::vector<int> buffer(256, 0);
  for (int i = 0; i < buffer.size(); ++i) {
    buffer[i] = i;
  }

  int curr_pos = 0;
  int skip = 0;
  for (const int& length : lengths) {
    std::vector<int> tmp;
    int i = curr_pos;
    int j = length;
    while (j--) {
      tmp.push_back(buffer[i]);
      i = (i + 1) % buffer.size();
    }
    std::reverse(tmp.begin(), tmp.end());
    i = curr_pos;
    j = length;
    int k = 0;
    while (j--) {
      buffer[i] = tmp[k++];
      i = (i + 1) % buffer.size();
    }

    curr_pos = (curr_pos + length + skip) % buffer.size();
    skip++;
  }
  return (buffer[0] * buffer[1]);
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream input(argv[1]);
  std::string text;
  std::getline(input, text);

  std::cout << "Answer: " << ComputeHash(ParseRopeLengths(text)) << std::endl;

  exit(EXIT_SUCCESS);
}
