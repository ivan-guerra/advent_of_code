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

class VirtualMachine {
 public:
  VirtualMachine() = delete;
  explicit VirtualMachine(const std::string& inst_file) {
    LoadInstructions(inst_file);
  }

  int Run();

 private:
  void LoadInstructions(const std::string& inst_file);

  std::vector<Instruction> instructions_;
};

int VirtualMachine::Run() {
  std::unordered_set<int> inst_history;
  int pc = 0;
  int accumulator = 0;
  while (pc < instructions_.size()) {
    if (inst_history.count(pc)) {
      return accumulator;
    }
    inst_history.insert(pc);

    switch (instructions_[pc].opcode) {
      case OpCode::kNop:
        pc++;
        break;
      case OpCode::kAcc:
        accumulator += instructions_[pc].arg;
        pc++;
        break;
      case OpCode::kJmp:
        pc += instructions_[pc].arg;
        break;
    }
  }
  return accumulator;
}

void VirtualMachine::LoadInstructions(const std::string& inst_file) {
  std::fstream fhandle(inst_file);
  std::string line;
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
    instructions_.push_back({.opcode = code, .arg = std::stoi(arg)});
  }
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  VirtualMachine vm(argv[1]);
  std::cout << "Answer: " << vm.Run() << std::endl;

  exit(EXIT_SUCCESS);
}
