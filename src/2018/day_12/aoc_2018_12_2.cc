#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using Rules = std::unordered_map<std::string, bool>;

std::vector<std::string> GetWords(const std::string& text) {
  std::istringstream iss(text);
  std::vector<std::string> words;
  std::copy(std::istream_iterator<std::string>(iss),
            std::istream_iterator<std::string>(), std::back_inserter(words));

  return words;
}

void InitRules(Rules& rules, std::ifstream& input) {
  std::string line;
  while (std::getline(input, line)) {
    std::vector<std::string> words = GetWords(line);
    rules[words[0]] = (words[2] == "#");
  }
}

int ComputePotSum(const std::string& pots, int zero_pot_index) {
  int pot_num_sum = 0;
  for (int i = zero_pot_index; i < pots.size(); ++i) {
    if ('#' == pots[i]) {
      pot_num_sum += (i - zero_pot_index);
    }
  }

  for (int i = 0; i < zero_pot_index; ++i) {
    if ('#' == pots[i]) {
      pot_num_sum += (i - zero_pot_index);
    }
  }
  return pot_num_sum;
}

long long RunSimulation(const Rules& rules, std::string& state,
                        int num_iterations) {
  const std::string kPadding = "....";
  int zero_pot_index = 0;
  while (num_iterations--) {
    state = kPadding + state + kPadding;
    std::string buffer = state;
    for (int i = 2; i <= (state.size() - kPadding.size()); ++i) {
      std::string key = buffer.substr(i - 2, 5);
      state[i] = rules.at(key) ? '#' : '.';
    }
    zero_pot_index += kPadding.size();
  }
  return ComputePotSum(state, zero_pot_index);
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream input(argv[1]);
  std::string initial_state;
  std::getline(input, initial_state);

  Rules rules;
  InitRules(rules, input);

  const long long kNumIter = 2000LL;
  const long long kGenerations = 50000000000LL;
  /* kMagic is computed by taking the difference of the first few generation
   * sums. You quickly see the deltas settle on a value which I am calling
   * kMagic. */
  const long long kMagic = 98LL;
  long long soln = ((kGenerations - kNumIter) * kMagic) +
                   RunSimulation(rules, initial_state, kNumIter);

  std::cout << "Answer: " << soln << std::endl;

  exit(EXIT_SUCCESS);
}
