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
  kSet,
  kSub,
  kMul,
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
  if (tokens[0] == "set") {
    inst.type = InstructionType::kSet;
    inst.arg1 = tokens[1];
    inst.arg2 = tokens[2];
  } else if (tokens[0] == "sub") {
    inst.type = InstructionType::kSub;
    inst.arg1 = tokens[1];
    inst.arg2 = tokens[2];
  } else if (tokens[0] == "mul") {
    inst.type = InstructionType::kMul;
    inst.arg1 = tokens[1];
    inst.arg2 = tokens[2];
  } else if (tokens[0] == "jnz") {
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

int CountNumMuls(const std::vector<Instruction>& instructions) {
  RegisterMap registers;
  registers["a"] = 0;
  registers["b"] = 0;
  registers["c"] = 0;
  registers["d"] = 0;
  registers["e"] = 0;
  registers["f"] = 0;
  registers["g"] = 0;
  registers["h"] = 0;

  int ip = 0;
  int num_mul = 0;
  while ((ip >= 0) && (ip < instructions.size())) {
    const Instruction& inst = instructions[ip];
    switch (inst.type) {
      case InstructionType::kSet:
        registers[inst.arg1] = EvaluateArg(registers, inst.arg2);
        ip++;
        break;
      case InstructionType::kSub:
        registers[inst.arg1] -= EvaluateArg(registers, inst.arg2);
        ip++;
        break;
      case InstructionType::kMul:
        num_mul++;
        registers[inst.arg1] *= EvaluateArg(registers, inst.arg2);
        ip++;
        break;
      case InstructionType::kJmp:
        if (EvaluateArg(registers, inst.arg1) != 0) {
          ip += EvaluateArg(registers, inst.arg2);
        } else {
          ip++;
        }
        break;
    }
  }
  return num_mul; /* Unreachable */
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

  std::cout << "Answer: " << CountNumMuls(instructions) << std::endl;

  exit(EXIT_SUCCESS);
}
