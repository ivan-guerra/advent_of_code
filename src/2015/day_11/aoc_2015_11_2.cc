#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>

bool AuditRule1(const std::string& password) {
  int window_size = 3;
  for (int i = 0; (password.size() - i) >= window_size; ++i) {
    if (((password[i + 2] - password[i + 1]) == 1) &&
        ((password[i + 1] - password[i]) == 1)) {
      return true;
    }
  }
  return false;
}

bool AuditRule2(const std::string& password) {
  static const std::unordered_set<char> kBannedLetters = {'i', 'o', 'l'};
  for (const char& c : password) {
    if (kBannedLetters.count(c)) {
      return false;
    }
  }
  return true;
}

bool AuditRule3(const std::string& password) {
  static const std::unordered_set<std::string> kLetterPairs = {
      "aa", "bb", "cc", "dd", "ee", "ff", "gg", "hh", "ii",
      "jj", "kk", "ll", "mm", "nn", "oo", "pp", "qq", "rr",
      "ss", "tt", "uu", "vv", "ww", "xx", "yy", "zz"};

  int matches = 0;
  int window_size = 2;
  std::unordered_set<std::string> match_strs;
  for (int i = 0; (password.size() - i) >= window_size; ++i) {
    std::string candidate = password.substr(i, 2);
    if (kLetterPairs.count(candidate) && !match_strs.count(candidate)) {
      match_strs.insert(candidate);
      matches++;
      if (matches == 2) {
        return true;
      }
    }
  }
  return false;
}

void UpdatePassword(std::string& password, int index) {
  if (index < 0) {
    return;
  }

  char& c = password[index];
  if ('z' == c) {
    c = 'a';
    UpdatePassword(password, index - 1);
  } else {
    c = static_cast<char>(c + 1);
  }
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream input(argv[1]);
  std::string password;
  std::getline(input, password);

  do {
    UpdatePassword(password, password.size() - 1);
  } while (!AuditRule1(password) || !AuditRule2(password) ||
           !AuditRule3(password));

  std::cout << "Answer: " << password << std::endl;

  exit(EXIT_SUCCESS);
}
