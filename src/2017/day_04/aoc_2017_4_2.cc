#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

bool DoesNotContainDupeWords(const std::vector<std::string>& words) {
  std::unordered_set<std::string> seen;
  for (const std::string& word : words) {
    if (seen.count(word)) {
      return false;
    }
    seen.insert(word);
  }
  return true;
}

bool DoesNotContainAnagramPair(const std::vector<std::string>& words) {
  for (int i = 0; i < words.size(); ++i) {
    std::string word1 = words[i];
    std::sort(word1.begin(), word1.end());
    for (int j = i + 1; j < words.size(); ++j) {
      std::string word2 = words[j];
      std::sort(word2.begin(), word2.end());
      if (word1 == word2) {
        return false;
      }
    }
  }
  return true;
}

bool IsValidPassphrase(const std::string& passphrase) {
  std::istringstream iss(passphrase);
  std::vector<std::string> words;
  std::copy(std::istream_iterator<std::string>(iss),
            std::istream_iterator<std::string>(), std::back_inserter(words));

  return (DoesNotContainDupeWords(words) && DoesNotContainAnagramPair(words));
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream input(argv[1]);
  std::string passphrase;
  int num_valid_passphrases = 0;
  while (std::getline(input, passphrase)) {
    if (IsValidPassphrase(passphrase)) {
      num_valid_passphrases++;
    }
  }
  std::cout << "Answer: " << num_valid_passphrases << std::endl;

  exit(EXIT_SUCCESS);
}
