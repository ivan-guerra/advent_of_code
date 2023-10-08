#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

struct IpAddress {
  std::vector<std::string> hypertexts;
  std::vector<std::string> texts;
};

bool IsPalindrome(const std::string& s) {
  int i = 0;
  int j = s.size() - 1;
  for (; i < j; ++i, --j) {
    if (s[i] != s[j]) {
      return false;
    }
  }
  return true;
}

bool IsAbba(const std::string& s) {
  int win_size = 4;
  for (int k = 0; (s.size() - k) >= win_size; ++k) {
    if ((s[k] != s[k + 1]) && IsPalindrome(s.substr(k, win_size))) {
      return true;
    }
  }
  return false;
}

bool IsValidIp(const IpAddress& address) {
  bool abba_text = false;
  for (const std::string& text : address.texts) {
    if (IsAbba(text)) {
      abba_text = true;
      break;
    }
  }

  if (!abba_text) {
    return false;
  }

  for (const std::string& hypertext : address.hypertexts) {
    if (IsAbba(hypertext)) {
      return false;
    }
  }
  return true;
}

IpAddress ParseAddress(const std::string& address) {
  IpAddress ip;
  int i = 0;
  bool parsing_hypertext = false;
  std::string text;
  std::string hypertext;
  while (i < address.size()) {
    if (address[i] == '[') {
      parsing_hypertext = true;
      ip.texts.push_back(text);
      text = "";
    } else if (address[i] == ']') {
      parsing_hypertext = false;
      ip.hypertexts.push_back(hypertext);
      hypertext = "";
    } else if (parsing_hypertext) {
      hypertext += address[i];
    } else {
      text += address[i];
    }
    i++;
  }

  if (!text.empty()) {
    ip.texts.push_back(text);
  }

  return ip;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream input(argv[1]);
  std::string address;
  std::vector<IpAddress> addresses;
  while (std::getline(input, address)) {
    addresses.push_back(ParseAddress(address));
  }

  int valid_ips = 0;
  for (const IpAddress& ip : addresses) {
    if (IsValidIp(ip)) {
      valid_ips++;
    }
  }
  std::cout << "Answer: " << valid_ips << std::endl;

  exit(EXIT_SUCCESS);
}
