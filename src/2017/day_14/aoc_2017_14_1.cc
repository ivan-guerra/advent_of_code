#include <algorithm>
#include <bitset>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

static constexpr int kHashLen = 128;

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

std::string HexCharToBin(char c) {
  switch (c) {
    case '0':
      return "0000";
    case '1':
      return "0001";
    case '2':
      return "0010";
    case '3':
      return "0011";
    case '4':
      return "0100";
    case '5':
      return "0101";
    case '6':
      return "0110";
    case '7':
      return "0111";
    case '8':
      return "1000";
    case '9':
      return "1001";
    case 'a':
      return "1010";
    case 'b':
      return "1011";
    case 'c':
      return "1100";
    case 'd':
      return "1101";
    case 'e':
      return "1110";
    case 'f':
      return "1111";
  }
  return "";
}

std::bitset<kHashLen> KnotHashToBin(const std::string& hash) {
  std::string bin_str;
  for (const char& c : hash) {
    bin_str += HexCharToBin(c);
  }

  std::bitset<kHashLen> bin;
  int bin_idx = bin.size() - 1;
  for (int i = 0; i < bin.size(); ++i) {
    bin[bin_idx--] = static_cast<int>(bin_str[i] - '0');
  }
  return bin;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream input(argv[1]);
  std::string seed;
  std::getline(input, seed);

  int used_squares = 0;
  for (int i = 0; i < kHashLen; ++i) {
    std::string key = seed + "-" + std::to_string(i);
    std::string knot_hash = ComputeHash(ParseRopeLengths(key));
    std::bitset<kHashLen> row_bits = KnotHashToBin(knot_hash);
    used_squares += row_bits.count();
  }

  std::cout << "Answer: " << used_squares << std::endl;

  exit(EXIT_SUCCESS);
}
