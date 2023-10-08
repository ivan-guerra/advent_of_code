#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using RegisterMap = std::unordered_map<std::string, int64_t>;

enum InstructionType {
  kSnd,
  kSet,
  kAdd,
  kMul,
  kMod,
  kRcv,
  kJmp,
};

struct Instruction {
  InstructionType type;
  std::string arg1;
  std::string arg2;
};

Instruction ParseInstruction(const std::string& text) {
  std::istringstream iss(text);
  std::vector<std::string> tokens;
  std::copy(std::istream_iterator<std::string>(iss),
            std::istream_iterator<std::string>(), std::back_inserter(tokens));

  Instruction inst;
  if (tokens[0] == "snd") {
    inst.type = InstructionType::kSnd;
    inst.arg1 = tokens[1];
  } else if (tokens[0] == "set") {
    inst.type = InstructionType::kSet;
    inst.arg1 = tokens[1];
    inst.arg2 = tokens[2];
  } else if (tokens[0] == "add") {
    inst.type = InstructionType::kAdd;
    inst.arg1 = tokens[1];
    inst.arg2 = tokens[2];
  } else if (tokens[0] == "mul") {
    inst.type = InstructionType::kMul;
    inst.arg1 = tokens[1];
    inst.arg2 = tokens[2];
  } else if (tokens[0] == "mod") {
    inst.type = InstructionType::kMod;
    inst.arg1 = tokens[1];
    inst.arg2 = tokens[2];
  } else if (tokens[0] == "rcv") {
    inst.type = InstructionType::kRcv;
    inst.arg1 = tokens[1];
  } else if (tokens[0] == "jgz") {
    inst.type = InstructionType::kJmp;
    inst.arg1 = tokens[1];
    inst.arg2 = tokens[2];
  }
  return inst;
}

int EvaluateArg(const RegisterMap& registers, const std::string& arg) {
  if (registers.count(arg)) {
    return registers.at(arg);
  }
  return std::stoi(arg);
}

int RetrieveFirstFreq(const std::vector<Instruction>& instructions) {
  RegisterMap registers;
  int ip = 0;
  int last_freq = 0;
  while ((ip >= 0) && (ip < instructions.size())) {
    const Instruction& inst = instructions[ip];
    switch (inst.type) {
      case kSnd:
        last_freq = EvaluateArg(registers, inst.arg1);
        ip++;
        break;
      case kSet:
        registers[inst.arg1] = EvaluateArg(registers, inst.arg2);
        ip++;
        break;
      case kAdd:
        registers[inst.arg1] += EvaluateArg(registers, inst.arg2);
        ip++;
        break;
      case kMul:
        registers[inst.arg1] *= EvaluateArg(registers, inst.arg2);
        ip++;
        break;
      case kMod:
        registers[inst.arg1] %= EvaluateArg(registers, inst.arg2);
        ip++;
        break;
      case kRcv:
        if (EvaluateArg(registers, inst.arg1) != 0) {
          return last_freq;
        }
        ip++;
        break;
      case kJmp:
        if (EvaluateArg(registers, inst.arg1) > 0) {
          ip += EvaluateArg(registers, inst.arg2);
        } else {
          ip++;
        }
        break;
    }
  }
  return -1; /* Unreachable */
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream input(argv[1]);
  std::string text;
  std::vector<Instruction> instructions;
  while (std::getline(input, text)) {
    instructions.push_back(ParseInstruction(text));
  }

  std::cout << "Answer: " << RetrieveFirstFreq(instructions) << std::endl;

  exit(EXIT_SUCCESS);
}
