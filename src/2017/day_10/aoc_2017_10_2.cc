#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

std::vector<int> ParseRopeLengths(const std::string& text) {
  std::vector<int> lengths;
  for (const char& c : text) {
    lengths.push_back(static_cast<int>(c));
  }

  const std::vector<int> kSuffix = {17, 31, 73, 47, 23};
  lengths.insert(lengths.end(), kSuffix.begin(), kSuffix.end());

  return lengths;
}

std::string ComputeDenseHash(const std::vector<int>& sparse_hash) {
  std::stringstream dense_hash;
  const int kBlockSize = 16;
  for (int i = 0; i < sparse_hash.size(); i += kBlockSize) {
    int block_hash = sparse_hash[i];
    for (int j = i + 1; j < (i + kBlockSize); ++j) {
      block_hash ^= sparse_hash[j];
    }
    dense_hash << std::setw(2) << std::setfill('0') << std::hex << block_hash;
  }
  return dense_hash.str();
}

std::string ComputeHash(const std::vector<int>& lengths) {
  std::vector<int> sparse_hash(256, 0);
  for (int i = 0; i < sparse_hash.size(); ++i) {
    sparse_hash[i] = i;
  }

  int curr_pos = 0;
  int skip = 0;
  const int kNumRounds = 64;
  for (int round = 0; round < kNumRounds; ++round) {
    for (const int& length : lengths) {
      std::vector<int> tmp;
      int i = curr_pos;
      int j = length;
      while (j--) {
        tmp.push_back(sparse_hash[i]);
        i = (i + 1) % sparse_hash.size();
      }
      std::reverse(tmp.begin(), tmp.end());
      i = curr_pos;
      j = length;
      int k = 0;
      while (j--) {
        sparse_hash[i] = tmp[k++];
        i = (i + 1) % sparse_hash.size();
      }
      curr_pos = (curr_pos + length + skip) % sparse_hash.size();
      skip++;
    }
  }
  return ComputeDenseHash(sparse_hash);
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
