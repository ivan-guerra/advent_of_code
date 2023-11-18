#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using BitStrVec = std::vector<std::string>;

using BitStrPair = std::pair<std::string, std::string>;

[[nodiscard]] BitStrVec ParseBitStrs(const std::string& filename) {
  BitStrVec bit_strs;
  std::string line;
  std::ifstream fhandle(filename);
  while (std::getline(fhandle, line)) {
    bit_strs.push_back(line);
  }
  return bit_strs;
}

[[nodiscard]] int FindMostPopularBit(const BitStrVec& bit_strs,
                                     int pos) noexcept {
  int num_ones = 0;
  int num_zeroes = 0;
  for (int i = 0; i < bit_strs.size(); ++i) {
    if (bit_strs[i][pos] == '1') {
      num_ones++;
    } else {
      num_zeroes++;
    }
  }
  return (num_ones > num_zeroes) ? 1 : 0;
}

[[nodiscard]] BitStrPair FindGammaAndEpsilon(
    const BitStrVec& bit_strs) noexcept {
  BitStrPair gamma_and_epsilon;
  for (int i = 0; i < bit_strs[0].size(); ++i) {
    int most_popular_bit = FindMostPopularBit(bit_strs, i);
    gamma_and_epsilon.first += (most_popular_bit) ? '1' : '0';
    gamma_and_epsilon.second += (most_popular_bit) ? '0' : '1';
  }
  return gamma_and_epsilon;
}

[[nodiscard]] int BitStrToInt(const std::string& bit_str) noexcept {
  int value = 0;
  for (int i = bit_str.size() - 1; i >= 0; --i) {
    if (bit_str[i] == '1') {
      value += (1 << (bit_str.size() - i - 1));
    }
  }
  return value;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  BitStrVec bit_strs = ParseBitStrs(argv[1]);
  BitStrPair gamma_and_epsilon = FindGammaAndEpsilon(bit_strs);
  int result = BitStrToInt(gamma_and_epsilon.first) *
               BitStrToInt(gamma_and_epsilon.second);
  std::cout << "Answer: " << result << std::endl;

  std::exit(EXIT_SUCCESS);
}
