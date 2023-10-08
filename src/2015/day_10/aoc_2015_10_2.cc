#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

std::string LookAndSay(const std::string& seq) {
  std::string out;
  int i = 0;
  while (i < seq.size()) {
    if (((i + 1) < seq.size()) && (seq[i] == seq[i + 1])) {
      int j = i;
      while ((j < seq.size()) && (seq[j] == seq[i])) {
        j++;
      }
      int seq_len = j - i;
      out += std::to_string(seq_len);
      out += seq[i];
      i = j;
    } else {
      out += '1';
      out += seq[i];
      i++;
    }
  }
  return out;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream input(argv[1]);
  std::string sequence;
  std::getline(input, sequence);
  for (int i = 0; i < 50; ++i) {
    sequence = LookAndSay(sequence);
  }

  std::cout << "Answer: " << sequence.size() << std::endl;

  exit(EXIT_SUCCESS);
}
