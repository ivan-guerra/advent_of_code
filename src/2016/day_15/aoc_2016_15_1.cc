#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

struct Disc {
  int num_positions = 0;
  int start_pos = 0;
  int disc_num = 0;
};

Disc ParseDisc(const std::string& text) {
  std::istringstream iss(text);
  std::vector<std::string> tokens;
  std::copy(std::istream_iterator<std::string>(iss),
            std::istream_iterator<std::string>(), std::back_inserter(tokens));

  Disc disc = {
      .num_positions = std::stoi(tokens[3]),
      .start_pos = std::stoi(tokens[11].substr(0, tokens[11].size() - 1)),
      .disc_num = std::stoi(tokens[1].substr(1)),
  };
  return disc;
}

bool CapturedCapsule(const std::vector<Disc>& discs, int start_time) {
  for (const Disc& disc : discs) {
    int end_pos = (start_time + disc.disc_num) + disc.start_pos;
    if (end_pos % disc.num_positions) {
      return false;
    }
  }
  return true;
}

int GetCapsuleCaptureTime(const std::vector<Disc>& discs) {
  int capture_time = 0;
  while (!CapturedCapsule(discs, capture_time)) {
    capture_time++;
  }
  return capture_time;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream input(argv[1]);
  std::string disc;
  std::vector<Disc> discs;
  while (std::getline(input, disc)) {
    discs.push_back(ParseDisc(disc));
  }

  std::cout << "Answer: " << GetCapsuleCaptureTime(discs) << std::endl;

  exit(EXIT_SUCCESS);
}
