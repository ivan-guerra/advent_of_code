#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using Range = std::pair<int, int>;
using LightMatrix = std::vector<std::vector<int>>;

enum Action {
  kOn,
  kOff,
  kToggle,
};

struct Instruction {
  Action action;
  Range start;
  Range end;
};

Range ParseRange(const std::string& range) {
  std::istringstream iss(range);

  std::string token;
  std::vector<std::string> tokens;
  while (std::getline(iss, token, ',')) {
    tokens.push_back(token);
  }
  return std::make_pair(std::stoi(tokens[0]), std::stoi(tokens[1]));
}

Instruction ParseInstruction(const std::string& instruction) {
  std::istringstream iss(instruction);
  std::vector<std::string> tokens;
  std::copy(std::istream_iterator<std::string>(iss),
            std::istream_iterator<std::string>(), std::back_inserter(tokens));

  Instruction inst;
  if (tokens.size() == 4) {
    inst.action = Action::kToggle;
    inst.start = ParseRange(tokens[1]);
    inst.end = ParseRange(tokens[3]);
  } else {
    inst.action = (tokens[1] == "on") ? Action::kOn : Action::kOff;
    inst.start = ParseRange(tokens[2]);
    inst.end = ParseRange(tokens[4]);
  }
  return inst;
}

void ApplyInstruction(LightMatrix& lights, const Instruction& inst) {
  for (int i = inst.start.first; i <= inst.end.first; ++i) {
    for (int j = inst.start.second; j <= inst.end.second; ++j) {
      switch (inst.action) {
        case Action::kOn:
          lights[i][j]++;
          break;
        case Action::kOff:
          lights[i][j]--;
          if (lights[i][j] < 0) {
            lights[i][j] = 0;
          }
          break;
        case Action::kToggle:
          lights[i][j] += 2;
          break;
      }
    }
  }
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream input(argv[1]);
  std::string instruction;

  static const int kLightDim = 1000;
  LightMatrix lights(kLightDim, std::vector<int>(kLightDim, 0));
  while (std::getline(input, instruction)) {
    Instruction inst = ParseInstruction(instruction);
    ApplyInstruction(lights, inst);
  }

  int total_brightness = 0;
  for (const auto& row : lights) {
    total_brightness += std::accumulate(row.begin(), row.end(), 0);
  }

  std::cout << "Answer: " << total_brightness << std::endl;

  exit(EXIT_SUCCESS);
}
