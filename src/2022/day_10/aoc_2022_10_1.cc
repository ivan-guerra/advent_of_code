#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

std::vector<std::string> SplitOnSpace(const std::string& s) {
  std::istringstream buffer(s);
  return std::vector<std::string>((std::istream_iterator<std::string>(buffer)),
                                  std::istream_iterator<std::string>());
}

int ComputeSignal(int& tick, int x) {
  const static std::unordered_set<int> kSpecialCycles = {20,  60,  100,
                                                         140, 180, 220};
  return (kSpecialCycles.count(tick)) ? tick * x : 0;
}

int ExecAddOp(int& tick, int& x, int increment, int sig_strength) {
  tick += 1;
  sig_strength += ComputeSignal(tick, x);
  tick += 1;
  x += increment;
  sig_strength += ComputeSignal(tick, x);

  return sig_strength;
}

int ExecNoop(int& tick, int& x, int sig_strength) {
  tick += 1;
  sig_strength += ComputeSignal(tick, x);

  return sig_strength;
}

int GetSignalStrength(const std::string& input) {
  std::ifstream op_stream(input);

  std::string line;
  int x = 1;
  int tick = 1;
  int sig_strength = 0;
  const static std::string kAddOp = "addx";
  const static std::string kNoop = "noop";
  while (std::getline(op_stream, line)) {
    std::vector<std::string> op_tokens = SplitOnSpace(line);
    if (op_tokens.front() == kAddOp) {
      sig_strength = ExecAddOp(tick, x, std::stoi(op_tokens[1]), sig_strength);
    } else if (op_tokens.front() == kNoop) {
      sig_strength = ExecNoop(tick, x, sig_strength);
    }
  }
  return sig_strength;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "usage: aoc INPUT_FILE" << std::endl;
    exit(EXIT_FAILURE);
  }
  std::cout << "Signal Strength: " << GetSignalStrength(argv[1]) << std::endl;

  exit(EXIT_SUCCESS);
}
