#include <climits>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

char GetCharAtCol(const std::vector<std::string>& messages, int k) {
  std::unordered_map<char, int> char_frequencies;
  for (int i = 0; i < messages.size(); ++i) {
    char_frequencies[messages[i][k]]++;
  }

  char least_frequent = '\0';
  int least_frequent_ctr = INT_MAX;
  for (const auto& [character, freq] : char_frequencies) {
    if (freq < least_frequent_ctr) {
      least_frequent_ctr = freq;
      least_frequent = character;
    }
  }
  return least_frequent;
}

std::string DecipherMessage(const std::vector<std::string>& messages) {
  std::string deciphered_msg;
  for (int i = 0; i < messages[0].size(); ++i) {
    deciphered_msg += GetCharAtCol(messages, i);
  }
  return deciphered_msg;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream input(argv[1]);
  std::string message;
  std::vector<std::string> messages;
  while (std::getline(input, message)) {
    messages.push_back(message);
  }

  std::cout << "Answer: " << DecipherMessage(messages) << std::endl;

  exit(EXIT_SUCCESS);
}
