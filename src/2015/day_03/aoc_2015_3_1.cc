#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_set>

std::string Stringify(int x, int y) {
  std::ostringstream os;
  os << x << ',' << y;
  return os.str();
}

int GetNumUniqueDeliveries(const std::string& moves) {
  std::unordered_set<std::string> history;
  history.insert(Stringify(0, 0));

  int x = 0;
  int y = 0;
  for (const char& move : moves) {
    switch (move) {
      case '^':
        y -= 1;
        break;
      case 'v':
        y += 1;
        break;
      case '<':
        x -= 1;
        break;
      case '>':
        x += 1;
        break;
    }
    history.insert(Stringify(x, y));
  }
  return history.size();
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream input(argv[1]);
  std::string moves;
  std::getline(input, moves);

  std::cout << "Answer: " << GetNumUniqueDeliveries(moves) << std::endl;

  exit(EXIT_SUCCESS);
}
