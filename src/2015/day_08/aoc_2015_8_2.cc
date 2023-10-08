#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

std::string Encode(const std::string& item) {
  std::string encoding = "\"";
  for (const char& c : item) {
    switch (c) {
      case '"':
        encoding += '\\';
        encoding += '"';
        break;
      case '\\':
        encoding += '\\';
        encoding += '\\';
        break;
      default:
        encoding += c;
    }
  }
  encoding += '"';

  return encoding;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream input(argv[1]);
  std::string item;
  int original_code_char_count = 0;
  int encoded_char_count = 0;
  while (std::getline(input, item)) {
    std::string encoding = Encode(item);
    encoded_char_count += encoding.size();
    original_code_char_count += item.size();
  }

  std::cout << "Answer: " << (encoded_char_count - original_code_char_count)
            << std::endl;

  exit(EXIT_SUCCESS);
}
