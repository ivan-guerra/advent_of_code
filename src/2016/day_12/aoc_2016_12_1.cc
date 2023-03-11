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

    void Execute(const std::vector<Instruction>& instructions);

   private:
    using RegisterMap = std::unordered_map<std::string, int64_t>;

    bool IsReg(const std::string& label) const {
        return (registers_.count(label) > 0);
    }

    int ip_;
    RegisterMap registers_;
};

VirtualMachine::VirtualMachine() : ip_(0) {
    registers_["a"] = 0;
    registers_["b"] = 0;
    registers_["c"] = 0;
    registers_["d"] = 0;
}

void VirtualMachine::Execute(const std::vector<Instruction>& instructions) {
    while ((ip_ >= 0) && (ip_ < instructions.size())) {
        const Instruction inst = instructions[ip_];
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
                int64_t jmp_cond_val = (IsReg(inst.arg1))
                                           ? registers_[inst.arg1]
                                           : std::stoll(inst.arg1);
                if (0 != jmp_cond_val) {
                    ip_ += std::stoi(inst.arg2);
                } else {
                    ip_++;
                }
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
