#include <algorithm>
#include <charconv>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <istream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

struct Policy {
  char letter;
  std::pair<int, int> frequency_rng;
};

struct DatabaseEntry {
  std::string password;
  Policy policy;
};

std::vector<DatabaseEntry> ParseDatabaseEntries(const std::string& database) {
  std::ifstream infile(database);
  std::string line;
  std::vector<DatabaseEntry> entries;
  while (std::getline(infile, line)) {
    std::istringstream iss(line);
    std::vector<std::string> tokens;
    std::copy(std::istream_iterator<std::string>(iss),
              std::istream_iterator<std::string>(), std::back_inserter(tokens));

    Policy policy;
    int dash_seperator = tokens[0].find('-');
    std::pair<int, int> frequency_rng = {};
    frequency_rng.first = std::stoi(tokens[0].substr(0, dash_seperator));
    frequency_rng.second = std::stoi(tokens[0].substr(dash_seperator + 1));
    policy.letter = tokens[1][0];
    policy.frequency_rng = frequency_rng;

    DatabaseEntry entry = {.password = tokens[2], .policy = policy};
    entries.push_back(entry);
  }
  return entries;
}

bool IsValidPassword(const DatabaseEntry& entry) {
  int occurrences =
      std::count_if(std::begin(entry.password), std::end(entry.password),
                    [&entry](char c) { return (c == entry.policy.letter); });

  if (occurrences < entry.policy.frequency_rng.first ||
      occurrences > entry.policy.frequency_rng.second) {
    return false;
  }
  return true;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::vector<DatabaseEntry> entries = ParseDatabaseEntries(argv[1]);
  int valid_passwords = std::count_if(
      std::begin(entries), std::end(entries),
      [](const DatabaseEntry& entry) { return IsValidPassword(entry); });
  std::cout << "Answer: " << valid_passwords << std::endl;

  exit(EXIT_SUCCESS);
}
