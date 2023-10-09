#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <unordered_map>
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

bool IsValidPassport(const Passport& passport) {
  static const std::vector<std::string> kRequiredFields = {
      "byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"};

  for (const std::string& field : kRequiredFields) {
    if (!passport.count(field)) {
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
