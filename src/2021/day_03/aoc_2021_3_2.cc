#include <cstdlib>
#include <fstream>
#include <functional>
#include <iostream>
#include <list>
#include <string>
#include <vector>

using BitStrList = std::list<std::string>;

struct BitCount {
  int one = 0;
  int zero = 0;
};

[[nodiscard]] BitStrList ParseBitStrs(const std::string& filename) {
  BitStrList bit_strs;
  std::string line;
  std::ifstream fhandle(filename);
  while (std::getline(fhandle, line)) {
    bit_strs.push_back(line);
  }
  return bit_strs;
}

[[nodiscard]] BitCount CountBits(const BitStrList& bit_strs, int pos) noexcept {
  BitCount bit_count;
  for (int i = 0; i < bit_strs.size(); ++i) {
    const std::string& bit_str = *(std::next(bit_strs.cbegin(), i));
    if (bit_str[pos] == '1') {
      bit_count.one++;
    } else {
      bit_count.zero++;
    }
  }
  return bit_count;
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

[[nodiscard]] std::string FindRating(
    const BitStrList& bit_strs, std::function<int(const BitCount&)> ScoreFunc) {
  BitStrList candidates = bit_strs;
  int bit_str_len = bit_strs.cbegin()->size();
  for (int i = 0; i < bit_str_len && candidates.size() > 1; ++i) {
    BitCount bit_count = CountBits(candidates, i);
    int most_popular_bit = ScoreFunc(bit_count);

    auto it = candidates.begin();
    while (it != candidates.end()) {
      int candidate_bit = (*it)[i] - '0';
      if (candidate_bit != most_popular_bit) {
        it = candidates.erase(it);
      } else {
        it++;
      }
    }
  }
  return candidates.front();
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  BitStrList bit_strs = ParseBitStrs(argv[1]);

  auto OxygenScoreFunc = [](const BitCount& bc) {
    return (bc.one >= bc.zero) ? 1 : 0;
  };
  auto Co2ScoreFunc = [](const BitCount& bc) {
    return (bc.one < bc.zero) ? 1 : 0;
  };
  int oxygen_rating = BitStrToInt(FindRating(bit_strs, OxygenScoreFunc));
  int co2_rating = BitStrToInt(FindRating(bit_strs, Co2ScoreFunc));

  std::cout << "Answer: " << (oxygen_rating * co2_rating) << std::endl;

  std::exit(EXIT_SUCCESS);
}
