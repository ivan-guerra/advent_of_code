#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

static const int kScreenWidth = 40;
static const int kScreenHeight = 6;

using Screen = std::vector<std::vector<char>>;

std::vector<std::string> SplitOnSpace(const std::string& s) {
  std::istringstream buffer(s);
  return std::vector<std::string>((std::istream_iterator<std::string>(buffer)),
                                  std::istream_iterator<std::string>());
}

void Tick(int& crt, int sprite, Screen& screen) {
  static int ticks = 0;

  ticks++;
  if ((sprite == crt) || ((sprite - 1) == crt) || ((sprite + 1) == crt))
    screen[ticks / kScreenWidth][crt] = '#';
  crt = (crt + 1) % kScreenWidth;
}

void RunSimulation(const std::string& input) {
  std::ifstream op_stream(input);

  std::string line;
  int x = 0;
  int crt = 0;
  int sprite = 1;
  Screen screen = std::vector<std::vector<char>>(
      kScreenHeight, std::vector<char>(kScreenWidth, ' '));

  const static std::string kAddOp = "addx";
  const static std::string kNoop = "noop";
  while (std::getline(op_stream, line)) {
    std::vector<std::string> op_tokens = SplitOnSpace(line);
    if (op_tokens.front() == kAddOp) {
      Tick(crt, sprite, screen);
      Tick(crt, sprite, screen);
      x += std::stoi(op_tokens.back());
      if (sprite < 0)
        sprite = kScreenWidth + sprite - 1;
      else
        sprite = (x + 1) % kScreenWidth;
    } else if (op_tokens.front() == kNoop) {
      Tick(crt, sprite, screen);
    }
  }
  for (int i = 0; i < screen.size(); ++i) {
    for (int j = 0; j < screen[0].size(); ++j) std::cout << screen[i][j];
    std::cout << std::endl;
  }
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "usage: aoc INPUT_FILE" << std::endl;
    exit(EXIT_FAILURE);
  }

  RunSimulation(argv[1]);

  exit(EXIT_SUCCESS);
}
