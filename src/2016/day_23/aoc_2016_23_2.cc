#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

enum class InstructionType {
  kCpy,
  kInc,
  kDec,
  kJnz,
  kTgl,
  kMul,
  kAdd,
  kNop,
};

struct Instruction {
  InstructionType type;
  std::string arg1;
  std::string arg2;
};

class VirtualMachine {
 public:
  VirtualMachine();
  ~VirtualMachine() = default;
  VirtualMachine(const VirtualMachine&) = default;
  VirtualMachine(VirtualMachine&&) = default;

  VirtualMachine& operator=(const VirtualMachine&) = default;
  VirtualMachine& operator=(VirtualMachine&&) = default;

  int64_t ReadRegister(const std::string& reg_label) const {
    return registers_.at(reg_label);
  }

  void Execute(std::vector<Instruction>& instructions);

 private:
  using RegisterMap = std::unordered_map<std::string, int64_t>;

  bool IsReg(const std::string& label) const {
    return (registers_.count(label) > 0);
  }

  int64_t EvaluateArg(const std::string& arg) const {
    return (IsReg(arg)) ? registers_.at(arg) : std::stoll(arg);
  }

  bool IsValidInstruction(const Instruction& inst, int num_inst) const;

  Instruction ToggleInstruction(const Instruction& inst) const;

  int ip_;
  RegisterMap registers_;
};

bool VirtualMachine::IsValidInstruction(const Instruction& inst,
                                        int num_inst) const {
  switch (inst.type) {
    case InstructionType::kInc:
    case InstructionType::kDec:
      if (!IsReg(inst.arg1)) {
        return false;
      }
      break;
    case InstructionType::kMul:
    case InstructionType::kAdd:
    case InstructionType::kCpy:
      if (!IsReg(inst.arg2)) {
        return false;
      }
      break;
    case InstructionType::kJnz: {
      int64_t target_ip = ip_ + EvaluateArg(inst.arg2);
      if (target_ip < 0) {
        return false;
      }
      break;
    }
    case InstructionType::kTgl: {
      int64_t target_ip = ip_ + EvaluateArg(inst.arg1);
      if ((target_ip < 0) || (target_ip >= num_inst)) {
        return false;
      }
      break;
    }
    case InstructionType::kNop:
      break;
  }
  return true;
}

Instruction VirtualMachine::ToggleInstruction(const Instruction& inst) const {
  Instruction toggled_inst = inst;
  switch (inst.type) {
    case InstructionType::kInc:
      toggled_inst.type = InstructionType::kDec;
      break;
    case InstructionType::kDec:
    case InstructionType::kTgl:
      toggled_inst.type = InstructionType::kInc;
      break;
    case InstructionType::kJnz:
      toggled_inst.type = InstructionType::kCpy;
      break;
    case InstructionType::kCpy:
    case InstructionType::kMul:
    case InstructionType::kAdd:
      toggled_inst.type = InstructionType::kJnz;
      break;
    case InstructionType::kNop:
      break;
  }
  return toggled_inst;
}

VirtualMachine::VirtualMachine() : ip_(0) {
  registers_["a"] = 12;
  registers_["b"] = 0;
  registers_["c"] = 0;
  registers_["d"] = 0;
}

void VirtualMachine::Execute(std::vector<Instruction>& instructions) {
  while ((ip_ >= 0) && (ip_ < instructions.size())) {
    const Instruction& inst = instructions[ip_];

    /* Skip over invalid instructions. */
    if (!IsValidInstruction(inst, instructions.size())) {
      ip_++;
      continue;
    }

    switch (inst.type) {
      case InstructionType::kCpy: {
        if (IsReg(inst.arg1)) {
          registers_[inst.arg2] = registers_[inst.arg1];
        } else {
          registers_[inst.arg2] = std::stoll(inst.arg1);
        }
        ip_++;
        break;
      }
      case InstructionType::kInc: {
        registers_[inst.arg1]++;
        ip_++;
        break;
      }
      case InstructionType::kDec: {
        registers_[inst.arg1]--;
        ip_++;
        break;
      }
      case InstructionType::kJnz: {
        if (0 != EvaluateArg(inst.arg1)) {
          ip_ += EvaluateArg(inst.arg2);
        } else {
          ip_++;
        }
        break;
      }
      case InstructionType::kTgl: {
        int64_t tgl_ip = ip_ + EvaluateArg(inst.arg1);
        instructions[tgl_ip] = ToggleInstruction(instructions[tgl_ip]);
        ip_++;
        break;
      }
      case InstructionType::kMul: {
        registers_[inst.arg2] *= EvaluateArg(inst.arg1);
        ip_++;
        break;
      }
      case InstructionType::kAdd: {
        registers_[inst.arg2] = EvaluateArg(inst.arg1) + EvaluateArg(inst.arg2);
        ip_++;
        break;
      }
      case InstructionType::kNop: {
        ip_++;
        break;
      }
    }
  }
}

Instruction ParseInstruction(const std::string& inst) {
  std::istringstream iss(inst);
  std::vector<std::string> tokens;
  std::copy(std::istream_iterator<std::string>(iss),
            std::istream_iterator<std::string>(), std::back_inserter(tokens));

  Instruction instruction;
  if (tokens[0] == "cpy") {
    instruction.type = InstructionType::kCpy;
    instruction.arg1 = tokens[1];
    instruction.arg2 = tokens[2];
  } else if (tokens[0] == "inc") {
    instruction.type = InstructionType::kInc;
    instruction.arg1 = tokens[1];
  } else if (tokens[0] == "dec") {
    instruction.type = InstructionType::kDec;
    instruction.arg1 = tokens[1];
  } else if (tokens[0] == "jnz") {
    instruction.type = InstructionType::kJnz;
    instruction.arg1 = tokens[1];
    instruction.arg2 = tokens[2];
  } else if (tokens[0] == "tgl") {
    instruction.type = InstructionType::kTgl;
    instruction.arg1 = tokens[1];
  } else if (tokens[0] == "mul") {
    instruction.type = InstructionType::kMul;
    instruction.arg1 = tokens[1];
    instruction.arg2 = tokens[2];
  } else if (tokens[0] == "add") {
    instruction.type = InstructionType::kAdd;
    instruction.arg1 = tokens[1];
    instruction.arg2 = tokens[2];
  } else if (tokens[0] == "nop") {
    instruction.type = InstructionType::kNop;
  }
  return instruction;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "error: missing puzzle input file" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::ifstream input(argv[1]);
  std::string instruction;
  std::vector<Instruction> instructions;
  while (std::getline(input, instruction)) {
    instructions.push_back(ParseInstruction(instruction));
  }

  VirtualMachine vm;
  vm.Execute(instructions);

  std::cout << "Answer: " << vm.ReadRegister("a") << std::endl;

  exit(EXIT_SUCCESS);
}
