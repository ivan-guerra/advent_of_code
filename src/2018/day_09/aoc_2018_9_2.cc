#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <list>
#include <sstream>
#include <string>
#include <vector>

std::vector<unsigned long long> SimulateMarbleGame(int num_players,
                                                   int num_marbles) {
  std::list<int> marbles(1, 0);
  std::vector<unsigned long long> scores(num_players, 0);
  int player_idx = 0;
  auto curr = marbles.begin();

  auto next = [&](auto i) {
    if (++i == marbles.end()) return marbles.begin();
    return i;
  };
  auto prev = [&](auto i) {
    if (i == marbles.begin()) i = marbles.end();
    return --i;
  };

  for (int i = 1; i <= num_marbles; ++i) {
    if (0 == (i % 23)) {
      curr = prev(prev(prev(prev(prev(prev(prev(curr)))))));
      scores[player_idx] += i + *curr;
      curr = marbles.erase(curr);
    } else {
      curr = marbles.insert(next(next(curr)), i);
    }
    player_idx = (player_idx + 1) % num_players;
  }
  return scores;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream input(argv[1]);
  std::string line;
  std::getline(input, line);
  std::istringstream iss(line);
  std::vector<std::string> words;
  std::copy(std::istream_iterator<std::string>(iss),
            std::istream_iterator<std::string>(), std::back_inserter(words));

  int num_players = std::stoi(words[0]);
  int num_marbles = std::stoi(words[6]);
  std::vector<unsigned long long> scores =
      SimulateMarbleGame(num_players, num_marbles);

  std::cout << "Answer: " << *std::max_element(scores.begin(), scores.end())
            << std::endl;

  exit(EXIT_SUCCESS);
}
