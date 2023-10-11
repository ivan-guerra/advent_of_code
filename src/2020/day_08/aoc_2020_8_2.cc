#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

enum class OpCode {
  kNop,
  kAcc,
  kJmp,
};

struct Instruction {
  OpCode opcode;
  int arg;
};

std::vector<Instruction> LoadInstructions(const std::string& inst_file) {
  std::fstream fhandle(inst_file);
  std::string line;
  std::vector<Instruction> instructions;
  while (std::getline(fhandle, line)) {
    std::stringstream ss(line);
    std::string opcode;
    ss >> opcode;
    std::string arg;
    ss >> arg;

    OpCode code = OpCode::kNop;
    if (opcode == "nop") {
      code = OpCode::kNop;
    } else if (opcode == "acc") {
      code = OpCode::kAcc;
    } else if (opcode == "jmp") {
      code = OpCode::kJmp;
    }
    instructions.push_back({.opcode = code, .arg = std::stoi(arg)});
  }
  return instructions;
}

bool ProgramTerminates(const std::vector<Instruction>& instructions) {
  std::unordered_set<int> inst_history;
  int pc = 0;
  while (pc < instructions.size()) {
    if (inst_history.count(pc)) {
      return false;
    }
    inst_history.insert(pc);

    switch (instructions[pc].opcode) {
      case OpCode::kNop:
        pc++;
        break;
      case OpCode::kAcc:
        pc++;
        break;
      case OpCode::kJmp:
        pc += instructions[pc].arg;
        break;
    }
  }
  return true;
}

int Run(const std::vector<Instruction>& instructions) {
  int pc = 0;
  int accumulator = 0;
  while (pc < instructions.size()) {
    switch (instructions[pc].opcode) {
      case OpCode::kNop:
        pc++;
        break;
      case OpCode::kAcc:
        accumulator += instructions[pc].arg;
        pc++;
        break;
      case OpCode::kJmp:
        pc += instructions[pc].arg;
        break;
    }
  }
  return accumulator;
}

int Solve(const std::vector<Instruction>& instructions) {
  std::vector<Instruction> instructions_modified;
  for (int i = 0; i < instructions.size(); ++i) {
    instructions_modified = instructions;
    if (instructions_modified[i].opcode == OpCode::kJmp ||
        instructions_modified[i].opcode == OpCode::kNop) {
      instructions_modified[i].opcode =
          OpCode::kNop == instructions_modified[i].opcode ? OpCode::kJmp
                                                          : OpCode::kNop;
      if (ProgramTerminates(instructions_modified)) {
        return Run(instructions_modified);
      }
    }
  }
  return -1;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::vector<Instruction> instructions = LoadInstructions(argv[1]);
  std::cout << "Answer: " << Solve(instructions) << std::endl;

  exit(EXIT_SUCCESS);
}
