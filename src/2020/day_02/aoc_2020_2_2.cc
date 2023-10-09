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
  std::pair<int, int> valid_positions;
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
    std::pair<int, int> valid_positions = {};
    valid_positions.first = std::stoi(tokens[0].substr(0, dash_seperator));
    valid_positions.second = std::stoi(tokens[0].substr(dash_seperator + 1));
    policy.letter = tokens[1][0];
    policy.valid_positions = valid_positions;

    DatabaseEntry entry = {.password = tokens[2], .policy = policy};
    entries.push_back(entry);
  }
  return entries;
}

bool IsValidPassword(const DatabaseEntry& entry) {
  int match_count = 0;
  if (entry.policy.valid_positions.first - 1 < entry.password.size() &&
      entry.policy.letter ==
          entry.password[entry.policy.valid_positions.first - 1]) {
    match_count++;
  }

  if (entry.policy.valid_positions.second - 1 < entry.password.size() &&
      entry.policy.letter ==
          entry.password[entry.policy.valid_positions.second - 1]) {
    match_count++;
  }

  return (1 == match_count);
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
