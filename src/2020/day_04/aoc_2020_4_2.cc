#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using Passport = std::unordered_map<std::string, std::string>;

Passport ParsePassport(const std::vector<std::string>& pp_data) {
  Passport passport;
  for (const std::string& entry : pp_data) {
    int colon_idx = entry.find(":");
    passport[entry.substr(0, colon_idx)] = entry.substr(colon_idx + 1);
  }
  return passport;
}

std::vector<std::string> SplitOnWhitespace(const std::string& s) {
  std::istringstream iss(s);
  std::vector<std::string> tokens;
  std::copy(std::istream_iterator<std::string>(iss),
            std::istream_iterator<std::string>(), std::back_inserter(tokens));
  return tokens;
}

std::vector<Passport> LoadPassports(const std::string infile) {
  std::vector<Passport> passports;

  std::ifstream pp_file(infile);
  std::string line;
  std::string pp_buffer;
  while (std::getline(pp_file, line)) {
    if (line.empty()) {
      passports.push_back(ParsePassport(SplitOnWhitespace(pp_buffer)));
      pp_buffer.clear();
    } else {
      pp_buffer = pp_buffer + " " + line;
    }
  }
  passports.push_back(ParsePassport(SplitOnWhitespace(pp_buffer)));

  return passports;
}

bool IsValidBirthYear(const std::string& byr) {
  const int kMinYear = 1920;
  const int kMaxYear = 2002;
  int birth_year = std::stoi(byr);
  return ((kMinYear <= birth_year) && (kMaxYear >= birth_year));
}

bool IsValidIssueYear(const std::string& iyr) {
  const int kMinYear = 2010;
  const int kMaxYear = 2020;
  int issue_year = std::stoi(iyr);
  return ((kMinYear <= issue_year) && (kMaxYear >= issue_year));
}

bool IsValidExpirationYear(const std::string& eyr) {
  const int kMinYear = 2020;
  const int kMaxYear = 2030;
  int exp_year = std::stoi(eyr);
  return ((kMinYear <= exp_year) && (kMaxYear >= exp_year));
}

bool IsValidHeight(const std::string& hgt) {
  const int kMinHgtCm = 150;
  const int kMaxHgtCm = 193;
  const int kMinHgtIn = 59;
  const int kMaxHgtIn = 76;

  bool is_valid = false;
  int end_idx = 0;
  if (std::string::npos != hgt.find("cm")) {
    end_idx = hgt.find("cm");
    int hgt_cm = std::stoi(hgt.substr(0, end_idx));
    is_valid = (hgt_cm >= kMinHgtCm) && (hgt_cm <= kMaxHgtCm);
  } else if (std::string::npos != hgt.find("in")) {
    end_idx = hgt.find("in");
    int hgt_in = std::stoi(hgt.substr(0, end_idx));
    is_valid = (hgt_in >= kMinHgtIn) && (hgt_in <= kMaxHgtIn);
  }
  return is_valid;
}

bool IsValidHairColor(const std::string& hcl) {
  if (hcl.empty() || hcl[0] != '#') {
    return false;
  }

  const std::unordered_set<char> kValidChars = {'0', '1', '2', '3', '4', '5',
                                                '6', '7', '8', '9', 'a', 'b',
                                                'c', 'd', 'e', 'f'};
  return std::all_of(
      std::begin(hcl) + 1, std::end(hcl),
      [&kValidChars](const char c) { return kValidChars.count(c); });
}

bool IsValidEyeColor(const std::string ecl) {
  const std::unordered_set<std::string> kValidColors = {
      "amb", "blu", "brn", "gry", "grn", "hzl", "oth"};
  return kValidColors.count(ecl);
}

bool IsValidPassportId(const std::string& pid) {
  if (pid.size() != 9) {
    return false;
  }
  return std::all_of(std::begin(pid), std::end(pid),
                     [](char c) { return std::isdigit(c); });
}

bool IsValidPassport(const Passport& passport) {
  static const std::unordered_map<std::string,
                                  std::function<bool(const std::string&)>>
      kRequiredFields = {
          {"byr", IsValidBirthYear},      {"iyr", IsValidIssueYear},
          {"eyr", IsValidExpirationYear}, {"hgt", IsValidHeight},
          {"hcl", IsValidHairColor},      {"ecl", IsValidEyeColor},
          {"pid", IsValidPassportId}};

  for (const auto& [field, func] : kRequiredFields) {
    if (!passport.count(field) || !func(passport.at(field))) {
      return false;
    }
  }
  return true;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::vector<Passport> passports = LoadPassports(argv[1]);
  int valid_passports = std::count_if(std::begin(passports),
                                      std::end(passports), IsValidPassport);
  std::cout << "Answer: " << valid_passports << std::endl;

  exit(EXIT_SUCCESS);
}
