#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

int GetCharsInMemory(const std::string& item) {
  int total_chars = 0;
  int i = 0;
  while (i < item.size()) {
    if ('\\' == item[i]) {
      if (item[i + 1] == '\\' || item[i + 1] == '"') {
        i++;
      } else if (item[i + 1] == 'x') {
        i += 3;
      }
    }
    i++;
    total_chars++;
  }
  return (total_chars - 2); /* -2 because we ignore the surrounding "'s. */
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream input(argv[1]);
  std::string item;
  int mem_char_count = 0;
  int code_char_count = 0;
  while (std::getline(input, item)) {
    code_char_count += item.size();
    mem_char_count += GetCharsInMemory(item);
  }

  std::cout << "Answer: " << (code_char_count - mem_char_count) << std::endl;

  exit(EXIT_SUCCESS);
}
