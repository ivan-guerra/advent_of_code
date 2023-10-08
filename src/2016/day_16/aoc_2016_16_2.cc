#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::vector<int> GetDragonCurve(const std::string& init, int disk_size) {
  /* Prep the initial curve vector. */
  std::vector<int> curve;
  for (const char& c : init) {
    curve.push_back(c - '0');
  }

  while (curve.size() < disk_size) {
    std::vector<int> tmp = curve;
    std::reverse(tmp.begin(), tmp.end());
    for (int& i : tmp) {
      i = (i) ? 0 : 1;
    }
    curve.push_back(0);
    curve.insert(curve.end(), tmp.begin(), tmp.end());
  }

  /* Trim off trailing data that does not fit on disk. */
  curve.erase(curve.begin() + disk_size, curve.end());

  return curve;
}

std::vector<int> GetChecksum(const std::vector<int>& curve) {
  std::vector<int> curr_curve = curve;
  std::vector<int> checksum;
  do {
    checksum.clear();
    for (int i = 1; i < curr_curve.size(); i += 2) {
      if (curr_curve[i] == curr_curve[i - 1]) {
        checksum.push_back(1);
      } else {
        checksum.push_back(0);
      }
    }
    curr_curve = checksum;
  } while (0 == (checksum.size() % 2));

  return checksum;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream input(argv[1]);
  std::string init_state;
  std::getline(input, init_state);

  const int kDiskSize = 35651584;
  std::vector<int> checksum =
      GetChecksum(GetDragonCurve(init_state, kDiskSize));

  std::cout << "Answer: ";
  for (const int& i : checksum) {
    std::cout << i;
  }
  std::cout << std::endl;

  exit(EXIT_SUCCESS);
}
